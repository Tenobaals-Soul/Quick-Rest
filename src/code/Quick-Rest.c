#include<infrastructure/network/listener.h>
#include<infrastructure/network/http.h>
#include<infrastructure/mstring.h>
#include<unistd.h>
#include<dirent.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<stdlib.h>

#define write_str(fd, str) write(fd, str, strlen(str))

struct env_args {
    char* src_file;
};

void endpoint_handler(struct session_args args) {
    struct env_args* env_args = args.args;

    char* response = "example response";
    StringDict header;
    string_dict_init(&header);
    string_dict_put(&header, "Connection", "close");
    string_dict_put(&header, "Content-Type", "text/plain");
    struct http_time cur_http_time = http_time();
    string_dict_put(&header, "Date", cur_http_time.string);
    http_respond_from_dict(args.fd, 200, &header, response);
    string_dict_destroy(&header);
}

int main(int argc, char** argv) {
    
}