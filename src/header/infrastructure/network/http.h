#ifndef INCLUDE_HTTP
#define INCLUDE_HTTP
#include<time.h>
#include<infrastructure/datastructures/string_dict.h>

const char* http_code_message(int code);

struct http_time {
    time_t rawtime;
    struct tm* timeinfo;
    char string[40];
};

struct http_time http_time();
void http_respond(int fd, int code, const char* header, const char* message);
void http_abort(int fd, int code, const char* header, const char* message);
void http_abort_from_dict(int fd, int code, StringDict* header, const char* message);
void http_respond_from_dict(int fd, int code, StringDict* header, const char* message);

#endif