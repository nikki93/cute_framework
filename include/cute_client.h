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

#ifndef CUTE_CLIENT_H
#define CUTE_CLIENT_H

#include "cute_defines.h"
#include "cute_error.h"

namespace cute
{

struct client_t;

CUTE_API client_t* CUTE_CALL client_make(uint16_t port, uint64_t application_id, bool use_ipv6 = false, void* user_allocator_context = NULL);
CUTE_API void CUTE_CALL client_destroy(client_t* client);

CUTE_API error_t CUTE_CALL client_connect(client_t* client, const uint8_t* connect_token);
CUTE_API void CUTE_CALL client_disconnect(client_t* client);

CUTE_API void CUTE_CALL client_update(client_t* client, double dt, uint64_t current_time);
CUTE_API bool CUTE_CALL client_pop_packet(client_t* client, void** packet, int* size);
CUTE_API void CUTE_CALL client_free_packet(client_t* client, void* packet);
CUTE_API error_t CUTE_CALL client_send(client_t* client, const void* packet, int size, bool send_reliably);

enum client_state_t : int
{
	CLIENT_STATE_CONNECT_TOKEN_EXPIRED         = -6,
	CLIENT_STATE_INVALID_CONNECT_TOKEN         = -5,
	CLIENT_STATE_CONNECTION_TIMED_OUT          = -4,
	CLIENT_STATE_CHALLENGE_RESPONSE_TIMED_OUT  = -3,
	CLIENT_STATE_CONNECTION_REQUEST_TIMED_OUT  = -2,
	CLIENT_STATE_CONNECTION_DENIED             = -1,
	CLIENT_STATE_DISCONNECTED                  = 0,
	CLIENT_STATE_SENDING_CONNECTION_REQUEST    = 1,
	CLIENT_STATE_SENDING_CHALLENGE_RESPONSE    = 2,
	CLIENT_STATE_CONNECTED                     = 3,
};

CUTE_API client_state_t CUTE_CALL client_state_get(const client_t* client);
CUTE_API const char* CUTE_CALL client_state_string(client_state_t state); 
CUTE_API float CUTE_CALL client_time_of_last_packet_recieved(const client_t* client);
CUTE_API void CUTE_CALL client_enable_network_simulator(client_t* client, double latency, double jitter, double drop_chance, double duplicate_chance);

}

#endif // CUTE_CLIENT_H
