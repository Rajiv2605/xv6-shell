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
    char in_file[20];
    char out_file[20];
    char *idx;
    // char *cmd_aug = malloc(sizeof(char)*(sizeof(cmd)+1));
    int i=0;
    char in_dir=0;
    char out_dir=0;
    // while(i < sizeof(cmd))
    // {
    //     cmd_aug[i] = cmd[i];
    //     i++;
    // }
    // cmd_aug[i] = ' ';
    // i=0;
    if((idx=strchr(cmd, '>'))!=NULL)
    {
        out_dir = 1;
        while(*idx++ != ' ');
        while(*idx != ' ')
            in_file[i++] = *idx++; 
    }
    i=0;
    if((idx=strchr(cmd, '<'))!=NULL)
    {
        in_dir = 1;
        while(*idx++ != ' ');
        while(*idx != ' ')
            out_file[i++] = *idx++; 
    }

    char *cmd_name;
    int isDynamic = 0;

    if(fork()==0)
    {
        // open files if needed
        int fd_in, fd_out;
        if(in_dir)
        {
            fd_in = open(in_file, O_RDONLY);
            // dup2(fd_in, 0);
            close(0);
            dup(fd_in);
        }
        if(out_dir)
        {
            fd_out = open(out_file, O_WRONLY);
            // dup2(fd_out, 1);
            close(1);
            dup(fd_out);
        }

        // find which command it is
        // printf(1, "CMD:: %s\n", cmd);
        if(strchr(cmd, ' ')==NULL)
            cmd_name = cmd;
        else
        {
            isDynamic = 1;
            cmd_name = malloc(sizeof(char)*15);
            idx = cmd;
            while(*idx != ' ')
                cmd_name[i++] = *idx++;
        }
        // i=0;
        if(strcmp(cmd_name, "ls")==0)
        {
            char *args[] = {cmd_name, NULL};
            exec(args[0], args);
        }
        else if(strcmp(cmd_name, "cat")==0)
        {
            // parse filename if no redirection.
            if(!in_dir)
            {
                idx = strchr(cmd, ' ');
                while(*idx++ != ' ');
                char filename[20];
                i=0;
                while(*idx != '\0')
                    filename[i++] = *idx++;

                char *args[] = {cmd_name, filename, NULL};
                exec(args[0], args);
            }
        }
        else if(strcmp(cmd_name, "grep")==0)
        {
            // parse pattern.
            idx = strchr(cmd, ' ');
            while(*idx++ != ' ');
            char pattern[20];
            i=0;
            while(*idx != ' ')
                pattern[i++] = *idx++;

            if(!in_dir)
            {
                i=0;
                while(*idx == ' ')
                    idx++;
                char filename[20];
                printf(1, "POinter at %c\n", *idx);
                while(*idx != '\0')
                    filename[i++] = *idx++;

                char *args[] = {cmd_name, pattern, filename, NULL};
                exec(args[0], args);
            }

            // parse filename if no redirection.
        }
        else if(strcmp(cmd_name, "echo")==0)
        {
            // parse echo string.
            idx = strchr(cmd, ' ');
            while(*idx++ != ' ');
            char echo_string[20];
            i=0;
            while(*idx != '\0')
                echo_string[i++] = *idx++;
            char *args[] = {cmd_name, echo_string, NULL};
            exec(cmd_name, args);
            exit(0);
        }
        else if(strcmp(cmd_name, "wc")==0)
        {
            // dk how to handle this.
            char *args[] = {cmd_name, NULL};
            exec(args[0], args);
            exit(0);
        }
        else if(strcmp(cmd_name, "ps")==0)
        {
            psinfo();
            exit(0);
        }
        else if(strcmp(cmd_name, "procinfo")==0)
        {
            idx = strchr(cmd, ' ');
            while(*idx++ != ' ');
            char cpid[5];
            i=0;
            while(*idx != ' ')
                cpid[i++] = *idx++;
            int pid = atoi(cpid);
            // printf(1, "procinfo pid: %d\n", pid);
            procinfo(pid);
        }
        else if(strcmp(cmd_name, "executeCommands")==0)
        {
            // implement
        }
        else
        {
            printf(1, "Illegal command!\n");
            exit(0);
        }
    }
    else
    {
        if(isDynamic)
            free(cmd_name);
        int pid, status;
        pid = wait(&status);
        printf(1, "Process %d terminated with exit code %d.\n", pid, status);
    }

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
        // int pipeFD[2];

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
            printf(1, "Process %d terminated with exit code %d.\n", cpid, status);
        }
    }
    else
        pipe_parser(inputCmd);
}