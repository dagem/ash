#include "shell.h"
#include <signal.h>
#include <string.h>
#include <unistd.h>

user B;
char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};
int (*builtin_func[]) (char **) =   //function pointer 
{
    &shell_cd,
    &shell_help,
    &shell_exit
};
int shell_number_of_builtins()
{
    return sizeof(builtin_str) / sizeof(char*);
}
void shell_loop(char *dir, ...)
{
    
    char *line;
    char **args;
    int status;
    char buf[1024];
    char buf2[1024];
    B.logon = getlogin();

    if(getcwd(buf, sizeof(buf)) != NULL)
    {
        strcpy(B.dir, strrchr(buf, '/') + 1);
    }
    if(gethostname(buf2, sizeof(buf2)) == 0)
    {
        B.machine = buf2;
    }
    do
    {
        printf("(%s:%s->%s): ", B.logon, B.machine, dir);
        line = shell_read();
        args = shell_split_line(line);
        status = shell_execute(args);
        
        sigint_handler(status);

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

        if(c == EOF)
        {
            exit(EXIT_SUCCESS);
        }
        else if(c == '\n')
        {
            buffer[pos] = '\0';
            return buffer;
        }
        else
        {
            buffer[pos] = c;
        }
        pos++;
        if(pos >= buffer_size)
        {
            buffer_size += SHELL_BUFFER_SIZE;
            buffer = realloc(buffer,buffer_size);
            if(!buffer)
            {
                fprintf(stderr, "allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}
char **shell_split_line(char *line)
{
    int buffer_size = SHELL_TOKEN_BUFFER, position = 0;
    char **tokens = malloc(buffer_size * sizeof(char));
    char *token, **backup_tokens;

    if(!tokens)
    {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
    }
    token = strtok(line, SHELL_TOKEN_DELIM);
    while(token != NULL)
    {
        tokens[position] = token;
        position++;

        if(position >= buffer_size)
        {
            buffer_size += SHELL_TOKEN_BUFFER;
            backup_tokens = tokens;
            tokens = realloc(tokens, buffer_size * sizeof(char*));
            if(!tokens)
            {
                free(backup_tokens);
                fprintf(stderr, "allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, SHELL_TOKEN_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}
int shell_launch(char **args)
{
    pid_t pid;
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
          waitpid(pid, &status, WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}
int shell_cd(char **args)
{
    struct stat exists;
    char *tmp = malloc(sizeof(char) * 255);
    if(args[1] != NULL && stat(args[1], &exists) == 0 && S_ISDIR(exists.st_mode))
    {  
      chdir(args[1]);
      if(strcmp(args[1], "..") == 0)
      {
        char buf[255];
        if(getcwd(buf, sizeof(buf)) != NULL)
        {
            strcpy(B.dir, buf);
        }
        strcpy(tmp, strrchr(B.dir, '/')+1);
        shell_loop(tmp);
      }
      else
      {
        strcpy(tmp, args[1]);
      }
        shell_loop(tmp);    
      free(tmp);
    }
    else if(args[1] == NULL || strcmp(args[1], "~") == 0)
    {
        
        strcpy(tmp, "/home/");
        
        strcat(tmp,getlogin());
        chdir(tmp);
        shell_loop("~");
        
        free(tmp);
    }
    else if(chdir(args[1]) != 0)
    {
        perror(args[1]);
    }
    else
    {
        chdir(args[1]);  
    }
    return 1;
}
int shell_help(char **args)
{
    return 0;
}
int shell_exit(char **args)
{
    if(strcmp(args[1], "exit") == 0)
    {
        raise(SIGTERM);
        return 1;
    }
    return 0;
}
int shell_execute(char **args)
{
    if(args[0] == NULL)
    {
        return 1;
    }
    int i;
    for(i = 0; i < shell_number_of_builtins(); i++)
    {
        if(strcmp(args[0], builtin_str[i]) == 0)
        {
            return (*builtin_func[i])(args);
        }
    }
    return shell_launch(args);
}
void sigint_handler(int sig) /* SIGINT handler */
{
	if(sig == SIGINT){
        printf("use command exit");
	}
}
