# libhttp

Small, light-weight C library for parsing and constructing HTTP requests and
responses.

Offcial documentation: [here]()

> [!WARNING]  
> This is not a web server. This is simply a library that takes a HTTP request/
> response message and parses it into a struct, or takes a struct and serializes
> it into a HTTP message. If you are looking for a web server that can be
> written in C or C++, please check out
> [libhttpd](https://github.com/lammertb/libhttp) or
> [libuv](https://github.com/libuv/libuv).

## Project mission

The goal of this project is to provide a simple, easy-to-use library for
parsing and constructing HTTP requests and responses. The library is designed to
be well intergrated into existing projects such as web servers, proxies, or
other network applications that require HTTP support.

Instead of worrying about parsing HTTP messages strings into something usable,
developers can use this library to do such tasks and focus on the actual logic
of their web servers.

## Manual pages

- [API Reference]() &mdash; The official API reference for the library.
- [Installation]() &mdash; Install Guide for the library.
- [Examples]() &mdash; Examples of how to use the library.
- [Contributing]() &mdash; How to contribute to the project.

## Features

- [ ] Parse HTTP request message string into a reusable struct
- [ ] Serialize HTTP request struct into a string
- [ ] Construct a HTTP request struct from scratch
- [ ] Parse HTTP response message string into a reusable struct
- [ ] Support random access to common headers
- [ ] Support for chunked transfer encoding
- [ ] Support Cookies
- [ ] Support JSON application type
- [ ] Support for HTTP/1.1
- [ ] Support template rendering

### Basic

The basic usage of the library is to parse a HTTP request/response message into a struct, and then serialize the struct back into a HTTP message.

```c
#include <stdio.h>
#include <string.h>
#include <libhttp/http.h>

int main(int argc, const char* argv[])
{
    // The message can be a request 
    const char* message = "GET / HTTP/1.1\r\n"
                          "Host: localhost:8080\r\n"
                          "User-Agent: curl/7.68.0\r\n"
                          "Accept: */*\r\n"
                          "\r\n";

    http_request_t *req = http_request_parse(message, strlen(message), HTTP_HDR_LIST);
    if (req == NULL) {
        fprintf(stderr, "Failed to parse HTTP request\n");
        return 1;
    }

    printf("Method: %s\n", req->method);
    printf("URI: %s\n", req->uri);
    printf("Version: %s\n", req->version);

    char *serialized = req.
}
```

## License

This project is licensed under the MIT License - see the [LICENSE](https://github.com/richardnguyen99/libhttp/blob/main/LICENSE) file for details.
