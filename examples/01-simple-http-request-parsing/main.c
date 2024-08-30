#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lhttp_request.h"

#define BUFSIZ 8192

int main(void)
{
	char *request = "GET / HTTP/1.1\r\n"
	                "Host: localhost:8080\r\n"
	                "User-Agent: curl/7.68.0\r\n"
	                "Accept: */*\r\n"
	                "\r\n";

	lhttp_request_t *req = lhttp_request_new(BUFSIZ);
	if (!req)
	{
		fprintf(stderr, "Failed to allocate memory for request\n");
		return 1;
	}

	int ret = lhttp_request_parse(req, request, strlen(request));

	if (ret != 0)
	{
		fprintf(stderr, "Failed to parse request\n");
		return 1;
	}

	return 0;
}
