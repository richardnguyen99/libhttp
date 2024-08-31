/* Copyright (c) 2024 libhttp. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <lhttp_request.h>
#include <unity/unity.h>
#include <unity/unity_fixture.h>

TEST_GROUP(TEST_REQUEST);

lhttp_request_t *request     = NULL;
const char *get_http_request = "GET / HTTP/1.1\r\n"
                               "Host: localhost:8080\r\n"
                               "User-Agent: curl/7.68.0\r\n"
                               "Accept: */*\r\n"
                               "\r\n";

// Run before each test
TEST_SETUP(TEST_REQUEST) {}

// Run after each test
TEST_TEAR_DOWN(TEST_REQUEST) {}

TEST(TEST_REQUEST, InitializeRequest)
{
	request = (lhttp_request_t *)malloc(sizeof(lhttp_request_t));

	// Check if the request is malloc'd correctly
	TEST_ASSERT_NOT_NULL_MESSAGE(request, "Request is expected to be not NULL");

	// Initialize the request
	int s = lhttp_request_init(request, strlen(get_http_request));

	TEST_ASSERT_EQUAL_INT_MESSAGE(
	    0,
	    s,
	    "Request initialization is expected to be successful"
	);

	// Check if the status is initialized correctly
	TEST_ASSERT_EQUAL_INT_MESSAGE(
	    LHTTP_REQUEST_PARSING_INITIALIZED,
	    request->status,
	    "Request status is expected to be initialized for parsing"
	);

	// Check if the error is initialized correctly
	TEST_ASSERT_EQUAL_INT_MESSAGE(
	    LHTTP_REQUEST_ERROR_NONE,
	    request->error,
	    "Request error is expected to be none"
	);

	// Check if the request line is initialized correctly
	TEST_ASSERT_NULL_MESSAGE(
	    request->__request_line_start,
	    "Beginning marker of request line is expected to be NULL"
	);
	TEST_ASSERT_NULL_MESSAGE(
	    request->__request_line_end,
	    "Ending marker of request line is expected to be NULL"
	);

	// Check if the method is initialized correctly
	TEST_ASSERT_NULL_MESSAGE(
	    request->__method_start,
	    "Beginning marker of method is expected to be NULL"
	);
	TEST_ASSERT_NULL_MESSAGE(
	    request->__method_end,
	    "Ending marker of method is expected to be NULL"
	);

	// Check if the URI is initialized correctly
	TEST_ASSERT_NULL_MESSAGE(
	    request->__uri_start,
	    "Beginning marker of URI is expected to be NULL"
	);
	TEST_ASSERT_NULL_MESSAGE(
	    request->__uri_end,
	    "Ending marker of URI is expected to be NULL"
	);

	// Check if the version is initialized correctly
	TEST_ASSERT_NULL_MESSAGE(
	    request->__version_start,
	    "Beginning marker of version is expected to be NULL"
	);
	TEST_ASSERT_NULL_MESSAGE(
	    request->__version_end,
	    "Ending marker of version is expected to be NULL"
	);

	// Check if the header section are initialized correctly
	TEST_ASSERT_NULL_MESSAGE(
	    request->__headers_start,
	    "Beginning marker of the header section is expected to be NULL"
	);
	TEST_ASSERT_NULL_MESSAGE(
	    request->__headers_end,
	    "Ending marker of the header section is expected to be NULL"
	);

	// Check if the buffer is initialized correctly
	TEST_ASSERT_NOT_NULL_MESSAGE(
	    request->__buf,
	    "Buffer is expected to be not NULL"
	);
	TEST_ASSERT_EQUAL_UINT_MESSAGE(
	    strlen(get_http_request),
	    request->__buf_len,
	    "Buffer length is expected to be equal to the request line length"
	);

	TEST_PASS_MESSAGE("Request initialization test passed");
}

TEST(TEST_REQUEST, ParseRequest)
{
	int s;

	s = lhttp_request_parse(
	    request,
	    get_http_request,
	    strlen(get_http_request)
	);

	TEST_ASSERT_EQUAL_INT_MESSAGE(
	    0,
	    s,
	    "Request parsing is expected to be successful"
	);

	// Check if the request message is copied correctly
	TEST_ASSERT_EQUAL_STRING_MESSAGE(
	    get_http_request,
	    request->__buf,
	    "Request message is expected to be copied correctly"
	);

	// Check if the request line is parsed correctly
	TEST_ASSERT_EQUAL_UINT_MESSAGE(
	    strlen("GET / HTTP/1.1"),
	    request->__request_line_end - request->__request_line_start,
	    "Request line length is expected to be equal to the request line length"
	);

	// Check if the method is parsed correctly
	TEST_ASSERT_EQUAL_STRING_LEN_MESSAGE(
	    "GET",
	    request->__method_start,
	    request->__method_end - request->__method_start,
	    "Method is expected to be equal to 'GET'"
	);

	// Check if the URI is parsed correctly
	TEST_ASSERT_EQUAL_STRING_LEN_MESSAGE(
	    "/",
	    request->__uri_start,
	    request->__uri_end - request->__uri_start,
	    "URI is expected to be equal to '/'"
	);

	// Check if the version is parsed correctly
	TEST_ASSERT_EQUAL_STRING_LEN_MESSAGE(
	    "HTTP/1.1",
	    request->__version_start,
	    request->__version_end - request->__version_start,
	    "Version is expected to be equal to 'HTTP/1.1'"
	);

	TEST_PASS_MESSAGE("Parse request test passed");
}

TEST_GROUP_RUNNER(TEST_REQUEST)
{
	// global initialization before all tests goes here

	RUN_TEST_CASE(TEST_REQUEST, InitializeRequest);
	RUN_TEST_CASE(TEST_REQUEST, ParseRequest);

	// global clean up after all tests goes here

	// Free the request pointer
	if (request != NULL)
	{
		lhttp_request_free(request);
		free(request);
		request = NULL;
	}
}

static void RunAllTests(void)
{
	RUN_TEST_GROUP(TEST_REQUEST);
}

int main(int argc, const char *argv[])
{
	return UnityMain(argc, argv, RunAllTests);
}
