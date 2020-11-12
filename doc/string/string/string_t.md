
# string_t::string_t

Constructs a new `string_t` instance.

## Syntax

```cpp
string_t(strpool_t* pool = NULL);
string_t(char* str, strpool_t* pool = NULL);
string_t(const char* str, strpool_t* pool = NULL);
string_t(const char* begin, const char* end, strpool_t* pool = NULL);
string_t(const string_t& other, strpool_t* pool = NULL);
```

## Function Parameters

Parameter Name | Description
--- | ---
pool | This can be set to `NULL`. If `NULL` some static memory is used as the "default" global string pool. Be careful about threading concerns! No special care is taken here to handle multi-threading. If you really want to support multi-threading you can specify your own string pool by setting the pool parameter here.
str | A c-string to be converted to a `string_t` instance.
begin | The beginning of a c-string to be converted to a `string_t` instance.
end | One past the end of a c-string to be converted to a `string_t` instance.
other | Another `string_t` instance to make a copy from.

## Related Functions

[~string_t](https://github.com/RandyGaul/cute_framework/blob/master/doc/string/strpool/~string_t.md)  