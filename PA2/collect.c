//------------------------------------------------------------------
// SWE2007: Software Experiment II (2018 Fall)
//
// Skeleton code for PA #2
// October 10, 2018
//
// Somm Kim, Sooyun Lee
// Embedded Software Laboratory
// Sungkyunkwan University
//
// Student Name  : 
// Student ID No.: 
//------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> 
#include <sys/wait.h>
#include <string.h>

void collect (void);

static char *getcmd() {
    static char buf[256];

    write(1, "> ", 2);
    if (read(0, buf, sizeof(buf)) <= 0) return NULL;

    if (buf[strlen(buf)] == '\n')
        buf[strlen(buf)] = 0;

    return buf;
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

int main (int argc, char* argv[]) {
    collect();
    return 0;
}

void collect (void){
    char *cmdLine;
    char tmp[256];
    cmdLine = getcmd();
    strncpy(tmp, cmdLine, 7);
    if(!strcmp(tmp, "http://") || !strcmp(tmp, "https:/")){
	
    }
    printf("%s\n", cmdLine);
}
