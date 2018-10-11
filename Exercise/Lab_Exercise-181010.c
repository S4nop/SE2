#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#ifndef playing

int main(){
    char tmp[15];
    int pid;
    int status;
    if((pid = fork()) == 0){
        if(execl("/bin/ls", "ls", "-al", "/sys/kernel/debug", NULL) < 0){
            printf("exec failed\n");
            exit(0);
        }
    }
    else{
        waitpid(pid, &status, 0);
        printf("Child's pid with %d returns %d", pid, status);
    } 
}

#endif

#ifdef playing

int builtin_command(char **argv) 
{
    if (!strcmp(argv[0], "quit")) /* quit command */
	exit(0);  
    if (!strcmp(argv[0], "&"))    /* Ignore singleton & */
	return 1;
    return 0;                     /* Not a builtin command */
}
int parsecmd(char *buf, char **argv) 
{
    char *delim;         /* Points to first space delimiter */
    int argc;            /* Number of args */
    int bg;              /* Background job? */

    buf[strlen(buf)-1] = ' ';  /* Replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* Ignore leading spaces */
	buf++;

    /* Build the argv list */
    argc = 0;
    while ((delim = strchr(buf, ' '))) {
	argv[argc++] = buf;
	*delim = '\0';
	buf = delim + 1;
	while (*buf && (*buf == ' ')) /* Ignore spaces */
	       buf++;
    }
    argv[argc] = NULL;
    
    if (argc == 0)  /* Ignore blank line */
	return 1;

    /* Should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0)
	argv[--argc] = NULL;

    return bg;
}

static char *getcmd() {
    static char buf[256];

    fputs("> ", stdout); fflush(stdout);
    if (fgets(buf, sizeof(buf), stdin) == NULL)
        return NULL;

    if (buf[strlen(buf)] == '\n')
        buf[strlen(buf)] = 0;

    return buf;
}
int main(){
    char *cmdline;
    char *argv[20];
    pid_t pid;
    int status;

    while(cmdline = getcmd()){
        parsecmd(cmdline, argv);
        if (!builtin_command(argv)){
            if ((pid = fork()) == 0){
                if (execv(argv[0], argv) < 0){
                    printf("%s: command not found\n", argv[0]);
                    exit(0);
                }
            }
        }
        waitpid(pid, &status, 0);
        printf("Child's pid with %d returns %d\n\n", pid, status);
    }
}

#endif