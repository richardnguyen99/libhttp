/* include/lhttp_list.h
 * 
 * Copyright (c) 2024 libhttp. All rights reserved.
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

#ifndef LIBHTTP_LIST_H
#define LIBHTTP_LIST_H 1

#define LIBHTTP_VERSION "0.1.0"
#define LIBHTTP_VERSION_MAJOR 0
#define LIBHTTP_VERSION_MINOR 1
#define LIBHTTP_VERSION_PATCH 0

#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
#include <stdio.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief HTTP list status to indicate the returned status of list operations
 */
typedef enum lhttp_list_status_e
{
	/* List operations go ok */
	LHTTP_LIST_OK,

	/* List operations fail */
	LHTTP_LIST_ERROR
} lhttp_list_status_t;

/**
 * @brief HTTP list state to indicate the current state of the list
 */
typedef enum lhttp_list_state_e
{
	/* The list is not set, and not ready for list operations. */
	LHTTP_LIST_UNSET,

	/* The list is initialized and ready for list operations. */
	LHTTP_LIST_INITIALIZED,

	/* The list is ready to be used in other  */
	LHTTP_LIST_READY,

	/* The list was once ready but it got free'd and not available for list 
	operations. */
	LHTTP_LIST_UNAVAILABLE,
} lhttp_list_state_t;

/**
 * @brief HTTP list error to indicate the error when performing list operations
 * 
 */
typedef enum lhttp_list_error_e
{
	/* None error upon list operations */
	LHTTP_LIST_ERROR_NONE,

	/* The list is not initialized */
	LHTTP_LIST_ERROR_NOT_INITIALIZED,

	/* A memory allocation happened  */
	LHTTP_LIST_ERROR_MEMORY_ALLOCATION,

	/* Key not found in the list */
	LHTTP_LIST_ERROR_KEY_NOT_FOUND,

	/* Value not found in the list */
	LHTTP_LIST_ERROR_VALUE_NOT_FOUND,

	/* Key already exists in this list */
	LHTTP_LIST_ERROR_KEY_EXISTS,

	/* Value already exists in this list */
	LHTTP_LIST_ERROR_VALUE_EXISTS,

	/* Generic and/or unknown error */
	LHTTP_LIST_UNKNOWN_ERROR
} lhttp_list_error_t;

/**
 * @brief Private structure to store key-value string pairs of singly-linked list
 */
struct __lhttp_node_s
{
	/* Key member field is used for searching */
	char *__key;

	/* Value member containing a value for the corresponding key */
	char *__value;

	/* Next node in the list */
	struct __lhttp_node_s *__next;
};

/**
 * @brief Singly-linked list structure to store HTTP lists
 */
typedef struct lhttp_list_s
{
	/* Current state of the linked list after list operations */
	lhttp_list_state_t state;

	/* Error code of the linked list when list operations return errors */
	lhttp_list_error_t error;

	/* An empty node that marks the head of the list (not the first item) */
	struct __lhttp_node_s *__head;

	/* An empty node that marks the tail, or the last item, of the list */
	struct __lhttp_node_s *__tail;

	/* Number of existing nodes in the list */
	size_t __size;

} lhttp_list_t;

// clang-format off

/**
 * @brief Initialize the HTTP list structure
 * 
 * @param list A pointer to the HTTP list structure
 * @return 0 on success. -1 on failure otherwise and the error code is set
 * 
 * @note The caller is responsible for checking the returned value. If the value
 * is -1, the list structure is not initialized correctly, and the caller must
 * check the error code.
 */
lhttp_list_status_t lhttp_list_init(lhttp_list_t *list);

/**
 * @brief Add a key-value pair to the HTTP list structure
 * 
 * @param list A pointer to the HTTP list structure
 * @param key A key string to be added to the list
 * @param value A value string to be added to the list associated with the key
 * @return 0 on success. -1 on failure otherwise and the error code is set
 * 
 * @note On adding, the key and the value will be copied to the list structure
 * via `strdup`. List structure is responsible for the copied values, not for 
 * the original values. The caller is responsible for handling memory allocation
 * and deallocation of the original values.
 */
lhttp_list_status_t lhttp_list_add(lhttp_list_t *list, const char *key, const char *value);

/**
 * @brief Get the value with the `key` from `list` and store it in `value`
 * 
 * @param list A pointer to the HTTP list structure
 * @param key A key string to be searched in the list
 * @param value A pointer to the string pointer to store the value
 * @return 0 on success. -1 on failure otherwise and the error code is set.
 * 
 * @note The `value` is used to store the value associated with the key, so 
 * there is no allocatation. Upon error or not found, the value is set to NULL. 
 * 
 * If the `value` pointer currently points to a memory location, the value will 
 * be invalidated upon successful `lhttp_list_free` call.
 * 
 * If the `value` pointer is passed as `NULL`, the function will instead lookup
 * for `key` and return whether such key exists or not. No error is set upon
 * this operation, regardless of `key` exists or not.
 */
lhttp_list_status_t lhttp_list_get(lhttp_list_t *list, const char *key, char **value);

/**
 * @brief Free allocated memory of the HTTP list structure
 * 
 * @param list A pointer to the HTTP list structure
 * 
 * @note The caller is responsible for freeing the allocated memory, i.e. list 
 * nodes of the list structure. However, the caller is responsible for the 
 * pointer itself if the list structure is allocated via `malloc` or `calloc`.
 * 
 * All pointers that are returned by `lhttp_list_get` will be invalidated after
 * this call.
 */
void lhttp_list_free(lhttp_list_t *list);

// clang-format on

#ifdef __cplusplus
}
#endif

#endif // LIBHTTP_LIST_H
