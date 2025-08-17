#ifndef ERROR_H
#define ERROR_H

#include <errno.h>

/*
typedef enum {
    
} err_type;
*/

void err_fatal(const char *format, ...);
void err_warn (const char *format, ...);

#endif