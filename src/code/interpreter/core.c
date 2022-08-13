#include<infrastructure/datastructures/string_dict.h>
#include<infrastructure/datastructures/stack.h>
#include<interpreter/core.h>
#include<interpreter/lex.h>
#include<stdlib.h>
#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>

struct config {
    char* self;
    char* file;
};

static const char argument_help[] = \
"usage: quickrest file [options]\n"
"options:";

void print_argument_help_and_exit() {
    puts(argument_help);
    exit(0);
}

void advance_args(int* argc, char*** argv) {
    if (*argc == 0) {
        print_argument_help_and_exit();
    }
    else {
        --*argc;
        ++*argv;
    }
}

struct config config_parse(int argc, char** argv) {
    struct config config = {0};
    config.self = argv[0];
    advance_args(&argc, &argv);
    config.file = argv[0];
    advance_args(&argc, &argv);
    for (int i = 0; i < argc; i++) {
        print_argument_help_and_exit();
        exit(0);
    }
    return config;
}

int qr_str_exec(const char* file_content) {
    struct lex_state lex_state = {0};
    while (file_content[lex_state.cursor_pos]) {
        Token** tokens = lex_next_line(file_content, &lex_state);
        if (tokens == NULL) continue;
        free(tokens);
    }
}

char* fullread(char* filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        printf("could not open %s\n", filename);
        exit(0);
    }
    ssize_t size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    char* filecontent = malloc(size + 1);
    if (read(fd, filecontent, size) != size) {
        printf("failed to read %s\n", filename);
        exit(1);
    }
    return filecontent;
}

/**
 * call from the main function with unmodified argc and argv
 * @param argc 
 * @param argv 
 * @return int 
 */
int qr_exec(int argc, char** argv) {
    struct config config = config_parse(argc, argv);
    char* file_content = fullread(config.file);
    int return_code = qr_str_exec(file_content);
    free(file_content);
    return return_code;
}