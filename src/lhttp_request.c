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

#define CHECK_VALID_STRING(start, end)     \
	if (end == NULL || (end - start) <= 0) \
	{                                      \
		return -1;                         \
	}

/**
 * @brief Parse the request line of the HTTP request message string
 * 
 * @param request An existing HTTP request object
 */
static inline int __lhttp_request_parse_request_line(lhttp_request_t *request);

lhttp_request_t *lhttp_request_new(size_t size)
{
	lhttp_request_t *request = malloc(sizeof(lhttp_request_t));
	if (request == NULL)
	{
		return NULL;
	}

	request->__buf     = calloc(size, sizeof(char));
	request->__buf_len = size;

	if (request->__buf == NULL)
	{
		free(request);
		return NULL;
	}

	request->__request_line_start = NULL;
	request->__request_line_end   = NULL;
	request->__method_start       = NULL;
	request->__method_end         = NULL;
	request->__uri_start          = NULL;
	request->__uri_end            = NULL;
	request->__version_start      = NULL;
	request->__version_end        = NULL;
	request->__headers_start      = NULL;
	request->__headers_end        = NULL;
	request->__body_start         = NULL;
	request->__body_end           = NULL;

	return request;
}

int lhttp_request_parse(lhttp_request_t *request, const char *buf, size_t size)
{
	int s;

	if (request == NULL || buf == NULL)
	{
		return -1;
	}

	// Copy the buffer into the request
	memcpy(request->__buf, buf, size);

	s = __lhttp_request_parse_request_line(request);

	if (s != 0)
	{
		return -1;
	}

	return 0;
}

static inline int __lhttp_request_parse_request_line(lhttp_request_t *request)
{
	// Mark the boundaries of the request line
	request->__request_line_start = request->__buf;
	request->__request_line_end   = strstr(request->__buf, "\r\n");

	// Invalid request line
	if (request->__request_line_end == NULL)
	{
		return -1;
	}

	// Mark the boundaries of the method
	request->__method_start = request->__request_line_start;
	request->__method_end   = strstr(request->__method_start, " ");

	// Check if the length of the method is valid
	CHECK_VALID_STRING(request->__method_start, request->__method_end);

	// Allow loose spacing between method and URI
	request->__uri_start = request->__method_end + 1;
	for (; *request->__uri_start == ' '; request->__uri_start++)
		;

	// Mark the boundaries of the URI
	request->__uri_end = strstr(request->__uri_start, " ");

	// Check if the length of the URI is valid
	CHECK_VALID_STRING(request->__uri_start, request->__uri_end);

	// Allow loose spacing between URI and version
	request->__version_start = request->__uri_end + 1;
	for (; *request->__version_start == ' '; request->__version_start++)
		;

	// Mark the boundaries of the version
	request->__version_end = strstr(request->__version_start, "\r\n");

	// Check if the length of the version is valid
	CHECK_VALID_STRING(request->__version_start, request->__version_end);

	return 0;
}

void lhttp_request_free(lhttp_request_t *request)
{
	if (request == NULL)
	{
		return;
	}

	if (request->__buf != NULL)
	{
		free(request->__buf);
	}

	free(request);
}
