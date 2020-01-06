#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>

#define MAXLINE 80
#define MAXBUFF 30000

typedef struct filedescripter{
    int bible;
    int index;
    struct filedescripter * next;
}FDs;

typedef struct printedVerse{
    int chapter;
    int verse;
    struct printedVerse *next;
    struct printedVerse *prev;
}legacy;

legacy * leg;
int num_of_word;
char *findwords[10];
int mod;
FDs *fd_index;
int fd_bible;

void connFunction(int connfd,char * input);
int my_fgets(int fd, char *buf);
int get_word(int fd,char *buf, char dis);
int afterfind(int connfd, int fd, char *buf);
int get_word_buffer(char *src, char *dst, char dis, int offset);
void parser(char *buf, int* chp, int* ver, int* loc);
int wordfinder(int connfd, FDs *fd_index);
int legacyfree(legacy* start);
int initlegacy(legacy* start);
extern inline int legacycmp(legacy* l1, legacy* l2);

/*
int main (int argc, char *argv[]) {
    
    int n, listenfd, connfd;
    socklen_t caddrlen;
    struct hostent *h;
    struct sockaddr_in saddr, caddr;
    char buf[MAXLINE];
    int port = atoi(argv[1]);

    if(argc<2){
        printf("need more argument!\n");
        exit(0);
    }
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket() failed.\n");
        exit(1);
    }

    bzero((char *)&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(port);

    if (bind(listenfd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
        printf("bind() failed.\n");
        exit(2);
    }

    if (listen(listenfd, 5) < 0) {
        printf("listen() failed.\n");
        exit(3);
    }

    while (1) {
        caddrlen = sizeof(caddr);
        if ((connfd = accept(listenfd, (struct sockaddr *)&caddr, &caddrlen)) < 0) {
            printf ("accept() failed.\n");
            continue;
        }
        
        close(listenfd);
        h = gethostbyaddr((const char *)&caddr.sin_addr.s_addr,sizeof(caddr.sin_addr.s_addr), AF_INET);
        printf("server connected to %s (%s)\n", h->h_name, inet_ntoa(*(struct in_addr *)&caddr.sin_addr));

        
        while ((n = read(connfd, buf, MAXLINE)) > 0) {
            buf[n]=0;
            connFunction(connfd,buf);
            bzero((char *)buf, sizeof(buf));
            n=write(connfd,"\n\n",2);
            if(n==0)
                printf("write() failed\n");
            //write(connfd, buf, n);
        }
        
        printf("connection terminated.\n");
        close(connfd);
        exit(0);
    }
    close(connfd);
}*/
int main(){
    int i;
    char tmp_buf[100]={0,};
    FDs *cur=fd_index;
    char bibleFileNm[]="./bible.txt";

    //bible.txt를 읽고 index 파일들을 fd 링크드 리스트로 저장하는 루틴
    fd_bible=open(bibleFileNm,O_RDONLY);
    while(my_fgets(fd_bible,tmp_buf)){
        FDs *new;
        for(i=0;;i++){
            if(tmp_buf[i]==' '){
                tmp_buf[i]=0;
                break;
            }
        }
        new=(FDs*)malloc(sizeof(FDs));
        new->next=NULL;
        new->bible=open(tmp_buf,O_RDONLY);
        new->index=open(&tmp_buf[i+1],O_RDONLY);
        //printf("%x\n",new);
        if(fd_index==NULL){
            fd_index=new;
            cur=fd_index;
            // printf("%x\n",cur);
        }
        else{
            cur->next=new;
            cur=cur->next;
        }
    }
    connFunction(1,"former");
    connFunction(1,"former");
}

