# audio_stream_wav_from_memory

Loads an wav file from memory.

## Syntax

```cpp
audio_t* audio_stream_wav_from_memory(app_t* app, void* memory, int byte_count, promise_t promise, void* user_allocator_context = NULL);
```

## Function Parameters

Parameter Name | Description
--- | ---
app | The application.
memory | Pointer to the buffer storing the raw file data.
byte_count | The size of the `memory` buffer.
promise | The promise used to notify you when the streaming operation is finished.
user_allocator_context | Used for custom allocators, this can be set to `NULL`. See (TODO) for more details.

## Return Value

Returns an instance of the raw audio samples.

## Remarks

This function is asynchronous. For the synchronous io please try using [audio_load_wav_from_memory](https://github.com/RandyGaul/cute_framework/blob/master/docs/audio/audio/audio_load_wav_from_memory.md).

## Related Functions

[audio_load_ogg](https://github.com/RandyGaul/cute_framework/blob/master/docs/audio/audio/audio_load_ogg.md)  
[audio_load_wav](https://github.com/RandyGaul/cute_framework/blob/master/docs/audio/audio/audio_load_wav.md)  
[audio_load_wav_from_memory](https://github.com/RandyGaul/cute_framework/blob/master/docs/audio/audio/audio_load_wav_from_memory.md)  
[audio_stream_ogg](https://github.com/RandyGaul/cute_framework/blob/master/docs/audio/audio/audio_stream_ogg.md)  
[audio_stream_wav](https://github.com/RandyGaul/cute_framework/blob/master/docs/audio/audio/audio_stream_wav.md)  
[audio_stream_wav](https://github.com/RandyGaul/cute_framework/blob/master/docs/audio/audio/audio_stream_wav.md)  
[audio_stream_ogg_from_memory](https://github.com/RandyGaul/cute_framework/blob/master/docs/audio/audio/audio_stream_ogg_from_memory.md)  
[audio_destroy](https://github.com/RandyGaul/cute_framework/blob/master/docs/audio/audio/audio_destroy.md)  
[audio_ref_count](https://github.com/RandyGaul/cute_framework/blob/master/docs/audio/audio/audio_ref_count.md)  
