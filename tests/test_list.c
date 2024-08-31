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

#include <lhttp_list.h>
#include <unity/unity.h>
#include <unity/unity_fixture.h>

TEST_GROUP(TEST_LIST);

lhttp_list_t *list         = NULL;
lhttp_list_status_t status = -1;

// Run before each test
TEST_SETUP(TEST_LIST)
{
	status = lhttp_list_init(list);
}

// Run after each test
TEST_TEAR_DOWN(TEST_LIST)
{
	// Free the list
	lhttp_list_free(list);
}

TEST(TEST_LIST, InitializeList)
{
	// Check if the list is malloc'd correctly
	TEST_ASSERT_NOT_NULL_MESSAGE(list, "List is expected to be not NULL");

	// Check if the status is initialized correctly
	TEST_ASSERT_EQUAL_INT_MESSAGE(
	    LHTTP_LIST_OK,
	    status,
	    "List initialization is expected to be successful"
	);

	// Check if the size is initialized correctly
	TEST_ASSERT_EQUAL_INT_MESSAGE(
	    0,
	    list->__size,
	    "List size is expected to be initialized to 0"
	);

	// Check if the state is initialized correctly
	TEST_ASSERT_EQUAL_INT_MESSAGE(
	    LHTTP_LIST_INITIALIZED,
	    list->state,
	    "List state is expected to be initialized"
	);

	// Check if the head is initialized correctly
	TEST_ASSERT_NOT_NULL_MESSAGE(
	    list->__head,
	    "List head is expected to be not NULL"
	);

	// Check if the tail is initialized correctly
	TEST_ASSERT_NOT_NULL_MESSAGE(
	    list->__tail,
	    "List tail is expected to be not NULL"
	);

	// Check if the head is the tail after initialization
	TEST_ASSERT_EQUAL_PTR_MESSAGE(
	    list->__head,
	    list->__tail,
	    "List head and tail are expected to be different"
	);

	TEST_PASS_MESSAGE("InitializeList passed");
}

TEST(TEST_LIST, AddNode)
{
	// Safety check that the list is initialized before adding a node
	TEST_ASSERT_NOT_NULL_MESSAGE(list, "List is expected to be not NULL");
	TEST_ASSERT_EQUAL_INT_MESSAGE(
	    LHTTP_LIST_OK,
	    status,
	    "List initialization is expected to be successful"
	);

	// Add a node to the list
	status = lhttp_list_add(list, "Host", "localhost:8080");

	// Check if the status is successful
	TEST_ASSERT_EQUAL_INT_MESSAGE(
	    LHTTP_LIST_OK,
	    status,
	    "Adding a node is expected to be successful"
	);

	status = lhttp_list_add(list, "Connection", "keep-alive");

	// Check if the status is successful
	TEST_ASSERT_EQUAL_INT_MESSAGE(
	    LHTTP_LIST_OK,
	    status,
	    "Adding a node is expected to be successful"
	);

	// Check if the size is updated correctly
	TEST_ASSERT_EQUAL_INT_MESSAGE(
	    2,
	    list->__size,
	    "List size is expected to be updated to 2"
	);

	// Add a duplicate node to the list
	status = lhttp_list_add(list, "Host", "localhost:8080");

	// Check if the status is error
	TEST_ASSERT_EQUAL_INT_MESSAGE(
	    LHTTP_LIST_ERROR,
	    status,
	    "Adding a duplicate node is expected to be an error"
	);

	// Check if the error is set correctly
	TEST_ASSERT_EQUAL_INT_MESSAGE(
	    LHTTP_LIST_ERROR_KEY_EXISTS,
	    list->error,
	    "Error is expected to be key exists"
	);

	// Check if the size is not updated
	TEST_ASSERT_EQUAL_INT_MESSAGE(
	    2,
	    list->__size,
	    "List size is expected to be the same after adding a duplicate node"
	);

	TEST_PASS_MESSAGE("AddNode passed");
}