void connFunction(int connfd, char * input){
    int i;
    char tmp_buf[100]={0,};
    
    //input 처리 코드
    //input이 "로 시작할때
    if(input[0]==34){

    }
    //단일 단어 여러개일때
    else{
        int sum=0;
        for(i=0;;i++){
            int test;
            test=get_word_buffer(input,tmp_buf,' ', sum);
            int len=strlen(tmp_buf);
            findwords[i]=(char*)malloc(sizeof(char)*len);
            strcpy(findwords[i],tmp_buf);
            sum=sum+test+1;
            if(test==-1) break;
        }
        num_of_word=i+1;
    }
    printf("%d is number of word :  ",num_of_word);
    for(int k=0;k<num_of_word;k++){
        printf("%s  ",findwords[k]);
    }
    wordfinder(connfd, fd_index);

    //printf("after wordfinder\n");
    for(i=0;i<10;i++){
        if(findwords[i]==0) break;
        free(findwords[i]);
    }
    /*while ((n = read(connfd, buf, MAXLINE)) > 0) {
        buf[n]=0;
        printf ("%s", buf);
        bzero((char *)buf, sizeof(buf));
        write(connfd,"send complete.\n",15);
        //write(connfd, buf, n);
    }*/
}

//단어를 fd_index에서 찾아서 해당 내용(단어, 단어 수, 장:절:위치, 장:절:위치...) 을 buffer에 담고 afterfind를 호출
int wordfinder(int connfd, FDs *fd_index){
    int i;
    char tmp=0;
    char tmp_buf[MAXBUFF]={0,};
    FDs *cur=fd_index;
    int flag=0;

    while(cur){
        printf("in the while loop\n");
        lseek(cur->index,0,SEEK_SET);
        for(i=0;;i++){
            flag=0;
            if(!get_word(cur->index,tmp_buf, ':')){
                break;
            }
            if(i==0){
                continue;
            }
            for(int j=0;j<10;j++){
                if(findwords[j]==NULL){
                    break;
                }
                if(!strcmp(findwords[j],tmp_buf)){
                    my_fgets(cur->index,tmp_buf);
                    printf("find %s\n",findwords[j]);
                    afterfind(connfd, cur->bible, tmp_buf);
                    flag=1;
                    break;
                }
            }
            if(flag==0){
                while(read(cur->index,&tmp,1)){
                    if(tmp=='\n') break;
                }
            }
        }
        legacyfree(leg);
        cur=cur->next;
    }
    return 0;
}

//단어를 찾은 후 실행되는 함수, fd는 성경 파일, buf는 {단어수, 장:절:위치, 장:절:위치...}
//현재 작업중(11-22 21:00)
int afterfind(int connfd, int fd, char *buf){

    printf("afterfind...\n");
    lseek(fd,0,SEEK_SET);
    int i,n;
    unsigned int j;
    char buffer[100]={0,};
    char tmp_buf[MAXBUFF]={0,};
    char target[10]={0,};
    char name[20]={0,};
    int sum=0;
    int chapter, location, verse;
    int flag=0;

    get_word(fd, name, '\n'); //성경 이름을 제거
    printf("%s!!\n",name);
    sum+=get_word_buffer(buf,tmp_buf,',', sum);
    sum++;
    n=atoi(&tmp_buf[1]);
    get_word(fd, tmp_buf, ' ');

    //index에서 단어의 장 절 위치에 해당되는 부분을 토큰화
    for(i=0;i<n;i++){
        legacy * new=(legacy*)malloc(sizeof(legacy));
        legacy * cur=leg;
        legacy *prev=NULL;
        initlegacy(new);

        int c=0;
        sum+=get_word_buffer(buf,buffer,',', sum);
        sum++;
        parser(buffer+1, &chapter,&verse, &location);

        new->chapter=chapter;
        new->verse=verse;
        if(cur==NULL){
            leg=new;
        }
        else{
            while(cur){
                //if(cur->next!=NULL)
                    //printf("compare with %d:%d legacy system, %d:%d is next. ", cur->chapter, cur->verse, cur->next->chapter, cur->next->verse);
                //printf("%x is cur address, %x is next address\n", cur, cur->next);
                if(legacycmp(new,cur)==1){
                    flag=1;
                    //printf("%d:%d is in legacy system!\n", new->chapter, new->verse);
                    break;
                }
                else{
                    prev=cur;
                    cur=cur->next;
                    //printf("%x is  prev address, %x is next address\n", prev, cur);
                }
            }
            if(flag==1){
                flag=0;
                continue;
            }
            //printf("%d:%d is in new system!\n", new->chapter, new->verse);
            prev->next=new;
            new->prev=prev;
        }

        for(j=1;j<strlen(buffer);j++){
            if(buffer[j]==':'){
                c++;
                target[j-1]=':';
                continue;
            }
            if(c==2)
                target[j-1]=0;
            else
                target[j-1]=buffer[j];
        }

        while(strcmp(target,tmp_buf)){
            char tmp;
            while(1){
                if(!read(fd,&tmp,1)){
                    break;
                }
                //printf("%c",tmp);
                if(tmp=='\n')
                    break;
            }
            get_word(fd,tmp_buf,' ');
        }
        get_word(fd,tmp_buf,'\n');
        lseek(fd,-1,SEEK_CUR);
        //printf("%s%s\n,%d\n",target, tmp_buf,strlen(tmp_buf));

        //한 성경 구절에서 findwords를 전부 찾아내고 앞에 []를 붙어야 됨
        for(j=0;;j++){
            if(tmp_buf[j]==0) break;
            else if(tmp_buf[j]==' '){

            }
        }
        char printbuffer[MAXBUFF];
        sprintf(printbuffer,"%s:%s%s\n",name,target,tmp_buf);
        //printf("%s",printbuffer);
        if(!write(connfd,printbuffer, strlen(printbuffer)))
            printf("client write fail\n");
    }
    //printf("\n");
    return 0;
}

