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
// Student Name  : Ryu SungHee
// Student ID No.: 2017314895
//------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> 
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

typedef struct Node{
    char title[256];
    int tlen;
    struct Node * branch[28];
}nd, *pnd;

int urlSeq, pNum, finNum, isGetAll;
int fpid[100], pipFd[2];
char fBuffer[100000], tmpfbuffer[100000];
nd head;

void collect (void);
void DLweb(char* url);
static char *getcmd();
char *int2str (char *dest, int num);
char* findTitle(int pNum);
void svData(pnd pn, char* tt);
pnd MakeNd(void);
void movTrie(pnd *Node, char c);
int parsecmd(char *buf, char **argv);
char* trim(char * buf);
void handler(int sig);

int main (int argc, char* argv[]) {
    signal(SIGCHLD, handler);
    signal(SIGINT, handler);
    if(pipe(pipFd) < 0){
        perror("pipe error : ");
        exit(0);
    }
    collect();

    return 0;
}

int parsecmd(char *buf, char **argv) 
{
    char *delim;         /* Points to first space delimiter */
    int argc;            /* Number of args */
    //buf[strlen(buf)] = ' ';  /* Replace trailing '\n' with space */
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
    argv[argc++] = buf;
	buf = delim + 1;
    argv[argc] = NULL;
    
    if (argc == 0)  /* Ignore blank line */
	return 1;

    return argc;
}

void collect (void){
    char * cmdLine, *argv[20], *buftok, *svBuftok;
    char tmp[256], tmp2[25], rdBuf[8192], cmdParse[8],svArgv[256];
    int argc, tmpN, i, fd, hasBuf = 0;
    pnd pNode = &head;
    while(cmdLine = hasBuf ? strtok_r(svBuftok, "\n", &svBuftok) : getcmd()){
        if(hasBuf && strlen(svBuftok) == 0) hasBuf = 0;
        if(cmdLine[strlen(cmdLine) - 1] == '\n') cmdLine[strlen(cmdLine) - 1] = '\0';
        strncpy(cmdParse, cmdLine, 7);
        //printf("::::%s\n", cmdLine);
        pNum++;
        if(!strcmp(cmdParse, "http://") || (!strcmp(cmdParse, "https:/") && cmdLine[7] == '/')){

            isGetAll = 0;
            DLweb(cmdLine);
        }
        else{
            parsecmd(cmdLine, argv);
            
            if(argv[0][0] == 'p'){    
                if(!isGetAll && finNum){
                memset(rdBuf, 0, 8191);
                for(i = 1; i <= finNum; i++){
                    waitpid(fpid[i], NULL, 0);
                }
                isGetAll = 1;
                if(read(pipFd[0], rdBuf, 8191)){
                    for(i = strlen(rdBuf); rdBuf[i] != '@'; i--);
                    rdBuf[i + 1] = 0;
                    buftok = strtok(rdBuf, "@");
                    tmpN = strlen(buftok);
                    for(i = 0; i < tmpN; i++){
                        if(buftok[i] == '.') buftok[i] = 'z' + 1;
                        movTrie(&pNode, buftok[i]);
                    }


                    buftok = strtok(NULL, "@");
                    svData(pNode, buftok);
                    while(1){
                        pNode = &head;
                        if(!(buftok = strtok(NULL, "@"))) break;
                        tmpN = strlen(buftok);
                        for(i = 0; i < tmpN; i++){
                            if(buftok[i] == '.') buftok[i] = 'z' + 1;
                            movTrie(&pNode, buftok[i]);
                        }
                        buftok = strtok(NULL, "@");
                        svData(pNode, buftok);
                    }
                }
            }
                pNode = &head;
                memset(tmp, 0, 255);
                tmpN = strlen(argv[1]);
                for(i = 0; i < tmpN; i++){
                    if(argv[1][i] == '.') movTrie(&pNode, 'z' + 1);
                    else movTrie(&pNode, argv[1][i]);
                }
                if(!pNode->tlen) continue;
                int2str(tmp, pNum);
                strcat(tmp, ">");
                strcat(tmp, argv[1]);
                strcat(tmp, ":");
                strcat(tmp, pNode->title);
                strcat(tmp, "\n");
                write(1, tmp, strlen(tmp));
                
            }
            else if(argv[0][0] == 's'){
                if(!isGetAll && finNum){
                memset(rdBuf, 0, 8191);
                for(i = 1; i <= finNum; i++){
                    waitpid(fpid[i], NULL, 0);
                }
                isGetAll = 1;
                if(read(pipFd[0], rdBuf, 8191)){
                    for(i = strlen(rdBuf); rdBuf[i] != '@'; i--);
                    rdBuf[i + 1] = 0;
                    buftok = strtok(rdBuf, "@");
                    tmpN = strlen(buftok);
                    for(i = 0; i < tmpN; i++){
                        if(buftok[i] == '.') buftok[i] = 'z' + 1;
                        movTrie(&pNode, buftok[i]);
                    }


                    buftok = strtok(NULL, "@");
                    svData(pNode, buftok);
                    while(1){
                        pNode = &head;
                        if(!(buftok = strtok(NULL, "@"))) break;
                        tmpN = strlen(buftok);
                        for(i = 0; i < tmpN; i++){
                            if(buftok[i] == '.') buftok[i] = 'z' + 1;
                            movTrie(&pNode, buftok[i]);
                        }
                        buftok = strtok(NULL, "@");
                        svData(pNode, buftok);
                    }
                }
            }
                pNode = &head;
                memset(tmp, 0, 255);
                int2str(tmp, pNum);
                strcat(tmp, ">");
                int2str(tmp2, finNum);
                strcat(tmp, tmp2);
                strcat(tmp, " titles\n");
                write(1, tmp, strlen(tmp));
            }
            else if(argv[0][0] == 'l'){
                pNum--;
                memset(svArgv, 0, 255);
                strcpy(svArgv, argv[1]);
                if(hasBuf){
                    strcpy(tmpfbuffer, svBuftok);
                    tmpfbuffer[strlen(svBuftok)] = 0;
                }
                
                if((fd = open(svArgv, O_RDONLY))){
                    memset(rdBuf, 0, 8191);
                    memset(fBuffer, 0, 99999);
                    strcat(fBuffer, " \n");
                    while(read(fd, rdBuf, 7999)){
                        strcat(fBuffer, rdBuf);
                        memset(rdBuf, 0, 7999);
                    }
                    if(hasBuf){
                        strcat(fBuffer, tmpfbuffer);
                    }
                    strtok_r(fBuffer, "\n", &svBuftok);
                    hasBuf = 1;
                }
                close(fd);
            }
            else if(argv[0][0] == 'q'){
                for(i = 1; i <= finNum; i++ ) kill(fpid[i], SIGKILL);
                exit(0);
            }
        }
    }
}

