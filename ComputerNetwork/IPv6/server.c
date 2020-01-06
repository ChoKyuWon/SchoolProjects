#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#define N 1000

int main() {
	int listen_socket, server_socket, clientaddr_len, recv_size;
	struct hostent *h;
	struct sockaddr_in serveraddr, clientaddr;
	char buf[N];
	int port = 50000;

	if((listen_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("Can't creat a socket\n");
		return 1;
	}
	bzero((char *)&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(port);

	if(bind(listen_socket, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0){
		printf("Can't bind name to socket\n");
		return 1;
	}

	listen(listen_socket, 5);

	while (1)
	{
		clientaddr_len = sizeof(clientaddr);
		if((server_socket = accept(listen_socket, (struct sockaddr *)&clientaddr, &clientaddr_len)) < 0){
			printf("Can't accept client\n");
			return 1;
		}
		printf("connect\n");
        if(fork()==0){
            while (1) {
                if ((recv_size = read(server_socket, buf, N)) > 0) {
					FILE *f = fopen("result", "a");
                    printf("recv : %s", buf);
                    fprintf(f, "%s", buf);
					fclose(f);
                }
            }
            close(server_socket);
        }
	}
	close(listen_socket);
	return 0;
}