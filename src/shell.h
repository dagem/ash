#ifndef __SHELL_HEADER_H__
#define __SHELL_HEADER_H__
#include "headers.h"

/*Main shell commands*/
void shell_loop(char *dir, ...);
char *shell_read(void);
char **shell_split_line(char *line);
int shell_launch(char **exec);
int shell_execute(char **args);

/*Builtin shell commands*/
int shell_cd(char **args);
int shell_exit(char **args);
int shell_help(char **args);

/*Custom shell commands*/
void sigint_handler(int sig);
#endif /*__SHELL_HEADER_H__*/