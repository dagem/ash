#include "shell.h"

user E;
void init()
{
    E.logon = malloc(sizeof(char)*255);
    E.dir = malloc(sizeof(char)*255);
    E.machine = malloc(sizeof(char)*255);
}
void free_user(void)
{
    free(E.logon);
    free(E.dir);
    free(E.machine);
}

int main (int argc, char **argv)
{
    char buf[1024];
    char buf2[1024];
    E.logon = getlogin();
    if(getcwd(buf, sizeof(buf)) != NULL)
    {
        E.dir = strrchr(buf, '/') + 1;
    }
    if(gethostname(buf2, sizeof(buf2)) == 0)
    {
        E.machine = buf2;
    }

    shell_loop(E.dir, E.logon,E.machine);

    return EXIT_SUCCESS;
}
