#ifndef INCLUDE_LISTENER
#define INCLUDE_LISTENER
#include<infrastructure/datastructures/string_dict.h>
#include<sys/socket.h>

struct ip_address {
    struct sockaddr_storage sa;
    socklen_t sa_len;
};

struct session_args {
    void* args;
    int fd;
    struct {
        const char* string;
        struct ip_address raw;
    } sender;
    StringDict* header_content;
    char* content;
    size_t content_len;
};

void start_listening_socket(const char* portname, void (*endpoint_handler)(struct session_args), void* args);
void http_abort(int fd, int code, const char* header, const char* message);
void http_respond(int fd, int code, const char* header, const char* message);

#endif