#ifndef INCLUDE_CORE_H
#define INCLUDE_CORE_H

/**
 * execute string as quickrest code
 * @param file_content 
 * @return int 
 */
int qr_str_exec(const char* code);


/**
 * call from the main function with unmodified argc and argv
 * @param argc 
 * @param argv 
 * @return int 
 */
int qr_exec(int argc, char** argv);

#endif