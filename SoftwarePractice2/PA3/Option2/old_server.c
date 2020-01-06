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
#define MAXCLIENT 10
#define MAXQUEUE 10

typedef struct filedescripter{
    int bible;
    int index;
    char * bibleNm;
    struct filedescripter * next;
}FDs;

//전역변수
FDs * fd_index;
int num_of_word;
char *findwords[10];
int fd_bible;
int count;

int q[MAXQUEUE];
int front;
int rear;

//프로토타입
int connFunction(int connfd,char * input);
int wordfinder(int connfd, int mod);
int my_fgets(int fd, char *buf);
int get_word_buffer(char *src, char *dst, char dis, int offset);
int get_word(int fd,char *buf, char dis);
int append(char *src, char *dest, int bound);
char* strlwr(char * src);
char* cuttxt(char *bible);
void sig_handler();

int main (int argc, char *argv[]) {
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
        new->bibleNm=cuttxt(tmp_buf);
        new->bibleNm=strlwr(new->bibleNm);
        if(fd_index==NULL){
            fd_index=new;
            cur=fd_index;
        }
        else{
            cur->next=new;
            cur=cur->next;
        }
    }

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




    pid_t pid[10]={0,};

    signal(SIGCHLD, (void *)sig_handler);
    while (1) {
        caddrlen = sizeof(caddr);
        if ((connfd = accept(listenfd, (struct sockaddr *)&caddr, &caddrlen)) < 0) {
            printf ("accept() failed.\n");
            continue;
        }
        h = gethostbyaddr((const char *)&caddr.sin_addr.s_addr,sizeof(caddr.sin_addr.s_addr), AF_INET);
        printf("server connected to %s (%s)\n", h->h_name, inet_ntoa(*(struct in_addr *)&caddr.sin_addr));
        if(count>MAXCLIENT){
            printf("client is full!\n");
            continue;
        }
        //pid[count++]=fork();
        pid[count]=fork();
        if(pid[count]==0){
            while ((n = read(connfd, buf, MAXLINE)) > 0) {
                buf[n]=0;
                connFunction(connfd,buf);
                bzero((char *)buf, sizeof(buf));
                n=write(connfd,"\n\n\n",3);
                if(n==0)
                    printf("write() failed\n");
                //write(connfd, buf, n);
            }
            
            printf("connection terminated.\n");
            close(connfd);
            exit(0);
        }
        else{
            count++;
            printf("%d is count\n",count);
        }
    }
    close(connfd);
}
int connFunction(int connfd, char *input){
    int l=strlen(input);
    if(write(connfd,input,l)==0){
        printf("write failed!\n");
    }
    return 0;
}
/*int connFunction(int connfd, char *input){
    int i;
    char tmp_buf[100]={0,};
    int mod=0;
    //"으로 시작할때
    if(input[0]==34){
        mod=1;
        int sum=0;
        for(i=0;;i++){
            int test;
            test=get_word_buffer(input+1,tmp_buf,' ', sum);
            sum=sum+test+1;
            if(!strcmp(tmp_buf,"*")){
                mod++;
                i--;
                continue;
            }
            int len=strlen(tmp_buf);
            findwords[i]=(char*)malloc(sizeof(char)*len);
            strcpy(findwords[i],tmp_buf);
            if(test==-1) break;
        }
        num_of_word=i+1;
        int l=strlen(findwords[i]);
        findwords[i][l-1]=0;
        wordfinder(connfd, mod);
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
        wordfinder(connfd, mod);
    }
    for(i=0;i<10;i++){
        if(findwords[i]==0) break;
        free(findwords[i]);
    }
    return 1;
}*/

