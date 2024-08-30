#include <lhttp_request.h>
#include <unity/unity.h>
#include <unity/unity_fixture.h>

TEST_GROUP(TEST_REQUEST);

lhttp_request_t *request     = NULL;
const char *get_http_request = "GET / HTTP/1.1\r\n"
                               "Host: localhost:8080\r\n"
                               "User-Agent: curl/7.64.1\r\n"
                               "Accept: */*\r\n"
                               "\r\n";

// Run before each test
TEST_SETUP(TEST_REQUEST) {}

// Run after each test
TEST_TEAR_DOWN(TEST_REQUEST) {}

TEST(TEST_REQUEST, InitializeRequest)
{
	request = lhttp_request_new(strlen(get_http_request));

	TEST_ASSERT_NOT_NULL_MESSAGE(request, "Request is expected to be not NULL");

	// Check if the request line is initialized correctly
	TEST_ASSERT_NULL_MESSAGE(request->__request_line_start,
	                         "Beginning marker of request line is expected to be NULL");
	TEST_ASSERT_NULL_MESSAGE(request->__request_line_end,
	                         "Ending marker of request line is expected to be NULL");

	// Check if the method is initialized correctly
	TEST_ASSERT_NULL_MESSAGE(request->__method_start,
	                         "Beginning marker of method is expected to be NULL");
	TEST_ASSERT_NULL_MESSAGE(request->__method_end,
	                         "Ending marker of method is expected to be NULL");

	// Check if the URI is initialized correctly
	TEST_ASSERT_NULL_MESSAGE(request->__uri_start,
	                         "Beginning marker of URI is expected to be NULL");
	TEST_ASSERT_NULL_MESSAGE(request->__uri_end, "Ending marker of URI is expected to be NULL");

	// Check if the version is initialized correctly
	TEST_ASSERT_NULL_MESSAGE(request->__version_start,
	                         "Beginning marker of version is expected to be NULL");
	TEST_ASSERT_NULL_MESSAGE(request->__version_end,
	                         "Ending marker of version is expected to be NULL");

	// Check if the header section are initialized correctly
	TEST_ASSERT_NULL_MESSAGE(request->__headers_start,
	                         "Beginning marker of the header section is expected to be NULL");
	TEST_ASSERT_NULL_MESSAGE(request->__headers_end,
	                         "Ending marker of the header section is expected to be NULL");

	// Check if the buffer is initialized correctly
	TEST_ASSERT_NOT_NULL_MESSAGE(request->__buf, "Buffer is expected to be not NULL");
	TEST_ASSERT_EQUAL_UINT_MESSAGE(
	    strlen(get_http_request), request->__buf_len,
	    "Buffer length is expected to be equal to the request line length");

	TEST_PASS_MESSAGE("Request initialization test passed");
}

TEST(TEST_REQUEST, ParseRequest)
{
	int s;

	s = lhttp_request_parse(request, get_http_request, strlen(get_http_request));

	TEST_ASSERT_EQUAL_INT_MESSAGE(0, s, "Request parsing is expected to be successful");

	// Check if the request message is copied correctly
	TEST_ASSERT_EQUAL_STRING_MESSAGE(get_http_request, request->__buf,
	                                 "Request message is expected to be copied correctly");

	// Check if the request line is parsed correctly
	TEST_ASSERT_EQUAL_UINT_MESSAGE(
	    strlen("GET / HTTP/1.1"), request->__request_line_end - request->__request_line_start,
	    "Request line length is expected to be equal to the request line length");

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
