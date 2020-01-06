#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define MAXLINE 8000

int main (int argc, char *argv[]) {

    if(argc<2){
        printf("need more argument!\n");
        exit(0);
    }
    int n, cfd;
    struct hostent *h; //information of host(name,addtype,..)
    struct sockaddr_in saddr;
    char buf[MAXLINE];
    char *host = argv[1];
    int port = atoi(argv[2]); //convert string to integer

    if ((cfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket() failed.\n");
        exit(1);
    }

    if ((h = gethostbyname(host)) == NULL) {
        printf("invalid hostname %s\n", host);
        exit(2);
    }

    bzero((char *)&saddr, sizeof(saddr)); // memset()
    saddr.sin_family = AF_INET;
    bcopy((char *)h->h_addr, (char *)&saddr.sin_addr.s_addr, h->h_length); // memcpy()

    saddr.sin_port = htons(port); //Convert host byte-aligned 2-byte datato network byte-aligned

    if (connect(cfd,(struct sockaddr *)&saddr,sizeof(saddr)) < 0) {
        printf("connect() failed.\n");
        exit(3);
    }

    while (1) {
        if(write(1,"> ",2)){}

        if((n = read(0, buf, MAXLINE)) <= 0){
            printf("break client!\n");
            break;
        }
        buf[n]=0;
        if(!strcmp(buf,"!q\n")){
            break;
        }
        if(!write(cfd, buf, n)){
            printf("client write failed!");
        }
        int flag=0;

        while(1){
            char tmp=0;
            n = read(cfd, &tmp, 1);
            if(tmp=='\n'){
                flag++;
                if(flag>2){
                    break;
                }
            }
            else {flag=0;}
            if(!write(1, &tmp, 1)){
                printf("client write to stdin failed");
            }
        }
    }
    
    close(cfd);
}