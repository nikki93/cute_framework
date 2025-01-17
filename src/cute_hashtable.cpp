/*
	Cute Framework
	Copyright (C) 2019 Randy Gaul https://randygaul.net

	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	   claim that you wrote the original software. If you use this software
	   in a product, an acknowledgment in the product documentation would be
	   appreciated but is not required.
	2. Altered source versions must be plainly marked as such, and must not be
	   misrepresented as being the original software.
	3. This notice may not be removed or altered from any source distribution.
*/

#include <cute_hashtable.h>
#include <cute_c_runtime.h>
#include <cute_alloc.h>

namespace cute
{

static CUTE_INLINE int s_is_prime(int x)
{
	if ((x == 2) | (x == 3)) return 1;
	if ((x % 2 == 0) | (x % 3 == 0)) return 0;

	int divisor = 6;
	while (divisor * divisor - 2 * divisor + 1 <= x)
	{
		if (x % (divisor - 1) == 0) return 0;
		if (x % (divisor + 1) == 0) return 0;
		divisor += 6;
	}

	return 1;
}

static CUTE_INLINE int s_next_prime(int a)
{
	while (1)
	{
		if (s_is_prime(a)) return a;
		else ++a;
	}
}

int hashtable_init(hashtable_t* table, int key_size, int item_size, int capacity, void* mem_ctx)
{
	CUTE_ASSERT(capacity);
	CUTE_MEMSET(table, 0, sizeof(hashtable_t));

	table->count = 0;
	table->slot_capacity = s_next_prime(capacity);
	table->key_size = key_size;
	table->item_size = item_size;
	int slots_size = (int)(table->slot_capacity * sizeof(*table->slots));
	table->slots = (hashtable_slot_t*)CUTE_ALLOC((size_t)slots_size, mem_ctx);
	CUTE_MEMSET(table->slots, 0, (size_t) slots_size);

	table->item_capacity = s_next_prime(capacity + capacity / 2);
	table->items_key = CUTE_ALLOC(table->item_capacity * (table->key_size + sizeof(*table->items_slot_index) + table->item_size) + table->item_size + table->key_size, mem_ctx);
	table->items_slot_index = (int*)((uint8_t*)table->items_key + table->item_capacity * table->key_size);
	table->items_data = (void*)(table->items_slot_index + table->item_capacity);
	table->temp_key = (void*)(((uintptr_t)table->items_data) + table->item_size * table->item_capacity);
	table->temp_item = (void*)(((uintptr_t)table->temp_key) + table->key_size);
	table->mem_ctx = mem_ctx;

	return 0;
}

void hashtable_cleanup(hashtable_t* table)
{
	CUTE_FREE(table->slots, mem_ctx);
	CUTE_FREE(table->items_key, mem_ctx);
}

static CUTE_INLINE int s_keys_equal(const hashtable_t* table, const void* a, const void* b)
{
	return !CUTE_MEMCMP(a, b, table->key_size);
}

static CUTE_INLINE void* s_get_key(const hashtable_t* table, int index)
{
	uint8_t* keys = (uint8_t*)table->items_key;
	return keys + index * table->key_size;
}

static CUTE_INLINE void* s_get_item(const hashtable_t* table, int index)
{
	uint8_t* items = (uint8_t*)table->items_data;
	return items + index * table->item_size;
}

static CUTE_INLINE uint64_t s_calc_hash(const void* key, int key_size)
{
	uint64_t h = (uint64_t)14695981039346656037ULL;
	const char* str = (const char*)key;

	while (key_size--)
	{
		char c = *str++;
		h = h ^ (uint64_t)c;
		h = h * (uint64_t)1099511628211ULL;
	}

	return h;
}

static int s_find_slot(const hashtable_t *table, const void* key)
{
	uint64_t hash = s_calc_hash(key, table->key_size);
	int base_slot = (int)(hash % (uint64_t)table->slot_capacity);
	int base_count = table->slots[base_slot].base_count;
	int slot = base_slot;

	while (base_count > 0)
	{
		uint64_t slot_hash = table->slots[slot].key_hash;
		if (slot_hash) {
			int slot_base = (int)(slot_hash % (uint64_t)table->slot_capacity);
			if (slot_base == base_slot) 
			{
				CUTE_ASSERT(base_count > 0);
				--base_count;
				const void* found_key = s_get_key(table, table->slots[slot].item_index);
				if (slot_hash == hash && s_keys_equal(table, found_key, key))
					return slot;
			}
		}
		slot = (slot + 1) % table->slot_capacity;
	}

	return -1;
}

static void s_expand_slots(hashtable_t* table)
{
	int const old_capacity = table->slot_capacity;
	hashtable_slot_t* old_slots = table->slots;

	table->slot_capacity *= 2;
	int slot_mask = table->slot_capacity - 1;

	int size = (int)(table->slot_capacity * sizeof(*table->slots));
	table->slots = (hashtable_slot_t*)CUTE_ALLOC(size, table->mem_ctx);
	CUTE_ASSERT(table->slots);
	CUTE_MEMSET(table->slots, 0, size);

	for (int i = 0; i < old_capacity; ++i)
	{
		uint64_t hash = old_slots[i].key_hash;
		if (hash) {
			int const base_slot = (int)(hash & (uint64_t)slot_mask);
			int slot = base_slot;
			while (table->slots[slot].key_hash)
				slot = (slot + 1) & slot_mask;
			table->slots[slot].key_hash = hash;
			int item_index = old_slots[i].item_index;
			table->slots[slot].item_index = item_index;
			table->items_slot_index[item_index] = slot; 
			++table->slots[base_slot].base_count;
		}
	}

	CUTE_FREE(old_slots, table->mem_ctx);
}

static void s_expand_items(hashtable_t* table)
{
	table->item_capacity *= 2;
	uint64_t* new_items_key = (uint64_t*)CUTE_ALLOC(table->item_capacity * (table->key_size + sizeof(*table->items_slot_index) + table->item_size) + table->item_size + table->key_size, table->mem_ctx);
	CUTE_ASSERT(new_items_key);

	int* new_items_slot_index = (int*)(new_items_key + table->item_capacity * table->key_size);
	void* new_items_data = (void*)(new_items_slot_index + table->item_capacity);
	void* new_temp_key = (void*)(((uintptr_t)new_items_data) + table->item_size * table->item_capacity);
	void* new_temp_item = (void*)(((uintptr_t)new_temp_key) + table->key_size);

	CUTE_MEMCPY(new_items_key, table->items_key, table->count * table->key_size);
	CUTE_MEMCPY(new_items_slot_index, table->items_slot_index, table->count * table->key_size);
	CUTE_MEMCPY(new_items_data, table->items_data, table->count * table->item_size);

	CUTE_FREE(table->items_key, table->mem_ctx);

	table->items_key = new_items_key;
	table->items_slot_index = new_items_slot_index;
	table->items_data = new_items_data;
	table->temp_key = new_temp_key;
	table->temp_item = new_temp_item;
}

void* hashtable_insert(hashtable_t* table, const void* key, const void* item)
{
	CUTE_ASSERT(s_find_slot(table, key) < 0);
	uint64_t hash = s_calc_hash(key, table->key_size);

	if (table->count >= table->slot_capacity) {
		s_expand_items(table);
	}

	int base_slot = (int)(hash % (uint64_t)table->slot_capacity);
	int base_count = table->slots[base_slot].base_count;
	int slot = base_slot;
	int first_free = slot;
	while (base_count)
	{
		uint64_t slot_hash = table->slots[slot].key_hash;
		if (slot_hash == 0 && table->slots[first_free].key_hash != 0) first_free = slot;
		int slot_base = (int)(slot_hash % (uint64_t)table->slot_capacity);
		if (slot_base == base_slot) 
			--base_count;
		slot = (slot + 1) % table->slot_capacity;
	}

	slot = first_free;
	while (table->slots[slot].key_hash)
		slot = (slot + 1) % table->slot_capacity;

	CUTE_ASSERT(table->count < table->item_capacity);

	CUTE_ASSERT(!table->slots[slot].key_hash && (hash % (uint64_t)table->slot_capacity) == (uint64_t)base_slot);
	CUTE_ASSERT(hash);
	table->slots[slot].key_hash = hash;
	table->slots[slot].item_index = table->count;
	++table->slots[base_slot].base_count;

	void* item_dst = s_get_item(table, table->count);
	void* key_dst = s_get_key(table, table->count);
	if (item) CUTE_MEMCPY(item_dst, item, table->item_size);
	CUTE_MEMCPY(key_dst, key, table->key_size);
    table->items_slot_index[table->count] = slot;
	++table->count;

	return item_dst;
}

void hashtable_remove(hashtable_t* table, const void* key)
{
	int slot = s_find_slot(table, key);
	CUTE_ASSERT(slot >= 0);

	uint64_t hash = table->slots[slot].key_hash;
	int base_slot = (int)(hash % (uint64_t)table->slot_capacity);
	CUTE_ASSERT(hash);
	--table->slots[base_slot].base_count;
	table->slots[slot].key_hash = 0;

	int index = table->slots[slot].item_index;
	int last_index = table->count - 1;
	if (index != last_index)
	{
		void* dst_key = s_get_key(table, index);
		void* src_key = s_get_key(table, last_index);
		CUTE_MEMCPY(dst_key, src_key, (size_t)table->key_size);
		void* dst_item = s_get_item(table, index);
		void* src_item = s_get_item(table, last_index);
		CUTE_MEMCPY(dst_item, src_item, (size_t)table->item_size);
		table->items_slot_index[index] = table->items_slot_index[last_index];
		table->slots[table->items_slot_index[last_index]].item_index = index;
	}
	--table->count;
} 

void hashtable_clear(hashtable_t* table)
{
	table->count = 0;
	CUTE_MEMSET(table->slots, 0, sizeof(*table->slots) * table->slot_capacity);
}

void* hashtable_find(const hashtable_t* table, const void* key)
{
	int slot = s_find_slot(table, key);
	if (slot < 0) return 0;

	int index = table->slots[slot].item_index;
	return s_get_item(table, index);
}

int hashtable_count(const hashtable_t* table)
{
	return table->count;
}

void* hashtable_items(const hashtable_t* table)
{
	return table->items_data;
}

void* hashtable_keys(const hashtable_t* table)
{
	return table->items_key;
}

void hashtable_swap(hashtable_t* table, int index_a, int index_b)
{
	if (index_a < 0 || index_a >= table->count || index_b < 0 || index_b >= table->count) return;

	int slot_a = table->items_slot_index[index_a];
	int slot_b = table->items_slot_index[index_b];

	table->items_slot_index[index_a] = slot_b;
	table->items_slot_index[index_b] = slot_a;

	void* key_a = s_get_key(table, index_a);
	void* key_b = s_get_key(table, index_b);
	CUTE_MEMCPY(table->temp_key, key_a, table->key_size);
	CUTE_MEMCPY(key_a, key_b, table->key_size);
	CUTE_MEMCPY(key_b, table->temp_key, table->key_size);

	void* item_a = s_get_item(table, index_a);
	void* item_b = s_get_item(table, index_b);
	CUTE_MEMCPY(table->temp_item, item_a, table->item_size);
	CUTE_MEMCPY(item_a, item_b, table->item_size);
	CUTE_MEMCPY(item_b, table->temp_item, table->item_size);

	table->slots[slot_a].item_index = index_b;
	table->slots[slot_b].item_index = index_a;
}

}
