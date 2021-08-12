#include "helper.h"

void cmd_parser(char *inputCmd)
{
    // setup the command table.
    char n_cmds = 0;
    char i=0;
    while(inputCmd[i]!='\n')
    {
        if((inputCmd[i]=='|' && inputCmd[i+1]!='|') || inputCmd[i]==';')
            n_cmds++;
        else if((inputCmd[i]=='|' && inputCmd[i+1]=='|') || (inputCmd[i]=='&' && inputCmd[i+1]=='&'))
        {
            i++;
            n_cmds++;
        }

        i++;
    }
    cmdTable = malloc(n_cmds*sizeof(struct cmd));

    // initialize the table with default values
    for(struct cmd* p=cmdTable; p!=NULL; p++)
    {
        p->cmd = "";
        p->redir = 0;
        p->isPipe = 0;
        p->status = 0;
        p->seqno = 0;
        p->isAndOr = 0;
    }

    char *cmdClean = strtok(inputCmd, "\n");
    struct cmd* cmd_ptr = cmdTable;
    char *seg = strtok(cmdClean, " ");
    char idx = 0;
    char seq = 0;
    do
    {
        if(idx==0)
        {
            cmd_ptr->cmd = seg;
            cmd_ptr->seqno = seq;
        }
        else
        {
            if(seg[0]=='>')
                cmd_ptr->redir = 1;
            else if(seg[0]=='<')
                cmd_ptr->redir = -1;

            if(seg[0]=='|' && strlen(seg)==1)
                cmd_ptr->isPipe = 1;

            if(seg[0]=='|' && strlen(seg)==2)
                cmd_ptr->isAndOr = -1;
            if(seg[0]=='&' && strlen(seg)==2)
                cmd_ptr->isAndOr = 1;
        }
            cmd_ptr->args[idx-1] = seg;
        
        idx++;

        if(seg[0]=="|" || seg[0]=="&" || seg[0]==";")
        {
            if(seg[0]==';')
                seq++;
            cmd_ptr++;
            idx = 0;
        }
    } while(seg != NULL);
}

void free_cmdtable(void)
{
    free(cmdTable);
}

void cmd_run(char **args)
{
    int cpid = fork();
    if(cpid > 0) // parent
        wait(NULL);
    else
    {
        /* 
            check for special characters: ; | > < && ||
            parse commands
        */
    }
}