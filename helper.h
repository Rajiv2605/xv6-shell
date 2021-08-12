// structure of a command
struct cmd
{
    char *cmd;          // the command to be executed
    char *args[10];        // arguments needed with the command
    char redir;         // 0: no redirection, 1: >, -1: <
    char isPipe;        // 0: no pipe around, 1: pipe exists
    char status;        // return status of the command; useful for && and || operations
    char seqno;         // sequence number of the command
    char isAndOr;       // 0: none, 1: AND, -1: OR
} *cmdTable;

// build the array of commands
void cmd_parser(char *inputCmd);

// clear the command table memory
void free_cmdtable(void);
