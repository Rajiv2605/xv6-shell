// Custom test case for getprocesstimedetails() system call
#include "types.h"
#include "user.h"

int main(int argc, char *argv[])
{
    char *inpt = malloc(10*sizeof(char));
    read(0, inpt, 10);
    getprocesstimedetails();
    free(inpt);
    exit(0);
}