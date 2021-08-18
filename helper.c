void sc_parser(char *inputCmd);

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
            idx_hist += 1; // reaching the start of the last command
            while(*idx_hist == ' ')
                idx_hist++;
            int j=0;
            cmds[i] = malloc(50*sizeof(char));
            while(*idx_hist != '\0')
                cmds[i][j++] = *idx_hist++;

            cmds[i][j] = '\0';

            break;
        }
        idx_hist = idx;
        cmds[i] = malloc(50*sizeof(char));
        idx--; // accounting for space before pipe
        while(*idx == ' ')
            idx--;
        idx++;
        char *p = inputCmd;
        int j=0;
        while(p!=idx)
            cmds[i][j++] = *p++;

        cmds[i][j] = '\0';
        // inputCmd = idx+3; // reaching the start of the next command
        while(*idx==' ')
            idx++;
        idx++;
        while(*idx==' ')
            idx++;
        inputCmd = idx;
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
            while(*idx_hist == ' ')
                idx_hist++;
            int j=0;
            cmds[i] = malloc(10*sizeof(char));
            while(*idx_hist != '\0')
                cmds[i][j++] = *(idx_hist++);

            cmds[i][j] = '\0';

            break;
        }
        idx_hist = idx;
        cmds[i] = malloc(50*sizeof(char));
        idx--; // accounting for space before operator
        while(*idx==' ')
            idx--;
        idx++;
        char *p = inputCmd;
        int j=0;
        while(p!=idx)
        {
            cmds[i][j++] = *p;
            p++;
        }
        cmds[i][j] = '\0';
        while(*idx==' ')
            idx++;
        idx+=2;
        while(*idx==' ')
            idx++;
        inputCmd = idx;
        i++;
    }
}

