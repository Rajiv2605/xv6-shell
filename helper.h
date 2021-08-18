// parse the user input command.
void parse_any(char *inputCmd, char **cmds, char pp);

// parse commands that have to be successively executed.
void parse_sucexec(char *inputCmd, char **cmds, char pp);

// execute the command passed as argument.
void run_cmd(char *cmd, int *sts, int is_pipe_recv);

// run commands that have to be successively executed.
void run_sucexec(char *cmd);

// run the commands part of the pipe.
void run_pipe(char *cmd1, char *cmd2);

// parse the commands in the pipe.
void pipe_parser(char *cmd);

// parse the commands that have to be parallel processed.
void sc_parser(char *inputCmd);
