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

#ifndef CUTE_AUDIO_INTERNAL_H
#define CUTE_AUDIO_INTERNAL_H

namespace cute
{

struct audio_system_t;

audio_system_t* audio_system_make(int pool_count, void* mem_ctx = NULL);
void audio_system_destroy(audio_system_t* audio_system);
void audio_system_update(audio_system_t* audio_system, float dt);

int sound_instance_size();

}

#endif // CUTE_AUDIO_INTERNAL_H