void run_cmd(char *cmd, int *sts, int is_pipe_recv)
{
    // check for which command, I/O redirection
    char in_file[10];
    char out_file[10];
    char *idx;
    int i=0;
    int in_dir=0;
    int out_dir=0;

    if((idx=strchr(cmd, '>'))!=NULL)
    {
        out_dir = 1;
        idx++;
        while(*idx == ' ')
            idx++;
        while(*idx != ' ')
            out_file[i++] = *idx++;
    }
    i=0;
    if((idx=strchr(cmd, '<'))!=NULL)
    {
        in_dir = 1;
        idx++;
        while(*idx == ' ')
            idx++;
        while(*idx != ' ')
            in_file[i++] = *idx++;
    }

    char *cmd_name;
    int isDynamic = 0;

    if(fork()==0)
    {
        // open files if needed
        int fd_in;
        int fd_out;
        if(in_dir)
        {
            // printf(1, "In file: %s\n", in_file);
            fd_in = open(in_file, O_RDONLY);
            if(fd_in < 0)
                printf(1, "Error opening file %s!\n", in_file);
            close(0);
            dup(fd_in);
        }
        if(out_dir)
        {
            // printf(1, "Out file: %s\n", out_file);
            fd_out = open(out_file, O_CREATE | O_WRONLY);
            if(fd_out < 0)
                printf(1, "Error opening file %s!\n", out_file);
            // dup2(fd_out, 1);
            close(1);
            dup(fd_out);
        }

        // find which command it is
        if(strchr(cmd, ' ')==NULL)
            cmd_name = cmd;
        else
        {
            isDynamic = 1;
            cmd_name = malloc(sizeof(char)*15);
            idx = cmd;
            i=0;
            while(*idx != ' ')
                cmd_name[i++] = *idx++;
        }
        // i=0;
        // printf(1, "Command name: %s\n", cmd_name);
        if(strcmp(cmd_name, "ls")==0)
        {
            char *args[] = {cmd_name, NULL};
            exec(args[0], args);
        }
        else if(strcmp(cmd_name, "cat")==0)
        {
            // parse filename if no redirection.
            if(in_dir || is_pipe_recv)
            {
                char *args[] = {cmd_name, NULL};
                exec(args[0], args);
            }
            else if(!in_dir)
            {
                idx = strchr(cmd, ' ');
                while(*idx == ' ')
                    idx++;
                char filename[20];
                i=0;
                while(*idx != '\0' && *idx != ' ')
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

            if(in_dir || is_pipe_recv)
            {
                char *args[] = {cmd_name, pattern, NULL};
                exec(args[0], args);
            }
            else if(!in_dir)
            {
                i=0;
                while(*idx == ' ')
                    idx++;
                char filename[20];
                // printf(1, "POinter at %c\n", *idx);
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
            // exit(0);
        }
        else if(strcmp(cmd_name, "wc")==0)
        {
            // dk how to handle this.
            char *args[] = {cmd_name, NULL};
            exec(args[0], args);
            // exit(0);
        }
        else if(strcmp(cmd_name, "ps")==0)
        {
            psinfo();
            exit(0);
        }
        else if(strcmp(cmd_name, "procinfo")==0)
        {
            idx = strchr(cmd, ' ');
            while(*idx == ' ')
                idx++;
            char cpid[5];
            i=0;
            while(*idx != ' ')
                cpid[i++] = *idx++;
            int pid = atoi(cpid);
            // printf(1, "procinfo pid: %d\n", pid);
            procinfo(pid);
            exit(0);
        }
        else if(strcmp(cmd_name, "executeCommands")==0)
        {
            // parse filename
            idx = strchr(cmd, ' ');
            while(*idx == ' ')
                idx++;
            i=0;
            char filename[20], file_lines[100];
            while(*idx != ' ')
                filename[i++] = *idx++;

            char *cmds[10];
            int fd_exec = open(filename, 0);
            if(fd_exec < 0)
            {
                printf(1, "Error opening file!\n");
                exit(-1);
            }
            
            read(fd_exec, file_lines, 10000);
            int count = 1;
            for(char *p = file_lines; *p != '\0'; p++)
                if(*p=='\n')
                    count++;
            parse_any(file_lines, cmds, '\n');

            for(int i=0; i<count; i++)
                sc_parser(cmds[i]);

            for(int i=0; i<count; i++)
                if(cmds[i] != NULL)
                    free(cmds[i]);

            exit(0);
        }
        else
        {
            printf(1, "Illegal command!: %s\n", cmd_name);
            exit(-1);
        }
    }
    else
    {
        if(isDynamic)
            free(cmd_name);
        int status;
        wait(&status);
        *sts = status;
        // printf(1, "Process terminated with exit code %d.\n", status);
    }

}

void sucexec_parser(char *cmd)
{
    // check for &&/|| operators
    int status;
    if(strchr(cmd, '|')!=NULL)
    {
        char *cmds[2];
        parse_sucexec(cmd, cmds, '|');
        // printf(1, "1st command: %s\n", cmds[0]);
        // printf(1, "2nd command: %s\n", cmds[1]);
        if(fork()==0)
        {
            run_cmd(cmds[0], &status, 0);
            free(cmds[0]);
        }

        int status2;
        wait(&status2);
        // printf(1, "status = %d\n", status);
        if(status==0)
            return;
        
        if(fork()==0)
        {
            run_cmd(cmds[1], &status, 0);
            free(cmds[1]);
        }

        wait(&status);
        // printf(1, "OR command\n");
        // for(int i=0; i<2; i++)
        //     printf(1, "%s\n", cmds[i]);
        // sequentially run cmds[]
    }
    else if(strchr(cmd, '&') != NULL)
    {
        char *cmds[2];
        parse_sucexec(cmd, cmds, '&');
        // printf(1, "1st command: %s\n", cmds[0]);
        if(fork()==0)
        {
            run_cmd(cmds[0], &status, 0);
            free(cmds[0]);
        }

        int status2;
        wait(&status2);

        if(status==-1)
            return;

        if(fork()==0)
        {
            run_cmd(cmds[1], &status, 0);
            free(cmds[1]);
        }

        wait(&status);
        // printf(1, "AND command\n");
        // for(int i=0; i<2; i++)
        //     printf(1, "%s\n", cmds[i]);
        // sequentially run cmds[]
    }
    else
        run_cmd(cmd, &status, 0);
}

void run_pipe(char *cmd1, char *cmd2)
{
    // printf(1, "Command 1: %s\n", cmd1);
    // printf(1, "Command 2: %s\n", cmd2);
    // call function to execute per command
    int pipe_fd[2];
    int status;
    if(pipe(pipe_fd) < 0)
    {
        printf(1, "Pipe error\n");
        exit(-1);
    }
    // 1st child, left end of the pipe
    if(fork()==0)
    {
        close(1);
        dup(pipe_fd[1]);
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        run_cmd(cmd1, &status, 0);
        exit(0);
    }

    // 2nd chlid, right end of the pipe
    if(fork()==0)
    {
        close(0);
        dup(pipe_fd[0]);
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        run_cmd(cmd2, &status, 1);
        exit(0);
    }

    close(pipe_fd[0]);
    close(pipe_fd[1]);
    wait(0);
    wait(0);
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
        run_pipe(cmds[0], cmds[1]);
        free(cmds[0]);
        free(cmds[1]);
    }
    else
        sucexec_parser(cmd);
}

void sc_parser(char *inputCmd)
{
    // printf(1, "inputCmd: %s\n", inputCmd);
    if(strchr(inputCmd, ';') != NULL)
    {
        char *cmds[2]; // can be made dynamic
        parse_any(inputCmd, cmds, ';');
        printf(1, "PARALLEL command 1: %s\n", cmds[0]);
        printf(1, "PARALLEL command 2: %s\n", cmds[1]);
        // for(int i=0; i<2; i++)
        //     printf(1, "%s\n", cmds[i]);
        // cmds[2] contains the seperate commands
        // fork here and run pipe parser
        for(int i=0; i<2; i++)
        {
            // fork and execute cmds[i] in child process
            if(fork()==0)
            {
                pipe_parser(cmds[i]);
                free(cmds[i]);
                exit(0);
            }
        }

        int status;
        for(int i=0; i<2; i++)
        {
            wait(&status);
            // printf(1, "Process ; terminated with exit code %d.\n", status);
        }
    }
    else
        pipe_parser(inputCmd);
}