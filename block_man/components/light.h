/*
	Cute Framework
	Copyright (C) 2020 Randy Gaul https://randygaul.net

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

#ifndef LIGHT_H
#define LIGHT_H

#include <cute.h>
using namespace cute;

#include <cute/cute_coroutine.h>

struct Light
{
	bool is_lamp = false;
	float radius = 11;
	float radius_delta = 0;
	coroutine_t co = { 0 };
};

CUTE_INLINE cute::error_t Light_serialize(app_t* app, kv_t* kv, entity_t entity, void* component, void* udata)
{
	Light* light = (Light*)component;
	if (kv_get_state(kv) == KV_STATE_READ) {
		CUTE_PLACEMENT_NEW(light) Light;
	}
	kv_key(kv, "is_lamp"); kv_val(kv, &light->is_lamp);
	kv_key(kv, "radius"); kv_val(kv, &light->radius);
	return kv_error_state(kv);
}

#endif // LIGHT_H
