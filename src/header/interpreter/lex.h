#ifndef INCLUDE_LEX_H
#define INCLUDE_LEX_H

struct lex_state {
    unsigned int cursor_pos;
};

typedef enum TokenType {
    TOKEN_NUMBER, TOKEN_WORD, TOKEN_STRING, TOKEN_INLINE_SQL, TOKEN_INLINE_HTML
} TokenType;

typedef struct Token {
    TokenType type;
    union {
        char* copy;
        long len;
    };
} Token;

Token** lex_next_line(const char* code, struct lex_state* lex_state);

#endif