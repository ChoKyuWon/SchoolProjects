/*-----------------------------------------------------------
*
* SWE2007: System Software Experiment 2 (Fall 2018)
*
* Skeleton code for PA#1
*
* October 17, 2018.
* Sungkyunkwan University
*
* Student Id   : 2017314875
* Student Name : 조규원
*
*-----------------------------------------------------------
*/


#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct _word{
	 int chapter;
	 int verse;
	 int word_loc;
	 int same_num;
	 char *word;
	 struct _word *same;
	 struct _word *next;
 }WORD;

typedef struct index_tree {
	WORD *first_char[26];
}INDEX_TREE;

typedef struct tree_header{
	INDEX_TREE *root;
	int word_num;
	int index_num;
}HEADER;


int init_word(WORD * new, int chp, int v, int loc,char* word);
int init(HEADER * header);
int addword(HEADER *header, char *word, int chp, int v, int loc);
int my_fgets(int fd, char *buf);
int my_strcmp(char * src, char * dest);
int my_strlen(char * src);
int my_atoi(char * buf);
void my_itoa(int src, char *buffer);
void my_strcpy(char *dest, char*src);
int iscontent(char* buf);
void bufclear(char *buf);
void writeline(WORD* root, int fd);

void indexBuilder(const char* inputFileNm, const char* indexFileNm)
{
	HEADER *header;
	header=(HEADER*)malloc(sizeof(HEADER));
	init(header);
	int max_chapter=0, total_verse=0;
	int fd;
	char *buf;
	int count=0;
	buf=(char*)malloc(sizeof(char)*10000);
	fd = open(inputFileNm, O_RDONLY);
	//for(int asd=0;asd<100;asd++)
	while(1)
	{
		count=0;
		int condition=my_fgets(fd,buf);
		int chapter=0,verse=0;
		if(iscontent(buf)){
			total_verse++;
			char tmp[2000]={0,};
			int index=0;

			for(int i=0;;i++){
				if(buf[i]==':'){
					switch(count){
					case 0:
						chapter=my_atoi(tmp);
						count++;
						bufclear(tmp);
						index=0;
						break;
					case 1:
						index=0;
						count++;
						verse=my_atoi(tmp);
						bufclear(tmp);
						break;
					default: break;
					}
					continue;
				}
				if(buf[i]==' '){
					if(count==2){
						tmp[index++]=buf[i];
						count++;
						continue;
					}
					if(buf[i+1]==' ') continue;
					addword(header,tmp,chapter,verse,i);
					bufclear(tmp);
					index=0;
				}
				tmp[index++]=buf[i];
				if(buf[i]=='\n' || buf[i]==0) break;
			}
		}
		if(condition==0){
			max_chapter=chapter;
			break;
		}
		else continue;
	}
	//scanf("%d",fd);
	for(int i=0;i<26;i++){
		WORD *cur=header->root->first_char[i];
		if(cur==NULL) continue;
		while(1){
			header->index_num++;
			if(cur->next==NULL) break;
			else cur=cur->next;
		}
	}

	int gar=0;
	close(fd);
	fd=open(indexFileNm, O_WRONLY | O_CREAT, 0775);
	char buffer[100]={0,};
	gar=write(fd,inputFileNm,my_strlen((char*)inputFileNm));
	gar=write(fd," : ",sizeof(" : "));

	my_itoa(max_chapter, buffer);
	gar=write(fd, buffer, my_strlen(buffer));
	gar=write(fd,",  ",sizeof(",  "));

	my_itoa(total_verse, buffer);
	gar=write(fd, buffer, my_strlen(buffer));
	gar=write(fd,",  ",sizeof(",  "));

	my_itoa(header->index_num, buffer);
	gar=write(fd, buffer, my_strlen(buffer));
	gar=write(fd,",  ",sizeof(",  "));

	my_itoa(header->word_num, buffer);
	gar=write(fd, buffer, my_strlen(buffer));
	gar=write(fd,"\n",sizeof("\n"));

	for(int i=0;i<26;i++){
		writeline(header->root->first_char[i], fd);
	}
	if(gar) ;
}