void parser(char *buf, int* chp, int* ver, int* loc){
    int sum=0;
    char tmp[10]={0,};
    sum+=get_word_buffer(buf,tmp,':', sum);
    sum++;
    *chp=atoi(tmp);
    sum+=get_word_buffer(buf,tmp,':', sum);
    sum++;
    *ver=atoi(tmp);
    sum+=get_word_buffer(buf,tmp,':', sum);
    sum++;
    *loc=atoi(tmp);

}
int get_word_buffer(char *src, char *dst, char dis, int offset){
    char tmp=0;
	bzero((char *)dst, strlen(dst));
	for(int i=0;;i++){
		tmp = src[i+offset];
		if(tmp==dis){
			dst[i]=0;
			return i;
		}
        else if(tmp==0 || tmp=='\n') return -1;
        else{
            dst[i]=src[i+offset];
        }
	}
	return 0;
}
int get_word(int fd,char *buf, char dis){
    char tmp=0;
	bzero((char *)buf, strlen(buf));
	for(int i=0;;i++){
		int test=read(fd,&tmp,1);
        if(test==0){
            return 0;
        }
		if(tmp==dis){
			buf[i]=0;
			return 1;
		}
        else{
            buf[i]=tmp;
        }
	}
	return -1;
}
int my_fgets(int fd, char *buf){
	char tmp=0;
	bzero((char *)buf, strlen(buf));
	for(int i=0;;i++){
		int test=read(fd,&tmp,1);
		if(test==0){
			buf[i]=0;
			return 0;
		}
		else if(tmp=='\n'){
			buf[i]=0;
			return 1;
		}
        else{
            buf[i]=tmp;
        }
	}
	return 2;
}
int initlegacy(legacy* new){
    new->chapter=0;
    new->verse=0;
    new->next=NULL;
    new->prev=NULL;
    return 0;
}
int legacyfree(legacy* start){
    legacy* cur=start;
    legacy *prev=cur;
    while(cur){
        prev=cur;
        cur=cur->next;
    }
    while(prev){
        prev=prev->prev;
        if(prev==NULL)break;
        cur=prev->next;
        //printf("free %d:%d, %x\n",prev->chapter, prev->verse, cur);
        free(cur);
    }
    return 0;
}
//같으면 1, 다르면 0
extern inline int legacycmp(legacy* l1, legacy* l2){
    if(l1->chapter==l2->chapter && l1->verse==l2->verse) return 1;
    else return 0;
}