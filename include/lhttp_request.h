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

#ifndef LIBHTTP_REQUEST_H
#define LIBHTTP_REQUEST_H 1

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
 * @brief HTTP request parsing status to indicate the current state of the 
 * request
 * 
 * There are 5 states:
 * 
 * - `UNSET`: The request is not set
 * 
 * - `INITIALIZED`: The request struct is initialized and ready to be parsed
 * 
 * - `ONGOING`: The request is being parsed
 * 
 * - `DONE`: The request is parsed successfully
 * 
 * - `ERROR`: The request is invalid
 */
typedef enum request_status_e
{
	LHTTP_REQUEST_OK,
	LHTTP_REQUEST_UNSET,
	LHTTP_REQUEST_PARSING_INITIALIZED,
	LHTTP_REQUEST_PARSING_ONGOING,
	LHTTP_REQUEST_PARSING_DONE,
	LHTTP_REQUEST_ERROR = -1
} lhttp_request_parsing_status_t;

/**
 * @brief HTTP request parsing error to indicate the error when parsing the
 * request
 * 
 * There are 5 errors:
 * 
 * - `NONE`: No error occurred, i.e. the request status is not `ERROR(4)`
 * 
 * - `METHOD`: Error occurred when parsing the method
 * 
 * - `URI`: Error occurred when parsing the URI string
 * 
 * - `VERSION`: Error occurred when parsing the version string
 * 
 * - `REQUEST_LINE`: Generic error occurred when parsing the request line
 * 
 * - `HEADERS`: Error occurred when parsing the headers
 * 
 * - `BODY`: Error occurred when parsing the body
 * 
 * - `MEMORY_ALLOCATION`: Error occurred when allocating memory for the request
 * 
 * - `UNKNOWN`: Unknown error occurred. This is a generic error or an internal
 * error has occurred.
 */
typedef enum request_error_e
{
	LHTTP_REQUEST_ERROR_NONE              = 0b000000,
	LHTTP_REQUEST_ERROR_METHOD            = 0b000001,
	LHTTP_REQUEST_ERROR_URI               = 0b000010,
	LHTTP_REQUEST_ERROR_VERSION           = 0b000100,
	LHTTP_REQUEST_REQUEST_LINE            = 0b000111,
	LHTTP_REQUEST_ERROR_HEADERS           = 0b001000,
	LHTTP_REQUEST_ERROR_BODY              = 0b010000,
	LHTTP_REQUEST_ERROR_MEMORY_ALLOCATION = 0b100000,
	LHTTP_REQUEST_ERROR_UNKNOWN           = 0b111111,
} lhttp_request_parsing_error_t;

/**
 * @brief HTTP enum for HTTP methods stated by RFC 7231
 */
typedef enum
{
	LHTTP_METHOD_GET,
	LHTTP_METHOD_HEAD,
	LHTTP_METHOD_POST,
	LHTTP_METHOD_PUT,
	LHTTP_METHOD_DELETE,
	LHTTP_METHOD_CONNECT,
	LHTTP_METHOD_OPTIONS,
	LHTTP_METHOD_TRACE,
	LHTTP_METHOD_PATCH,
	LHTTP_METHOD_INVALID
} lhttp_method_t;

typedef enum
{
	LHTTP_VERSION_1_0,
	LHTTP_VERSION_1_1,
	LHTTP_VERSION_INVALID
} lhttp_version_t;

struct lhttp_request_s
{
	/* Public fields for HTTP request */

	lhttp_method_t method;                 // HTTP method
	lhttp_version_t version;               // HTTP version
	char *uri;                             // URI
	lhttp_request_parsing_status_t status; // Status of the request
	lhttp_request_parsing_error_t error;   // Error when request status is ERROR

	/* Private fields for parsing HTTP requests. Used by method impls. */

	char *__buf;
	size_t __buf_len;

	char *__request_line_start; // start of the request line
	char *__request_line_end;   // end of the request line
	char *__method_start;       // start of the method string
	char *__method_end;         // end of the method string
	char *__uri_start;          // start of the URI string
	char *__uri_end;            // end of the URI string
	char *__version_start;      // start of the version string
	char *__version_end;        // end of the version string
	char *__headers_start;      // start of the header section
	char *__headers_end;        // end of the header section
	char *__body_start;         // start of the body
	char *__body_end;           // end of the body
};

/**
 * @brief HTTP requet structure for reusability
 */
typedef struct lhttp_request_s lhttp_request_t;

/**
 * @brief Initialize a `lhttp_request_t` structure with maximum buffer 
 * size `bufsz`
 * 
 * @param req A pointer to a `lhttp_request_t` structure
 * @param bufsz Maximum buffer size (amount of bytes)
 * @return int 0 on success, -1 on failure
 * 
 * @note The caller is responsible for checking the returned value. If the 
 * returned value is -1, 
 * 
 * The returned lhttp_request_t structure must be freed by the caller.
 */
int lhttp_request_init(lhttp_request_t *req, const size_t bufsz);

/**
 * @brief Parse raw HTTP request `data` with length `len` into `lhttp_request_t *` structure
 * 
 * @param req A pointer to a `lhttp_request_t` structure
 * @param data Immutable raw HTTP request data
 * @param len Length of the raw HTTP request data
 * @return 0 on success, -1 on failure (e.g. invalid/bad request)
 * 
 * @note The caller is responsible for checking the returned value. If an error
 * occurs, it means that the request is invalid and the caller should handle it.
 */
int lhttp_request_parse(lhttp_request_t *req, const char *data, size_t len);

/**
 * @brief Validate HTTP request structure based on RFC 7230
 * 
 * @param req A pointer to a `lhttp_request_t` structure
 * @param http_status HTTP status code
 * @return int 0 on success, -1 on failure.
 * 
 * @note The caller is responsible for checking the returned value. If an error
 * occurs, it means there is an internal error happening. If the returned value
 * is 0, it could mean that the request is valid or invalid. The caller should
 * check the `http_status` value afterwards.
 */
int lhttp_request_validate(lhttp_request_t *req, int *http_status);

/**
 * @brief Free the `lhttp_request_t` structure
 * 
 * @param req A pointer to a `lhttp_request_t` structure
 * 
 * @note The caller is only responsible for deallocating the allocated memory
 * in the request struct members. If the request struct is an allocated pointer,
 * for example via `malloc` or `calloc`, the caller should be responsible for 
 * freeing the pointer itself.
 */
void lhttp_request_free(lhttp_request_t *req);

#ifdef __cplusplus
}
#endif

#endif // LIBHTTP_REQUEST_H
