#ifndef __HEADERS_H__
#define __HEADERS_H__
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define SHELL_BUFFER_SIZE 1024
#define SHELL_TOKEN_BUFFER 64
#define SHELL_TOKEN_DELIM " \t\r\n\a"
#define PATH_MAX 2048
// #define PATH_MAX 2048

typedef struct user
{
    char *logon;
    char *machine;
    char dir[PATH_MAX];
}user;


#endif /*__HEADERS_H__*/