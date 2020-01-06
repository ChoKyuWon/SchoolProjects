#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>

#define NTHREADS 10
#define MAXSBUF 10
#define MAXSIZE 80
#define MAXLINE 80
#define MAXBUFF 30000

typedef struct{
	int *buf;
	int n;
	int front;
	int rear;
	int count;
	pthread_mutex_t lock; //access control
	pthread_cond_t notfull;
	pthread_cond_t notempty;
} sbuf_t;

typedef struct filedescripter{
    int bible;
    int index;
    char * bibleNm;
    struct filedescripter * next;
}FDs;

FDs * fd_index;
int fd_bible;
sbuf_t sbuf;

//프로토타입
int connFunction(int connfd,char * input);
int wordfinder(int connfd, int mod, int num_of_word, char **findwords);
int my_fgets(int fd, char *buf);
int get_word_buffer(char *src, char *dst, char dis, int offset);
int get_word(int fd,char *buf, char dis);
int append(char *src, char *dest, int bound);
char* strlwr(char * src);
char* cuttxt(char *bible);
void *thread();

void sbuf_init(sbuf_t *sp, int n){
	pthread_mutex_init (&sp->lock, (pthread_mutexattr_t *)NULL);
	pthread_cond_init (&sp->notempty, (pthread_condattr_t *)NULL);
	pthread_cond_init (&sp->notfull, (pthread_condattr_t *)NULL);

	sp->n=n;
	sp->buf=(int*)malloc(sizeof(int)*n);
	sp->count=0;
	sp->front=0;
	sp->rear=0;
};

int main(int argc, char **argv){
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

	int listenfd, port;
	int connfd;
	socklen_t clientlen = sizeof(struct sockaddr_in);
	struct sockaddr_in clientaddr, saddr;
	pthread_t tid;

	if(argc!=2)
	{
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(0);
	}

	port = atoi(argv[1]);
	sbuf_init(&sbuf, MAXSBUF);

	//socket creation
	if((listenfd = socket(AF_INET, SOCK_STREAM, 0))<0)
	{
		printf("socket() failed.\n");
		exit(1);
	}

	bzero((char *)&saddr, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(port);

	//bind
	if(bind(listenfd, (struct sockaddr *)&saddr,  sizeof(saddr))<0)
	{
		printf("bind() failed.\n");
		exit(2);
	}

	//listen
	if(listen(listenfd, 5)<0)
	{
		printf("listen() failed.\n");
		exit(3);
	}

	//pthread creation
	for(i=0; i<NTHREADS; i++)
		pthread_create(&tid, NULL, thread, NULL);

	while(1)
	{
		if((connfd  = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen))<0)
		{
			printf("accept() failed.\n");
			continue;
		}

		//printf("%d is connected\n",connfd);
		//큐에 fd coonfd 집어넣기
		pthread_mutex_lock(&sbuf.lock);

		if(sbuf.count==MAXSBUF){
			printf("sbuf is full!\n");
			pthread_cond_wait(&sbuf.notfull, &sbuf.lock);
		}
		sbuf.buf[sbuf.rear++]=connfd;
		sbuf.count++;
		printf("%d is front, %d is rear, %d is count\n",sbuf.front,sbuf.rear, sbuf.count);
		if(sbuf.rear>=MAXSBUF)
			sbuf.rear-=MAXSBUF;

		pthread_cond_signal(&sbuf.notempty);
		pthread_mutex_unlock(&sbuf.lock);
		
	}
}

void *thread()
{
	char buf[MAXSIZE]={0};
	int n;
	int fd;
	while(1){
		pthread_mutex_lock(&sbuf.lock);
		if(sbuf.count==0){
			pthread_cond_wait(&sbuf.notempty,&sbuf.lock);
		}
		sbuf.count--;
		fd=sbuf.buf[sbuf.front++];
		printf("in q, got %d!\n",fd);
		printf("%d is front, %d is rear, %d is count\n",sbuf.front,sbuf.rear, sbuf.count);
		if(sbuf.front>=MAXSBUF){
			sbuf.front-=MAXSBUF;
		}
		pthread_mutex_unlock(&sbuf.lock);
		
		while ((n = read(fd, buf, MAXLINE)) > 0) {
            buf[n]=0;
            connFunction(fd,buf);
            bzero((char *)buf, sizeof(buf));
            n=write(fd,"\n\n\n",3);
            if(n==0)
            	printf("write() failed\n");
        }
		pthread_cond_signal(&sbuf.notfull);
        printf("connection terminated.\n");
        close(fd);
	}
}

int connFunction(int connfd, char *input){
    int i;
    char tmp_buf[100]={0,};
    int mod=0;
    int num_of_word;
    char *findwords[10];
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
        wordfinder(connfd, mod, num_of_word, findwords);
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
        wordfinder(connfd, mod, num_of_word,findwords);
    }
    for(int k=0;k<num_of_word;k++){
            printf("%s  ",findwords[k]);
        }
    printf(", %d is num of word\n", num_of_word);
    for(i=0;i<num_of_word;i++){
        if(findwords[i]==0) break;
        free(findwords[i]);
    }
    return 1;
}

int wordfinder(int connfd, int mod, int num_of_word, char **findwords){
    int i,j;
    char tmp_buf[MAXBUFF]={0,};
    char print_buffer[MAXBUFF]={0};
    char tmp_print_buffer[MAXBUFF]={0,};
    FDs *cur=fd_index;
    int _flag=0, print_flag=0;
    int b_flag=0;
    int word_check_array[10]={0,};

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
                                word_check_array[j]++;
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
                                printf("m2 flag is 1!\n");
                                strcpy(tmp_print_buffer,print_buffer);
                                printf("%s\n",tmp_print_buffer);
                                append(print_buffer,word,MAXBUFF);
                                append(print_buffer,"] ",MAXBUFF);
                                if(test==-1) b_flag=1;
                                print_flag=1;
                                m2_flag=2;
                            }
                            else{
                                append(print_buffer,word,MAXBUFF);
                                append(print_buffer," ",MAXBUFF);
                            }
                        }
                        else if(m2_flag==2){
                            if(!strcmp(findwords[1],tmp_word)){
                                printf("m2 flag is 2!\n");
                                strcpy(print_buffer,tmp_print_buffer);
                                strcpy(tmp_print_buffer,print_buffer);
                                append(print_buffer,word,MAXBUFF);
                                append(print_buffer,"] ",MAXBUFF);
                                if(test==-1) b_flag=1;
                            }
                            else{
                                append(print_buffer,word,MAXBUFF);
                                append(print_buffer," ",MAXBUFF);
                            }
                            append(tmp_print_buffer,word,MAXBUFF);
                            append(tmp_print_buffer," ",MAXBUFF);
                        }
                    }
                    if(b_flag==1){
                        b_flag=0;
                        break;
                    }
                }
            }
            if(mod==0){
                for(int k=0;k<num_of_word;k++){
                    if(word_check_array[k]==0){
                        print_flag=0;
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
            bzero((char*)tmp_print_buffer,strlen(tmp_print_buffer));
            for(int z=0;z<num_of_word;z++){
                word_check_array[z]=0;
            }
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