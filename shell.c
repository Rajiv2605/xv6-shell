// Heart of the shell program
#include "types.h"
#include "user.h"
#include "helper.h"

int main(int argc, char **argv)
{   
    char *inputCmd;
    while(1)
    {
        printf(1, "MyShell> ");
        inputCmd = malloc(100*sizeof(char));
        read(0, inputCmd, 100);

        // parse the input command into an array of command structures
        cmd_parser(inputCmd);

        free(inputCmd);
    }
    exit(0);
}