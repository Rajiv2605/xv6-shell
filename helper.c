void parse_any(char *inputCmd, char **cmds, char pp)
{
    // add safety checks here

    char *idx;
    int i=0;
    char *idx_hist = NULL;
    while(1)
    {
        if((idx=strchr(inputCmd, pp))==NULL)
        {
            idx_hist += 2; // reaching the start of the last command
            int j=0;
            cmds[i] = malloc(10*sizeof(char));
            while(*idx_hist != '\0')
                cmds[i][j++] = *(idx_hist++);

            cmds[i][j] = '\0';

            break;
        }
        idx_hist = idx;
        cmds[i] = malloc(50*sizeof(char));
        idx--; // accounting for space before pipe
        char *p = inputCmd;
        int j=0;
        while(p!=idx)
        {
            cmds[i][j++] = *p;
            p++;
        }
        cmds[i][j] = '\0';
        inputCmd = idx+3; // reaching the start of the next command
        i++;
    }
}

void parse_sucexec(char *inputCmd, char **cmds, char pp)
{
    // add safety checks here

    char *idx;
    int i=0;
    char *idx_hist = NULL;
    while(1)
    {
        if((idx=strchr(inputCmd, pp))==NULL)
        {
            idx_hist += 3; // reaching the start of the last command
            int j=0;
            cmds[i] = malloc(10*sizeof(char));
            while(*idx_hist != '\0')
                cmds[i][j++] = *(idx_hist++);

            cmds[i][j] = '\0';

            break;
        }
        idx_hist = idx;
        cmds[i] = malloc(50*sizeof(char));
        idx--; // accounting for space before pipe
        char *p = inputCmd;
        int j=0;
        while(p!=idx)
        {
            cmds[i][j++] = *p;
            p++;
        }
        cmds[i][j] = '\0';
        inputCmd = idx+4; // reaching the start of the next command
        i++;
    }
}

void run_cmd(char *cmd)
{
    // check for which command, I/O redirection
}

void sucexec_parser(char *cmd)
{
    // check for &&/|| operators
    if(strchr(cmd, '|')!=NULL)
    {
        char *cmds[2];
        parse_sucexec(cmd, cmds, '|');
        // printf(1, "OR command\n");
        // for(int i=0; i<2; i++)
        //     printf(1, "%s\n", cmds[i]);
        // sequentially run cmds[]
    }
    else if(strchr(cmd, '&') != NULL)
    {
        char *cmds[2];
        parse_sucexec(cmd, cmds, '&');
        // printf(1, "AND command\n");
        // for(int i=0; i<2; i++)
        //     printf(1, "%s\n", cmds[i]);
        // sequentially run cmds[]
    }
    else
        run_cmd(cmd);
}

void run_pipes(char *cmd1, char *cmd2)
{
    // call function to execute per command
}

void pipe_parser(char *cmd)
{
    char *test;
    if((test=strchr(cmd, '|')) != NULL && *(test+1)!='|')
    {
        char *cmds[10];
        parse_any(cmd, cmds, '|');
        // printf(1, "PIPE command\n");
        // for(int i=0; i<2; i++)
        //     printf(1, "%s\n", cmds[i]);
        // cmds[] contains commands
        // fork and pipe here
        int pipeFD[2];

    }
    else
        sucexec_parser(cmd);
}

void sc_parser(char *inputCmd)
{
    if(strchr(inputCmd, ';') != NULL)
    {
        char *cmds[2]; // can be made dynamic
        parse_any(inputCmd, cmds, ';');
        // printf(1, "PARALLEL command\n");
        // for(int i=0; i<2; i++)
        //     printf(1, "%s\n", cmds[i]);
        // cmds[2] contains the seperate commands
        // fork here and run pipe parser
        int pd;
        for(int i=0; i<2; i++)
        {
            // fork and execute cmds[i] in child process
            if((pd=fork())==0)
            {
                pipe_parser(cmds[i]);
                exit(0);
            }
        }

        int cpid, status;
        for(int i=0; i<2; i++)
        {
            cpid = wait(&status);
            printf(1, "Process %d terminated.\n", cpid);
        }
    }
    else
        pipe_parser(inputCmd);
}