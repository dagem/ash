#ifndef __SHELL_HEADER_H__
#define __SHELL_HEADER_H__
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define SHELL_BUFFER_SIZE 1024
#define SHELL_TOK_BUFFER 64
#define SHELL_TOK_DELIM " \t\r\n\a"

typedef struct user
{
    char *logon;
    char *machine;
    char *dir;
}user;

void shell_loop(char *name, char *dir, char *machine);
char *shell_read(void);
char **shell_split_line(char *line);
int shell_execute(char **exec);
#endif /*_SHELL_HEADER__*/