int wordfinder(int connfd, int mod){
    int i,j;
    char tmp_buf[MAXBUFF]={0,};
    char print_buffer[MAXBUFF]={0};
    FDs *cur=fd_index;
    int _flag=0, print_flag=0;
    int b_flag=0;

    while(cur){
        lseek(cur->bible,0,SEEK_SET);
        for(i=0;;i++){
            if(!my_fgets(cur->bible,tmp_buf)){
                break;
            }
            if(i==0) continue;
            else{
                int offset=0;
                char word[100]={0,};
                char *tmp_word;
                int m2_flag=0;

                append(print_buffer,cur->bibleNm,MAXBUFF);
                append(print_buffer,":",MAXBUFF);
                while(1){
                    int test=0;
                    test=get_word_buffer(tmp_buf,word,' ', offset);
                    if(test==-1) b_flag=1;
                    offset=offset+test+1;
                    tmp_word=strlwr(word);
                    if(mod==0){
                        // 분리된 단어 여러개 찾기
                        for(j=0;j<num_of_word;j++){
                            if(!strcmp(findwords[j],tmp_word)){
                                //일치할 때
                                _flag=1;
                                print_flag=1;
                            }
                        }
                        if(_flag==1){
                            append(print_buffer, "[", MAXBUFF);
                            append(print_buffer,word,MAXBUFF);
                            append(print_buffer,"] ",MAXBUFF);
                            _flag=0;
                        }
                        else{
                            append(print_buffer,word,MAXBUFF);
                            append(print_buffer," ",MAXBUFF);
                        }
                    }
                    else if(mod==1){
                        //여러 단어가 이어진 구절 찾기
                        if(!strcmp(findwords[0],tmp_word)){
                            char t[100]={0,};
                            get_word_buffer(tmp_buf,t,' ', offset);
                            tmp_word=strlwr(word);
                            if(!strcmp(t, findwords[1])){
                                append(print_buffer,"[",MAXBUFF);
                                append(print_buffer,word,MAXBUFF);
                                append(print_buffer," ",MAXBUFF);
                                append(print_buffer,t,MAXBUFF);
                                append(print_buffer,"] ",MAXBUFF);
                                print_flag=1;
                            }
                            else{
                                append(print_buffer,word,MAXBUFF);
                                append(print_buffer," ",MAXBUFF);
                            }
                        }
                        else{
                            append(print_buffer,word,MAXBUFF);
                            append(print_buffer," ",MAXBUFF);
                        }
                    }
                    else if(mod==2){
                        // god * create
                        if(m2_flag==0){
                            if(!strcmp(findwords[0],tmp_word)){
                                append(print_buffer,"[",MAXBUFF);
                                append(print_buffer,word,MAXBUFF);
                                append(print_buffer," ",MAXBUFF);
                                if(test==-1) b_flag=1;
                                m2_flag=1;
                            }
                            else{
                                append(print_buffer,word,MAXBUFF);
                                append(print_buffer," ",MAXBUFF);
                            }
                        }
                        else if(m2_flag==1){
                            if(!strcmp(findwords[1],tmp_word)){
                                append(print_buffer,word,MAXBUFF);
                                append(print_buffer,"] ",MAXBUFF);
                                if(test==-1) b_flag=1;
                                print_flag=1;
                                m2_flag=0;
                            }
                            else{
                                append(print_buffer,word,MAXBUFF);
                                append(print_buffer," ",MAXBUFF);
                            }
                        }
                    }
                    if(b_flag==1){
                        b_flag=0;
                        break;
                    }
                }
            }
            if(print_flag==1){
                append(print_buffer,"\n",MAXBUFF);
                if(!write(connfd,print_buffer,strlen(print_buffer))){
                    printf("server write failed\n");
                }
                print_flag=0;
            }
            bzero((char*)print_buffer,strlen(print_buffer));
        }
        cur=cur->next;
        _flag=0;
    }
    
    return 0;
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
int append(char *src, char* dest, int bound){
    int l1=strlen(src);
    int l2=strlen(dest);
    int i;
    for(i=l1;i<l1+l2;i++){
        if(i>bound) return -1;
        src[i]=dest[i-l1];
    }
    return 1;
}
char* strlwr(char * src){
    char * dst;
    int l = strlen(src);
    dst=(char*)malloc(sizeof(char)*l);
    bzero((char*)dst, strlen(dst));
    for(int i=0;i<l;i++){
        if(src[i]>='A' && src[i]<='Z'){
            dst[i]=src[i]-'A'+'a';
        }
        else if(src[i]==',' || src[i]=='.'|| src[i]==';'){
            dst[i]=0;
            return dst;
        }
        else
            dst[i]=src[i];
    }
    return dst;
}
char* cuttxt(char *bible){
    int i;
    char * bibleNm=(char*)malloc(sizeof(char)*(strlen(bible)-4));
    for(i=0;;i++){
        if(bible[i]=='.'){
            bibleNm[i]=0;
            return bibleNm;
        }
        else
            bibleNm[i]=bible[i];
    }
    return NULL;
}
void sig_handler()
{
  count--;
}