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
		return LHTTP_REQUEST_ERROR;        \
	}

/**
 * @brief Parse the request line of the HTTP request message string
 * 
 * @param request An existing HTTP request object
 * @return 0 on success, -1 on failure
 */
static inline int __lhttp_request_parse_request_line(lhttp_request_t *request);

/**
 * @brief Parse the header section of the HTTP request message string
 * 
 * @param request An existing HTTP request object
 * @return 0 on success, -1 on failure
 */
static inline int __lhttp_request_parse_headers(lhttp_request_t *request);

int lhttp_request_init(lhttp_request_t *request, size_t size)
{
	request->status = LHTTP_REQUEST_UNSET;

	// Allocate memory for the buffer of request message
	request->__buf     = calloc(size, sizeof(char));
	request->__buf_len = size;

	if (request->__buf == NULL)
	{
		request->status = LHTTP_REQUEST_ERROR;
		request->error  = LHTTP_REQUEST_ERROR_MEMORY_ALLOCATION;

		return LHTTP_REQUEST_ERROR;
	}

	request->error = LHTTP_REQUEST_ERROR_NONE;

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

	request->status = LHTTP_REQUEST_PARSING_INITIALIZED;

	return LHTTP_REQUEST_OK;
}

int lhttp_request_parse(lhttp_request_t *request, const char *buf, size_t size)
{
	int s;

	if (request == NULL || buf == NULL)
	{
		return LHTTP_REQUEST_ERROR;
	}

	// Copy the buffer into the request
	memcpy(request->__buf, buf, size);

	s = __lhttp_request_parse_request_line(request);

	if (s != 0)
	{
		return LHTTP_REQUEST_ERROR;
	}

	s = __lhttp_request_parse_headers(request);

	if (s != 0)
	{
		return LHTTP_REQUEST_ERROR;
	}

	return LHTTP_REQUEST_OK;
}

static inline int __lhttp_request_parse_request_line(lhttp_request_t *request)
{
	// Mark the boundaries of the request line
	request->__request_line_start = request->__buf;
	request->__request_line_end   = strstr(request->__buf, "\r\n");

	// Invalid request line
	if (request->__request_line_end == NULL)
	{
		return LHTTP_REQUEST_ERROR;
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
		return;

	if (request->__buf != NULL)
	{
		free(request->__buf);
		request->__buf     = NULL;
		request->__buf_len = 0;
	}
	return;
}

static inline int __lhttp_request_parse_headers(lhttp_request_t *request)
{
	return 0;
}
