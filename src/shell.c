#include "shell.h"

void shell_loop(char *name, char *dir, char *machine)
{
    char *line;
    char **args;
    int status;

    do
    {
        printf("(%s:%s->%s): ", name, machine, dir);
        line = shell_read();
        args = shell_split_line(line);
        status = shell_execute(args);

        free(line);
        free(args);
    }while(status);
}
char *shell_read(void)
{
    int buffer_size = SHELL_BUFFER_SIZE;
    int pos = 0;
    char *buffer = malloc(sizeof(char) * buffer_size);
    int c;

    if(!buffer)
    {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        c = getchar();

        if(c == EOF || c == '\n')
        {
            buffer[pos] = '\0';
            return buffer;
        }
        else
        {
            buffer[pos] = c;
        }
        if(pos >= buffer_size)
        {
            buffer_size += SHELL_BUFFER_SIZE;
            buffer = realloc(buffer,buffer_size);
            if(!buffer)
            {
                fprintf(stderr, "allocation error\n");
            }
            exit(EXIT_FAILURE);
        }
    }
}
char **shell_split_line(char *line)
{
    int buffer_size = SHELL_BUFFER_SIZE, position = 0;
    char **tokens = malloc(buffer_size * sizeof(char));
    char *token;

    if(!tokens)
    {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
    }
    token = strtok(line, SHELL_TOK_DELIM);
    while(token != NULL)
    {
        tokens[position] = token;
        position++;

        if(position >= buffer_size)
        {
            buffer_size += SHELL_BUFFER_SIZE;
            tokens = realloc(tokens, buffer_size * sizeof(char*));
            if(!tokens)
            {
                fprintf(stderr, "allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, SHELL_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}
int shell_execute(char **args)
{
    pid_t pid, wpid;
    int status;
    pid = fork();
    if(pid == 0)
    {
        if(execvp(args[0], args) == -1)
        {
            perror("shell");
        }
        exit(EXIT_FAILURE);
    }
    else if(pid < 0)
    {
        perror("shell");
    }
    else
    {
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}