#include<infrastructure/datastructures/stack.h>
#include<interpreter/lex.h>
#include<stdlib.h>
#include<ctype.h>
#include<memory.h>

#define EXPECTED_TAB_SIZE 4

static unsigned int lex_indentation_token(const char* code,
                                   struct lex_state* lex_state,
                                   Stack* tokens) {
    unsigned int len = 0;
    for (;; lex_state->cursor_pos++) {
        if (code[lex_state->cursor_pos] == ' ') {
            len++;
        }
        else if (code[lex_state->cursor_pos] == '\t') {
            len += 4;
        }
        else {
            break;
        }
    }
    Token* token = malloc(sizeof(Token));
    token->type = TOKEN_WORD;
    token->len = len / 4;
    push_ptr(tokens, token);
    return len % 4;
}

static int lex_number(const char* code, struct lex_state* lex_state, Stack* tokens) {
    char c = code[lex_state->cursor_pos];
    if (c < '0' || c > '9') return 0;
    unsigned int len = 0;
    while (isalnum(code[lex_state->cursor_pos + len])) {
        len++;
    }
    Token* token = malloc(sizeof(Token));
    token->type = TOKEN_NUMBER;
    token->copy = malloc(len + 1);
    memcpy(token->copy, code + lex_state->cursor_pos, len);
    token->copy[len] = 0;
    push_ptr(tokens, token);
    lex_state->cursor_pos += len;
    return 1;
}

static int lex_string(const char* code, struct lex_state* lex_state, Stack* tokens) {
    char c = code[lex_state->cursor_pos];
    if (c != '\"' && c != '\'') return 0;
    lex_state->cursor_pos++;
    unsigned int len = 0;
    while (code[lex_state->cursor_pos + len] != c && code[lex_state->cursor_pos + len] != '\n') {
        len++;
    }
    Token* token = malloc(sizeof(Token));
    token->type = TOKEN_STRING;
    token->copy = malloc(len + 1);
    memcpy(token->copy, code + lex_state->cursor_pos, len);
    token->copy[len] = 0;
    push_ptr(tokens, token);
    if (code[lex_state->cursor_pos + len] == c) lex_state->cursor_pos++;
    lex_state->cursor_pos += len;
    return 1;
}

static void lex_word(const char* code, struct lex_state* lex_state, Stack* tokens) {
    lex_state->cursor_pos++;
    unsigned int len = 0;
    enum {ALNUM, PUNCT, OTHER} type;
    if (isalnum(code[lex_state->cursor_pos])) type = ALNUM;
    else if (ispunct(code[lex_state->cursor_pos])) type = PUNCT;
    else type = OTHER;
    while (!isspace(code[lex_state->cursor_pos]) && !(code[lex_state->cursor_pos] != '\n')) {
        if (type == ALNUM && !isalnum(code[lex_state->cursor_pos])) break;
        else if (type == PUNCT && !ispunct(code[lex_state->cursor_pos])) break;
        else if (isalnum(code[lex_state->cursor_pos]) || ispunct(code[lex_state->cursor_pos])) break;
        len++;
    }
    Token* token = malloc(sizeof(Token));
    token->type = TOKEN_STRING;
    token->copy = malloc(len + 1);
    memcpy(token->copy, code + lex_state->cursor_pos, len);
    token->copy[len] = 0;
    push_ptr(tokens, token);
    lex_state->cursor_pos += len;
}

Token** lex_next_line(const char* code, struct lex_state* lex_state) {
    Stack tokens;
    init_stack(&tokens);
    if (lex_indentation_token(code, lex_state, &tokens)) {
        Token* to_free;
        while ((to_free = pop_ptr(&tokens))) {
            if (peek_ptr(&tokens)) free(to_free->copy);
            free(to_free);
        }
        destroy_stack(&tokens);
        return NULL;
    }
    while (code[lex_state->cursor_pos] && code[lex_state->cursor_pos] != '\n') {
        while (isspace(code[lex_state->cursor_pos])) lex_state->cursor_pos++;
        if (lex_number(code, lex_state, &tokens)) continue;
        if (lex_string(code, lex_state, &tokens)) continue;
        lex_word(code, lex_state, &tokens);
    }
    push_ptr(&tokens, NULL);
    lex_state->cursor_pos++;
    return stack_disown(&tokens);
}