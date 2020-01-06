#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h> 
#include <stdlib.h>
#include <stdio.h>

#define MAXLEN 1024
#define MYIP "210.92.20.251"
#define SERVERIP "2001:0:53aa:64c:2c4d:4456:8c6e:5548​"
//#define SERVERIP "2001:0:53aa:64c:1c3a:6a65:3403:dcf1" //skku
#define MYPORT 50001

int RecvnSend(char*, int);
int main(int argc, char **argv)
{
    struct sockaddr_in6 v6;
    char mylong[16];
    char addr6_str[40];
    struct timeval tv;

    int sock;
    sock = socket(AF_INET6, SOCK_STREAM, 0);
    if (sock < 0){
        printf("socket create error.\n");
        return -1;
    }
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    v6.sin6_family = AF_INET6;
    v6.sin6_flowinfo = 0;
    v6.sin6_port = htons(50000);

    //inet_pton(AF_INET6, "2001:0:53aa:64c:2c4d:4456:8c6e:5548​", (void *)&ipv6_addr);
    //inet_pton(AF_INET6, SERVERIP, (void *)&ipv6_addr);
    //inet_pton(AF_INET6, SERVERIP, (void *)&v6.sin6_addr);
    mylong[0] = 0b00100000;
    mylong[1] = 0b00000001;
    mylong[2] = 0b00000000;
    mylong[3] = 0b00000000;
    mylong[4] = 0b01010011;
    mylong[5] = 0b10101010;
    mylong[6] = 0b00000110;
    mylong[7] = 0b01001100;
    mylong[8] = 0b00101100;
    mylong[9] = 0b01001101;
    mylong[10] = 0b01000100;
    mylong[11] = 0b01010110;
    mylong[12] = 0b10001100;
    mylong[13] = 0b01101110;
    mylong[14] = 0b01010101;
    mylong[15] = 0b01001000;
    memcpy((void *)&v6.sin6_addr, (void *)&mylong, 16);
    inet_ntop(AF_INET6, (void *)&v6.sin6_addr, addr6_str, 40);
    int len = sizeof(v6);
    if(connect(sock, (struct sockaddr *)&v6, len) < 0){
        printf("connect error:%d\n", connect(sock, (struct sockaddr *)&v6, len));
        return -1;
    }
    int n = 0;
	char recvbuffer[MAXLEN] = {0,};
    char sendbuffer[MAXLEN] = {0,};
    RecvnSend("2017314875", sock);
    RecvnSend(MYIP, sock);
    RecvnSend("50000", sock);
    RecvnSend("Y", sock);
    RecvnSend("Y", sock);
    RecvnSend("OK", sock);
    sleep(5);

    FILE *f = fopen("result", "r");
    fscanf(f, "%s", sendbuffer);
    printf("sendbuffer1 = %s\n", sendbuffer);
    RecvnSend(sendbuffer, sock);
    memset(sendbuffer, 0, MAXLEN);
    fscanf(f, "%s", sendbuffer);
    printf("sendbuffer2 = %s\n", sendbuffer);
    RecvnSend(sendbuffer, sock);
    fclose(f);
    close(sock);
}

int RecvnSend(char *data, int sock){
    int n;
    char recvbuffer[MAXLEN] = {0, };
    char sendbuffer[MAXLEN] = {0, };
    while((n = read(sock,recvbuffer,MAXLEN)) > 0) {
        printf("%s", recvbuffer);
    }
    strcpy(sendbuffer, data);
    sendbuffer[strlen(sendbuffer)] = 0xa;
    write(sock, sendbuffer, strlen(sendbuffer));
}