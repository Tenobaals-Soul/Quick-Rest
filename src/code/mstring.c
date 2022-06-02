#include<mstring.h>
#include<stdlib.h>
#include<memory.h>
#include<stdbool.h>
#include<limits.h>
#include<stdarg.h>

char** strmrealsplit(const char* src, char split_char) {
    int item_count = 2;
    for (int i = 0; src[i]; i++) {
        if (src[i] == split_char) {
            item_count++;
            for (; src[i] && src[i] == split_char; i++);
        }
    }
    char** memspace = malloc(sizeof(char*) * item_count);
    item_count = 0;
    if (*src && *src == split_char) { src++; }
    while (*src) {
        int len = 0;
        const char* start = src;
        while (*src != split_char) {
            src++;
            len++;
        }
        char* split = malloc(len + 1);
        memcpy(split, start, len);
        split[len] = 0;
        memspace[item_count] = split;
        item_count++;
    }
    memspace[item_count] = NULL;
    return memspace;
}

char** strmsplit(const char* src, char split_char) {
    int item_count = 2;
    for (int i = 0; src[i]; i++) {
        if (src[i] == split_char) {
            item_count++;
            for (; src[i] && src[i] == split_char; i++);
        }
    }
    char** memspace = malloc(sizeof(char*) * item_count);
    item_count = 0;
    while (*src) {
        while (*src == split_char) {
            src++;
        }
        if (!*src) break;
        int len = 0;
        const char* start = src;
        while (*src != split_char) {
            src++;
            len++;
        }
        char* split = malloc(len + 1);
        memcpy(split, start, len);
        split[len] = 0;
        memspace[item_count] = split;
        item_count++;
    }
    memspace[item_count] = NULL;
    return memspace;
}

char** sstrmsplit(const char* src, const char* split_string) {
    int skip = strlen(split_string);
    int item_count = 2;
    for (int i = 0; src[i]; i++) {
        if (strbeginswith(src + i, split_string)) {
            i += skip;
            item_count++;
            for (; src[i] && strbeginswith(src + i, split_string); i += skip);
        }
    }
    char** memspace = malloc(sizeof(char*) * item_count);
    item_count = 0;
    while (*src) {
        while (strbeginswith(src, split_string)) {
            src += skip;
        }
        if (!*src) break;
        int len = 0;
        const char* start = src;
        while (!strbeginswith(src, split_string)) {
            src++;
            len++;
        }
        char* split = malloc(len + 1);
        memcpy(split, start, len);
        split[len] = 0;
        memspace[item_count] = split;
        item_count++;
    }
    memspace[item_count] = NULL;
    return memspace;
}

char* strmsub(const char* src, int start, int end) {
    return strmnsub(src, start, end - start);
}

char* strmnsub(const char* src, int start, int len) {
    if (len <= 0) return "";
    char* n_str = malloc(len + 1);
    if (start < 0) start = 0;
    strncpy(n_str, src + start, len);
    return n_str;
}

int strindexof(const char* src, char searched) {
    for (int i = 0; src[i]; i++) {
        if (src[i] == searched) return i;
    }
    return -1;
}

int sstrindexof(const char* src, const char* searched) {
    for (int i = 0; src[i]; i++) {
        if (strbeginswith(src + i, searched)) return i;
    }
    return -1;
}

char* strmstrip(const char* src, char strip_away) {
    int start, end;
    for (start = 0; src[start] && src[start] == strip_away; start++);
    for (end = strlen(src) - 1; end >= 0 && src[end] == strip_away; end--);
    return strmsub(src, start, end);
}

char* strmstripleft(const char* src, char strip_away) {
    int start;
    for (start = 0; src[start] && src[start] == strip_away; start++);
    return strmsub(src, start, INT_MAX);
}
char* strmstripright(const char* src, char strip_away) {
    int end;
    for (end = strlen(src) - 1; end >= 0 && src[end] == strip_away; end--);
    return strmsub(src, 0, end);
}

bool strbeginswith(const char* src, const char* start) {
    for (; *start; src++, start++) {
        if (*src != *start) return false;
    }
    return true;
}

char* strmcat(const char* src1, const char* src2) {
    size_t len1 = strlen(src1);
    size_t len2 = strlen(src2);
    char* new_str = malloc(len1 + len2 + 1);
    strcpy(new_str, src1);
    strcpy(new_str + len1, src2);
    new_str[len1 + len2] = 0;
    return new_str;
}

char* strmformat(const char* format, ...) {
    size_t len = 0;
    bool found_format = false;
    va_list args;

    va_start(args, format);
    size_t needed = vsnprintf(NULL, 0, format, args) + 1;
    va_end(args);

    char  *buffer = malloc(needed);
    
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    return buffer;
}

char* strmappend(char* src1, const char* src2) {
    size_t len1 = src1 ? strlen(src1) : 0;
    size_t len2 = strlen(src2);
    src1 = realloc(src1, len1 + len2 + 1);
    strcpy(src1 + len1, src2);
    return src1;
}