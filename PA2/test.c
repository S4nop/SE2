#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <string.h>

#define BUFS 256
typedef struct Node
{
    char title[256];
    int tlen;
    struct Node *branch[28];
} nd, *pnd;

nd head;

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

int main()
{
    struct timeval s_time, e_time, r_time;
    //int FF;
    int tmp, i;
    char buf[BUFSIZ]; //아까 데이터를 주고받는다고 했으니 일단 버퍼공간을 생성합니다.
    int fd[100][2];
    pnd pNode = &head;
    char url[256];
    pipe(fd); //으잌ㅋ? 파이프를 생성합니다. fd[0]과 fd[1]이 일단 뭔진 모르지만 생겨났구요.
    
    pid_t pid;
    pid = fork(); //fork함수를 통해 정보를 주고받을 2개의 프로세스 객체를 생성합니다.

    if (pid == 0)
    { //자식 프로세스의 경우//
        write(fd[1], "http://www.naver.com", BUFS);
        exit(0); //write로 fd[1]에다가 뭔가를 썼습니다. ;; 그럼 부모는?
    }
    else
    { //부모 프로세스의 경우//

        
        gettimeofday(&s_time, NULL);
        //int FF = open("2", O_RDONLY);
        read(fd[0], url, BUFS); //부모가 fd[0]으로 읽네요? fd[0]은 아무것도 적혀있지 않는데...
        strtok(url, ".");
            strcpy(buf, strtok(NULL, "/"));
            tmp = strlen(buf);
            for(i = 0; i < tmp; i++){
                if(buf[i] == '.') buf[i] = 'z' + 1;
                movTrie(&pNode, buf[i]);
            }
        gettimeofday(&e_time, NULL);
        timersub(&e_time, &s_time, &r_time);

        printf("%ld\n", 100000 * (r_time.tv_sec) + r_time.tv_usec);
        //printf("Output of parent process :: %s \n\n", buffer);
    }

    //int fd = open("1", O_RDONLY);
    //read(fd, tmp, 30);
}
