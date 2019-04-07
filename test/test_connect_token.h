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

#include <cute_client.h>
#include <cute_server.h>
#include <cute_alloc.h>

using namespace cute;

CUTE_TEST_CASE(test_generate_connect_token, "Basic test to generate a connect token and assert the expected token.");
int test_generate_connect_token()
{
	crypto_key_t client_to_server_key = crypto_generate_key();
	crypto_key_t server_to_client_key = crypto_generate_key();
	crypto_key_t shared_secret_key = crypto_generate_key();

	const char* endpoints[] = {
		"[::]:5000",
		"[::]:5001",
		"[::]:5002"
	};

	uint8_t token[CUTE_CONNECT_TOKEN_SIZE];
	CUTE_TEST_CHECK(generate_connect_token(
		~0,
		0,
		&client_to_server_key,
		&server_to_client_key,
		1,
		10,
		3,
		endpoints,
		17,
		NULL,
		&shared_secret_key,
		token
	));

	return 0;
}
