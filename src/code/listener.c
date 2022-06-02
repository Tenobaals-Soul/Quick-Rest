#include<listener.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<netdb.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<stdio.h>
#include<string_dict.h>
#include<http.h>
#include<mstring.h>
#include<unistd.h>
#include<sys/mman.h>

#define die(...) printf(__VA_ARGS__); printf("\n"); exit(1);
#define min(x, y) (x > y ? y : x)
#define max(x, y) (x > y ? x : y)

#define READ_STEP_SIZE (1 << 12)

char* read_all(int fd, size_t max_header_size, size_t max_content_size, size_t max_bytes, size_t* size) {
    size_t len = 0;
    char* buffer = malloc(READ_STEP_SIZE);
    while (1) {
        size_t last_read_len = read(fd, buffer + len, READ_STEP_SIZE);
        len += last_read_len;
        if (last_read_len != READ_STEP_SIZE) {
            break;
        }
        else {
            buffer = realloc(buffer, len + READ_STEP_SIZE);
        }
    }
    *size = len;
    return realloc(buffer, len);
}

void handle_session(int fd, void (*endpoint_handler)(struct session_args), struct ip_address addr, void* args) {
    char buffer[INET6_ADDRSTRLEN];
    int err = getnameinfo((struct sockaddr*) &addr.sa, addr.sa_len, buffer, sizeof(buffer), 0, 0, NI_NUMERICHOST);
    if (err != 0) {
        http_abort(fd, 400, "Content-Type: text/plain\r\nContent-Language: en-us\r\n"
            "Content-Length: 15\r\nConnection: close", "Invalid Address");
    }
    size_t request_len;
    char* request = read_all(fd, 1 << 24, 1 << 24, 1 << 24, &request_len);
    endpoint_handler((struct session_args) {args, fd, { buffer, addr }, NULL, request, request_len});
    free(request);
    close(fd);
}

void start_handling(int socket, void (*endpoint_handler)(struct session_args), void* args) {
    for (;;) {
        struct ip_address sender_addr = {
            .sa_len = sizeof(sender_addr.sa)
        };
        int session_fd = accept(socket, (struct sockaddr*) &sender_addr.sa, &sender_addr.sa_len);
        if (session_fd == -1) {
            if (errno == EINTR) continue;
            die("failed to accept connection (errno=%d)", errno);
        }
        pid_t pid = fork();
        if (pid == -1) {
            die("failed to create child process (errno=%d)", errno);
        }
        else if (pid == 0) {
            close(socket);
            handle_session(session_fd, endpoint_handler, sender_addr, args);
            close(session_fd);
            exit(0);
        }
        else {
            close(session_fd);
        }
    }
}

void start_listening_socket(const char* portname, void (*endpoint_handler)(struct session_args), void* args) {
    const char* hostname = 0;
    struct addrinfo hints = {0};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;
    struct addrinfo* res = 0;
    int err = getaddrinfo(hostname, portname, &hints, &res);
    if (err != 0) {
        die("failed to resolve local socket address (err=%d)", err);
    }
    int server_fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    if (server_fd == -1) {
        die("%4d: %s", __LINE__, strerror(errno));
    }
    int reuseaddr = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr)) == -1) {
        die("%4d: %s", __LINE__, strerror(errno));
    }
    if (bind(server_fd,res->ai_addr,res->ai_addrlen) == -1) {
        die("%4d: %s", __LINE__, strerror(errno));
    }
    freeaddrinfo(res);
    if (listen(server_fd, SOMAXCONN)) {
        die("failed to listen for connections (errno=%d)", errno);
    }
    start_handling(server_fd, endpoint_handler, args);
}