# music_set_volume

Sets the volume for music.

## Syntax

```cpp
void music_set_volume(app_t* app, float volume);
```

## Function Parameters

Parameter Name | Description
--- | ---
app | The application.
volume | A number from 0 to 1.

## Remarks

The music API is a higher level version of the [sound_play](https://github.com/RandyGaul/cute_framework/blob/master/docs/audio/sound/sound_play.md) function, and is mostly for convenience when wanting to fade or crossfade one or two music tracks together. For more fine-grained and custom control, use the [sound_play](https://github.com/RandyGaul/cute_framework/blob/master/docs/audio/sound/sound_play.md) function.

## Related Functions

[music_play](https://github.com/RandyGaul/cute_framework/blob/master/docs/audio/music/music_play.md)  
[music_stop](https://github.com/RandyGaul/cute_framework/blob/master/docs/audio/music/music_stop.md)  
[music_set_pitch](https://github.com/RandyGaul/cute_framework/blob/master/docs/audio/music/music_set_pitch.md)  
[music_set_loop](https://github.com/RandyGaul/cute_framework/blob/master/docs/audio/music/music_set_loop.md)  
[music_pause](https://github.com/RandyGaul/cute_framework/blob/master/docs/audio/music/music_pause.md)  
[music_resume](https://github.com/RandyGaul/cute_framework/blob/master/docs/audio/music/music_resume.md)  
[music_switch_to](https://github.com/RandyGaul/cute_framework/blob/master/docs/audio/music/music_switch_to.md)  
[music_crossfade](https://github.com/RandyGaul/cute_framework/blob/master/docs/audio/music/music_crossfade.md)  