TEST(TEST_LIST, GetNode)
{
	char *value = NULL;

	// Safety check that the list is initialized before getting a node
	TEST_ASSERT_NOT_NULL_MESSAGE(list, "List is expected to be not NULL");
	TEST_ASSERT_EQUAL_INT_MESSAGE(
	    LHTTP_LIST_OK,
	    status,
	    "List initialization is expected to be successful"
	);

	// Add a node to the list
	status = lhttp_list_add(list, "Host", "localhost:8080");
	TEST_ASSERT_EQUAL_INT_MESSAGE(
	    LHTTP_LIST_OK,
	    status,
	    "Adding a node is expected to be successful"
	);

	status = lhttp_list_add(list, "Connection", "keep-alive");
	TEST_ASSERT_EQUAL_INT_MESSAGE(
	    LHTTP_LIST_OK,
	    status,
	    "Adding a node is expected to be successful"
	);

	status = lhttp_list_add(list, "User-Agent", "curl/7.68.0");
	TEST_ASSERT_EQUAL_INT_MESSAGE(
	    LHTTP_LIST_OK,
	    status,
	    "Adding a node is expected to be successful"
	);

	// Get an existing node from the list
	status = lhttp_list_get(list, "Host", &value);

	// Check if the status is successful
	TEST_ASSERT_EQUAL_INT_MESSAGE(
	    LHTTP_LIST_OK,
	    status,
	    "Getting a node is expected to be successful"
	);

	// Check if the value is correct
	TEST_ASSERT_EQUAL_STRING_MESSAGE(
	    "localhost:8080",
	    value,
	    "Value is expected to be localhost:8080"
	);

	// Get a non-existing node from the list
	status = lhttp_list_get(list, "Accept", &value);

	// Check if the status is error
	TEST_ASSERT_EQUAL_INT_MESSAGE(
	    LHTTP_LIST_ERROR,
	    status,
	    "Getting a non-existing node is expected to be an error"
	);

	// Check if the error is set correctly
	TEST_ASSERT_EQUAL_INT_MESSAGE(
	    LHTTP_LIST_ERROR_KEY_NOT_FOUND,
	    list->error,
	    "Error is expected to be key not found"
	);

	// Check if the value is NULL
	TEST_ASSERT_NULL_MESSAGE(value, "Value is expected to be NULL");

	// Get an existing node from the list with search option
	status = lhttp_list_get(list, "User-Agent", NULL);

	// Check if the status is successful
	TEST_ASSERT_EQUAL_INT_MESSAGE(
	    LHTTP_LIST_OK,
	    status,
	    "Getting a node is expected to be successful"
	);

	// Check if the error is set correctly
	TEST_ASSERT_EQUAL_INT_MESSAGE(
	    LHTTP_LIST_ERROR_NONE,
	    list->error,
	    "Error is expected to be none"
	);

	// Get a non-existing node from the list with search option
	status = lhttp_list_get(list, "Accept", NULL);

	// Check if the status is error
	TEST_ASSERT_EQUAL_INT_MESSAGE(
	    LHTTP_LIST_ERROR,
	    status,
	    "Getting a non-existing node is expected to be an error"
	);

	// Check if the error is set correctly
	TEST_ASSERT_EQUAL_INT_MESSAGE(
	    LHTTP_LIST_ERROR_NONE,
	    list->error,
	    "Error is expected to be key not found"
	);

	TEST_PASS_MESSAGE("GetNode passed");
}

TEST_GROUP_RUNNER(TEST_LIST)
{
	RUN_TEST_CASE(TEST_LIST, InitializeList);
	RUN_TEST_CASE(TEST_LIST, AddNode);
	RUN_TEST_CASE(TEST_LIST, GetNode);
}

static void RunAllTests(void)
{
	// global initialization before all tests goes here
	list = (lhttp_list_t *)malloc(sizeof(lhttp_list_t));

	RUN_TEST_GROUP(TEST_LIST);

	// global clean up after all tests goes here

	// Free the request pointer
	free(list);
	list = NULL;
}

int main(int argc, const char *argv[])
{
	return UnityMain(argc, argv, RunAllTests);
}
