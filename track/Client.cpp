#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
//The in_port_t and in_addr_t types , in_addr structure are defined as in <netinet/in.h>.
#include <arpa/inet.h>

#define PORT 61622

int main(){

	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("Error in connection.\n");
		exit(1);
	}
	printf("1. Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

/* 	bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length); */

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("Error in connection.\n");
		exit(1);
	}
	printf("2. Connected to Server %s %d\n",inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));

	while(1){
		printf("Client: \t");
		scanf("%s", &buffer[0]);

		send(clientSocket,buffer,1024, 0);

		if(strcmp(buffer, ":exit") == 0){
			close(clientSocket);
			printf("Disconnected from server.\n");
			exit(1);
		}

		if(recv(clientSocket, buffer, 1024, 0) < 0){
			printf("Error in receiving data.\n");
		}else{
			printf("Server: \t%s\n", buffer);
		}
		bzero(buffer, sizeof(buffer));
	}

return 0;

}