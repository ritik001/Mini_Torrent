#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
//The in_port_t and in_addr_t types , in_addr structure are defined as in <netinet/in.h>.
#include <arpa/inet.h>

#define PORT 51636
using namespace std;

int main(int argc,char *argv[])
{
    int sockfd;
    int ret;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int opt=1;

    int newSocket;
    struct sockaddr_in new_addr;

    socklen_t addr_size;
    char buffer[1024];
    pid_t childpid;

    sockfd=socket(AF_INET,SOCK_STREAM,0); 
    // Leave protocol up to service provider

    if(sockfd<0){
        printf("Error in connection \n");
        exit(1);
    }
    printf("1. Server socket creation\n");

  //  setsockopt(sockfd,SOL_SOCKET,SO_REUSEPORT,&opt,sizeof(opt));
    memset(&server_addr,'\0',sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(PORT); 
	server_addr.sin_addr.s_addr =inet_addr("127.0.0.1");


    ret=bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if(ret<0)
    {
        printf("Error in binding\n");
        exit(1);
    }
    printf("2. Binded to the port %d\n",PORT);

    if(listen(sockfd,10) == 0)
        printf("3. Listening\n");
    else
        printf("Error in listening\n");
        
    signal(SIGCHLD,SIG_IGN);

    while(1)
    {
        cout << "hello" << "\n";
        newSocket=accept(sockfd,(struct sockaddr*)&new_addr,&addr_size); // why &addr_size
        cout << "hello1" << "\n";
        if(newSocket < 0){
            printf("Connection not accepted \n");
            exit(1);
        }
		printf("4. Connection accepted from %s%d\n",inet_ntoa(new_addr.sin_addr), ntohs(new_addr.sin_port));
//      ntohs :-   u_short from TCP/IP network byte order to host byte order
/*      inet_aton() converts the Internet host address cp from the IPv4 numbers-and-dots notation into binary form (in network byte order) 
        and stores it in the structure that inp points to.*/ 

		if((childpid = fork()) == 0){
			while(1){
				recv(newSocket, buffer, 1024, 0);
				if(strcmp(buffer, ":exit") == 0){
					printf("Disconnected from \n");
					break;
				}else{
					printf("Client: %s\n", buffer);
                    bzero(buffer, sizeof(buffer));
                     scanf("%s", &buffer[0]);
		            send(newSocket, buffer,1024, 0);
				}
			}
            }
        else
        {
            cout << getpid() << "\n";
            cout << getppid << "\n";
            close(sockfd);
        }
		

    }


    close(newSocket);

    return 0;

    

}