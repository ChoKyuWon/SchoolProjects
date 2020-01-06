#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

#define MAXLINE 80


int main(int argc, char *argv[])
{
	int n, cfd;
	struct hostent *h;
	struct sockaddr_in saddr;
	char buf[MAXLINE];
	char *host = argv[1];
	int port = atoi(argv[2]);
	char **pp;

	if((cfd = socket(AF_INET, SOCK_STREAM, 0)) <0){
		printf("socket() failed.\n");
		exit(1);
	}

	if((h = gethostbyname(host))==NULL)
	{
		printf("invaild hostname %s\n", host);
		exit(2);
	}
	
	bzero((char*)&saddr, sizeof(saddr)); //memset()
	saddr.sin_family = AF_INET;
	bcopy((char*)h->h_addr, (char*)&saddr.sin_addr.s_addr, h->h_length); //memcopy()
	saddr.sin_port = htons(port);

	if(connect(cfd, (struct sockaddr *)&saddr, sizeof(saddr))<0)
	{
		printf("connect() failed.\n");
		exit(3);
	}

	while((n=read(0,buf,MAXLINE))>0)
	{
		write(cfd, buf, n);
		n = read(cfd, buf, MAXLINE);
		write(1, buf, n);
	}

	close(cfd);
}
