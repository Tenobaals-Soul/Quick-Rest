#ifndef INCLUDE_MSTRING_H
#define INCLUDE_MSTRING_H
#include<stdbool.h>
#include<stdio.h>
#include<string.h>

/**
 * Splits src in multiple strings on the specified seperating char.
 * For example strmsplit("a|b|cd||e", '|') will return:
 * (char**) {"a", "b", "cd", "", "e", NULL}
 * The necessary memory will be allocated.
 * @param src This string will be splited.
 * @param split_char this character will be removed and
 * the string will be splitted upon finding one of them.
 * @return char** a null terminated string vector filled with results
 */
char** strmrealsplit(const char* src, char split_char);

/**
 * Splits src in multiple strings on the specified seperating char.
 * For example strmsplit("a b  cd   e", ' ') will return:
 * (char**) {"a", "b", "cd", "e", NULL}
 * The necessary memory will be allocated.
 * @param src This string will be splited.
 * @param split_char this character will be removed and
 * the string will be splitted upon finding one of them.
 * @return char** a null terminated string vector filled with results
 */
char** strmsplit(const char* src, char split_char);

/**
 * Splits src in multiple strings on the specified seperating char.
 * For example sstrmsplit("a, b,  cd, e,f", ", ") will return:
 * (char**) {"a", "b", " cd", "e,f", NULL}
 * The necessary memory will be allocated.
 * @param src This string will be splited.
 * @param split_string this strings will be removed and
 * the string will be splitted upon finding one of them.
 * @return char** a null terminated string vector filled with results
 */
char** sstrmsplit(const char* src, const char* split_string);

/**
 * Creates a new string starting at start (inclusive) and ending at end (exclusive).
 * For example strmsub("hello world", 4, 9) will return:
 * (char*) "o wor"
 * 
 * The necessary memory will be allocated.
 * @param src source string
 * @param start the start char (inclusive) values in front of the string
 * (negative values) will be handled as 0. Values after the string will make the
 * function return "".
 * @param end the end of the string. values smaller than start will make the function
 * return "". Values greater than the string lenght will behandled as strlen(src).
 * @return char* 
 */
char* strmsub(const char* src, int start, int end);

/**
 * Creates a new string starting at start (inclusive) with the maximum length of len.
 * For example strmnsub("hello world", 4, 5) will return:
 * (char*) "o wor"
 * 
 * @param src source string
 * @param start the start char (inclusive) values in front of the string
 * (negative values) will be handled as 0. Values after the string will make the
 * function return "".
 * @param len the maximum length of the string. values smaller or equal to 0 will
 * make the function return "".
 * @return char* 
 */
char* strmnsub(const char* src, int start, int len);

/**
 * Returnes the index first occurrence of searched in src.
 * For example strindexof("hello world", 'o') will return:
 * (int) 4
 * 
 * @param src the source string
 * @param searched the character to search.
 * @return int the index of searched in src. If searched could not be found
 * this function will return -1
 */
int strindexof(const char* src, char searched);
/**
 * Returnes the index first occurrence of searched in src.
 * For example strindexof("hello world", "world") will return:
 * (int) 6
 * 
 * The necessary memory will be allocated.
 * @param src the source string
 * @param searched the string to search.
 * @return int the index where searched starts in src. If searched could not be found
 * this function will return -1
 */
int sstrindexof(const char* src, const char* searched);

/**
 * Removes all occurences of strip_away at the start and the end of src.
 * 
 * The necessary memory will be allocated.
 * Neccesarry memory will me allocated.
 * @param src the source string
 * @param strip_away the character to remove
 * @return char* the string with the removed characters
 */
char* strmstrip(const char* src, char strip_away);

/**
 * Removes all occurences of strip_away at the start of src.
 * 
 * The necessary memory will be allocated.
 * Neccesarry memory will me allocated.
 * @param src the source string
 * @param strip_away the character to remove
 * @return char* the string with the removed characters
 */
char* strmstripleft(const char* src, char strip_away);/**

 * Removes all occurences of strip_away at the end of src.
 * 
 * The necessary memory will be allocated.
 * Neccesarry memory will me allocated.
 * @param src the source string
 * @param strip_away the character to remove
 * @return char* the string with the removed characters
 */
char* strmstripright(const char* src, char strip_away);

/**
 * Returns true in case src starts with start.
 * For example strbeginswith("Hello world", "Hello") will return true.
 * strbeginswith("Hello world", "world") on the other hand, will return false.
 * 
 * @param src the string to check
 * @param start the string the src has to begin with
 * @return true if src begins with start
 */
bool strbeginswith(const char* src, const char* start);

/**
 * Concatinates two strings and returnes newly allocated memory.
 * 
 * @param src1 
 * @param src2 
 * @return char* src1 + src2
 */
char* strmcat(const char* src1, const char* src2);

/**
 * Formats a string like sprintf but allocates the neccesary memory.
 * 
 * @param format the format string (like printf)
 * @param ... the format arguments
 * @return char* 
 */
char* strmformat(const char* format, ...) __attribute_format_arg__(1);

/**
 * Appends src2 to src1 and allocates the memory neccessary by reallocating src1.
 * Do NOT use src1 after and do not free it. Use the returned value instead.
 * 
 * @param src1 the source string. This string may be NULL.
 * @param src2 the string to append
 * @return char* src1 + src2
 */
char* strmappend(char* src1, const char* src2);

#endif