void writeline(WORD* root, int fd){
	WORD *cur=root;
	int gar=0;
	char buffer[100]={0,};
	if(cur==NULL) return;
	while(1){
		WORD *curr=cur;
		gar=write(fd, cur->word, my_strlen(cur->word));
		gar=write(fd,": ", sizeof(": "));
		my_itoa(cur->same_num, buffer);
		gar=write(fd, buffer, my_strlen(buffer));
		gar=write(fd,",  ",sizeof(",  "));
		while(1){
			my_itoa(curr->chapter, buffer);
			gar=write(fd, buffer, my_strlen(buffer));
			gar=write(fd,":",sizeof(":"));
			my_itoa(curr->verse, buffer);
			gar=write(fd, buffer, my_strlen(buffer));
			gar=write(fd,":",sizeof(":"));
			my_itoa(cur->word_loc, buffer);
			gar=write(fd, buffer, my_strlen(buffer));
			gar=write(fd,":",sizeof(":"));
			
			if(curr->same==NULL) break;
			else curr=curr->same;
			gar=write(fd,",   ",sizeof(",  "));
		}
		gar=write(fd,"\n",sizeof("\n"));
		if(cur->next==NULL) break;
		else cur=cur->next;
	}
	if(gar) ;
}
int addword(HEADER *header, char *word, int chp, int v, int loc){
	int cha=word[1];
	WORD *new;
	if(cha>='a' && cha<='z') cha-='a';

	WORD *cur=header->root->first_char[cha];
	header->word_num++;
	if(cur==NULL){
		//word init code
		cur=(WORD*)malloc(sizeof(WORD));
		init_word(cur,chp,v,loc, word);
		header->root->first_char[cha]=cur;
		return 0;
	}
	new=(WORD*)malloc(sizeof(WORD));
	init_word(new, chp, v, loc, word);
	while(1){
		if(my_strcmp(cur->word,word)==1){
			new->same=cur->same;
			cur->same=new;
			cur->same_num++;
			return 1;
		}
		if(cur->next==NULL) break;
		cur=cur->next;
	}
	cur->next=new;
	return 1;
}

extern inline int init_word(WORD * new, int chp, int v, int loc, char *word){
	int len=my_strlen(word);
	new->chapter=chp;
	new->verse=v;
	new->word_loc=loc;
	new->same_num=1;
	new->same=NULL;
	new->next=NULL;
	new->word=(char*)malloc(sizeof(char)*len);
	my_strcpy(new->word, word);
	return 1;
}

extern inline int init(HEADER * header){
	header->root=(INDEX_TREE*)malloc(sizeof(INDEX_TREE));
	header->word_num=0;
	header->index_num=0;
	//header->root->first_char=(WORD**)malloc(26*sizeof(WORD*));
	for(int i=0;i<26;i++){
		header->root->first_char[i]=NULL;
	}
	return 1;
}

int my_fgets(int fd, char *buf){
	char tmp=0;
	int gar=0;
	bufclear(buf);
	for(int i=0;;i++){
		int test=read(fd,&tmp,1);

		if(!((tmp>='a' && tmp<='z') || (tmp>='A' && tmp<='Z'))){
			if(tmp==' ' || tmp=='\n' || tmp==':' || tmp=='-' || tmp=='\''){
				//printf("%c",tmp);
				gar++;
			}
			else if(tmp>='0' && tmp<='9')
				//printf("%c",tmp);
				gar++;
			else{
				if(test==0){
					buf[i]=0;
					return 0;
				}
				buf[i]=' ';
				continue;
			}
		}
		if(tmp>='A' && tmp<='Z'){
			tmp+='a';
			tmp-='A';
		}
		else{
			//printf("%c",tmp);
			gar++;
		}
		buf[i]=tmp;
		if(test==0){
			//printf("Last line!\n");
			buf[i]=0;
			return 0;
		}
		else if(buf[i]=='\n'){
			buf[i+1]=0;
			return 1;
		}
		//printf("q");
	}
	return 2;
}

extern inline int my_strcmp(char * src, char * dest){
	if(src==NULL || dest==NULL) return 0;
	if(my_strlen(src)!=my_strlen(dest)) return 0;
	for(int i=0;;i++){
		if(src[i]==0 || dest[i]==0) return 1;
		if(src[i]==dest[i]) continue;
		else return 0;
	}
}

extern inline int my_strlen(char *src){
	for(int i=0;;i++){
		if(src[i]==0) return i;
	}
}

extern inline void my_strcpy(char *dest, char *src){
	int len=my_strlen(src);
	for(int i=0;i<len;i++){
		dest[i]=src[i];
	}
}

extern inline int iscontent(char *buf){
	int count=0;
	for(int i=0;;i++){
		if(buf[i]==0) return 0;
		else if(buf[i]==':'){
			if(count==1) return 1;
			else{
				count++;
				continue;
			}
		}
	}
}

extern inline int my_atoi(char * buf){
	int res=0;
	int len=my_strlen(buf);
	for(int i=len-1;i>=0;i--){
		int tmp=buf[i]-'0';
		for(int j=len-i;j>1;j--){
			tmp*=10;
		}
		res+=tmp;
	}
	return res;
}

extern inline void my_itoa(int src, char *buffer){
	int tmp=src;
	int i,j;
	bufclear(buffer);
	for(i=0;;i++){
		if(tmp==0) break;
		else tmp/=10;
	}
	for(j=i-1;j>=0;j--){
		buffer[j]=src%10+'0';
		src/=10;
	}
}
extern inline void bufclear(char *buf){
	int len=my_strlen(buf);
	for(int i=0;i<len;i++){
		buf[i]=0;
	}
}