// // Heart of the shell program
#include "types.h"
#include "user.h"
#include <stddef.h>
#include "fcntl.h"
#include "helper.c"

int main(int argc, char **argv)
{   
    while(1)
    {
        printf(1, "MyShell> ");
        char *inputCmd = malloc(sizeof(char)*100);
        read(0, inputCmd, 100);
        if(inputCmd[0]=='\n')
        {
            free(inputCmd);
            continue;
        }
        // remove \n from the command
        char *cmd = malloc(sizeof(char)*100);
        int i=0;
        while(inputCmd[i]!='\n')
        {
            cmd[i] = inputCmd[i];
            i++;
        }
        cmd[i] = '\0';
            
        // printf(1, "shell command is: %s\n", cmd);
        if(strcmp("exit", cmd)==0)
            break;

        // int fd = open(cmd, 0);
        sc_parser(cmd);

        free(inputCmd);
        free(cmd);
    }
    exit(0);
}