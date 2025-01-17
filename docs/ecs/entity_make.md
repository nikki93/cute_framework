# app_make_entity

Creates a new entity.

## Syntax

```cpp
entity_t entity_make(app_t* app, const char* entity_type, error_t* err = NULL);
```

## Function Parameters

Parameter Name | Description
--- | ---
app | The application.
entity_type | The type of entity to create.
err | Optional parameter for getting error details (like serialization errors).

## Return Value

Returns an instance of the specified entity type, or `INVALID_ENTITY` upon errors.

## Related Functions

[entity_is_valid](https://github.com/RandyGaul/cute_framework/blob/master/docs/ecs/entity_is_valid.md)  
[entity_is_type](https://github.com/RandyGaul/cute_framework/blob/master/docs/ecs/entity_is_type.md)  
[entity_get_type_string](https://github.com/RandyGaul/cute_framework/blob/master/docs/ecs/entity_get_type_string.md)  
[entity_has_component](https://github.com/RandyGaul/cute_framework/blob/master/docs/ecs/entity_has_component.md)  
[entity_get_component](https://github.com/RandyGaul/cute_framework/blob/master/docs/ecs/entity_get_component.md)  
[entity_destroy](https://github.com/RandyGaul/cute_framework/blob/master/docs/ecs/entity_destroy.md)  
[entity_delayed_destroy](https://github.com/RandyGaul/cute_framework/blob/master/docs/ecs/entity_delayed_destroy.md)  
[ecs_load_entities](https://github.com/RandyGaul/cute_framework/blob/master/docs/ecs/ecs_load_entities.md)  
[ecs_save_entities](https://github.com/RandyGaul/cute_framework/blob/master/docs/ecs/ecs_save_entities.md)  
