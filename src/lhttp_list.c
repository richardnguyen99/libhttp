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

#define MEMCHECK_ALLOC_STRING(value)                      \
	if (value == NULL)                                    \
	{                                                     \
		list->error = LHTTP_LIST_ERROR_MEMORY_ALLOCATION; \
		return LHTTP_LIST_ERROR;                          \
	}

#define LHTTP_LIST_FOREACH(list, key, code_block)          \
	struct __lhttp_node_s *current = list->__head->__next; \
	for (; current != NULL; current = current->__next)     \
	{                                                      \
		if (strcmp(current->__key, key) == 0)              \
		{                                                  \
			code_block                                     \
		}                                                  \
	}

static inline struct __lhttp_node_s *__lhttp_node_create()
{
	struct __lhttp_node_s *node = calloc(1, sizeof(struct __lhttp_node_s));
	if (node == NULL)
	{
		return NULL;
	}

	node->__key   = NULL;
	node->__value = NULL;

	return node;
}

static inline void __lhttp_node_free(struct __lhttp_node_s *node)
{
	if (node->__key != NULL)
	{
		free(node->__key);
		node->__key = NULL;
	}

	if (node->__value != NULL)
	{
		free(node->__value);
		node->__value = NULL;
	}

	free(node);
}

lhttp_list_status_t lhttp_list_init(lhttp_list_t *list)
{
	list->state = LHTTP_LIST_UNSET;
	list->error = LHTTP_LIST_ERROR_NONE;

	list->__head = __lhttp_node_create();
	MEMCHECK_ALLOC_STRING(list->__head);

	list->__tail = list->__head;
	list->__size = 0;

	list->state = LHTTP_LIST_INITIALIZED;

	return LHTTP_LIST_OK;
}

lhttp_list_status_t
lhttp_list_add(lhttp_list_t *list, const char *key, const char *value)
{
	if (list->state != LHTTP_LIST_INITIALIZED)
	{
		list->error = LHTTP_LIST_ERROR_NOT_INITIALIZED;
		return LHTTP_LIST_ERROR;
	}

	struct __lhttp_node_s *node = __lhttp_node_create();
	MEMCHECK_ALLOC_STRING(node);

	LHTTP_LIST_FOREACH(list, key, {
		list->error = LHTTP_LIST_ERROR_KEY_EXISTS;
		return LHTTP_LIST_ERROR;
	});

	node->__key = strdup(key);
	MEMCHECK_ALLOC_STRING(node->__key);

	node->__value = strdup(value);
	MEMCHECK_ALLOC_STRING(node->__value);

	// Append the node to the list
	list->__tail->__next = node;
	list->__tail         = list->__tail->__next;
	list->__size++;

	return LHTTP_LIST_OK;
}

lhttp_list_status_t
lhttp_list_get(lhttp_list_t *list, const char *key, char **value)
{
	if (list->state != LHTTP_LIST_INITIALIZED)
	{
		list->error = LHTTP_LIST_ERROR_NOT_INITIALIZED;
		return LHTTP_LIST_ERROR;
	}

	LHTTP_LIST_FOREACH(list, key, {
		if (value != NULL)
			*value = current->__value;

		list->error = LHTTP_LIST_ERROR_NONE;
		return LHTTP_LIST_OK;
	});

	if (value != NULL)
	{
		*value      = NULL;
		list->error = LHTTP_LIST_ERROR_KEY_NOT_FOUND;
	}
	else
	{
		list->error = LHTTP_LIST_ERROR_NONE;
	}

	return LHTTP_LIST_ERROR;
}

void lhttp_list_free(lhttp_list_t *list)
{
	if (list->state == LHTTP_LIST_INITIALIZED)
	{
		struct __lhttp_node_s *node = list->__head->__next;
		struct __lhttp_node_s *next = NULL;

		while (node != NULL)
		{
			next = node->__next;

			__lhttp_node_free(node);
			node = next;
		}

		free(list->__head);
		list->__head = NULL;
		list->__tail = NULL;
		list->__size = 0;
	}

	list->state = LHTTP_LIST_UNSET;
}
