// // Heart of the shell program
#include "types.h"
#include "user.h"
#include <stddef.h>
#include "helper.c"

int main(int argc, char **argv)
{   
    while(1)
    {
        printf(1, "MyShell> ");
        char inputCmd[100];
        read(0, inputCmd, 100);
        // remove \n from the command
        char cmd[100];
        int i=0;
        while(inputCmd[i]!='\n')
        {
            cmd[i] = inputCmd[i];
            i++;
        }

        if(strcmp("exit", cmd)==0)
            break;

        sc_parser(cmd);
    }
    exit(0);
}