static char *getcmd(char * cLine) {
    static char buf[256];
    memset(buf, 0, sizeof(buf));
    //write(1, "> ", 2);

    if (read(0, buf, sizeof(buf)) < 0)
        return NULL;

    if (buf[strlen(buf)] == '\n')
        buf[strlen(buf)] = 0;
    
    return buf;
}

void DLweb(char * url){
    int pid, status, i, tmp, chkdot;
    char buf[256], pbuf[256];
    pnd pNode = &head;
    if((fpid[++finNum] = fork()) == 0){
        close(pipFd[0]);
        if((pid = fork()) == 0){
            //printf("%s\n", url);
            if(execl("/usr/bin/wget", "wget", url, "-q", "-O", int2str(NULL, finNum),  NULL) < 0){
                printf("exec failed\n"); //just for debug!
                exit(0);
            }
            exit(0);
        }
        else{
            //printf("%s\n", url);
            tmp = strlen(url) - 1;
            chkdot = (url[tmp] == 'r' && url[tmp - 1] == 'k') ? 3 : 2;


            for(i = tmp; i > 0; i--){
                if(url[i] == '.') {
                    if(--chkdot == 0) break;
                }
            }
            url = &url[i + 1];
            //strtok(url, ".");
            //strcpy(url, strtok(NULL, "/"));
            //printf("%s\n", url);
            
            waitpid(pid, &status, 0);
            memset(buf, 0, sizeof(buf));
            if(status == -1){
                write(1, int2str(buf, finNum), strlen(buf));
                write(1, ">Error occured!\n", 16);
            }    
            else{
                strcpy(pbuf, findTitle(finNum));
                
                int2str(buf, pNum);
                strcat(buf, ">");
                strcat(buf, pbuf);
                strcat(buf, "\n");
                write(1, buf, strlen(buf));

                memset(buf, 0, 255);
                strcpy(buf, url);
                strcat(buf, "@");
                strcat(buf, pbuf);
                strcat(buf, "@");
                write(pipFd[1], buf, strlen(buf));
                // svData(pNode, buf);
                close(pipFd[1]);
            }
            exit(0);
        }

    }
    
}

char* findTitle(int pNum){
    char buf[1024], c;
    char *buftok, *trimer;
    int fdd;
    if((fdd = open(int2str(NULL, pNum), O_RDONLY)) < 0){
        write(2, "Source file not found\n", 22);
        exit(-1);
    };
    while(read(fdd, buf, sizeof(buf))){
        buftok = strtok(buf, "<");
        while(buftok && !(buftok[0] == 't' && buftok[1] == 'i' && buftok[2] == 't' && buftok[3] == 'l')){
            buftok = strtok(NULL, "<");
        }
        if(buftok){
            buftok = strtok(buftok, ">");
            buftok = strtok(NULL, "");
            buftok = trim(buftok);
            //printf("::::%s\n", buftok);
            return buftok;
        }
    }

    return NULL;
}

/* Conversion of numeric format to string */
char *int2str (char *dest, int num) {
    if(!(dest = (dest ? dest : malloc(12)))) return NULL;

	char befRev[12] = { 0 };
    int i, j;
    if(!num) {
        dest[0] = '0';
        dest[1] = 0;
        return dest;
    }
    for(i = 0; num; befRev[i++] = num % 10 + '0', num /= 10);
	
	for (j = 0; i > 0; dest[j++] = befRev[--i]);
    dest[j] = '\0';

    return dest;
}

void svData(pnd pn, char* tt){
    int tmp = strlen(tt);
    if(pn->tlen >= tmp) return;

    strcpy(pn->title, tt);
    pn->tlen = tmp;
}

pnd makeNd(){
	int i;
	pnd tmp = (pnd)malloc(sizeof(nd));
	tmp->tlen = 0;
	for (i = 0; i < 28; i++)
		tmp->branch[i] = NULL;

	return tmp;
}

void movTrie(pnd *Node, char c){
	if (!(*Node)->branch[c - 'a']){
		(*Node)->branch[c - 'a'] = makeNd();
	}
	(*Node) = (*Node)->branch[c - 'a'];

}

char* trim(char * buf){
    char * tmp;
    while(*buf && ((*buf == ' ') || (*buf == '\n') || (*buf == 9))) buf++;
    
    tmp = buf + strlen(buf) - 1;
    while(*tmp && ((*tmp == ' ') || (*tmp == '\n') || (*tmp == 9))) tmp--;
    *(tmp + 1) = 0;
    return buf;
}
void handler(int sig){
    if(sig == SIGKILL) exit(0);
    if(sig == SIGINT) return;
}