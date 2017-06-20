/*
	C ECHO client example using sockets
*/
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include <sys/time.h>

int try_send(int sock, char* message, int sequence, int time_execution);
 
int main(int argc , char *argv[])
{
	int sock, recv_size;
	struct sockaddr_in server;
	char message[1000];
	 
	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	 
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( 8888 );
 
	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("connect failed. Error");
		return 1;
	}
	 
	puts("Connected\n");
	
	struct timeval timeout;
	 timeout.tv_sec = 10;
	 timeout.tv_usec = 0;
	
	//keep communicating with server
	while(1)
	{
		printf("Enter message : ");
		fgets(message, 100, stdin);

		int r = try_send(sock, message, 1234, 1);
		
		if(r != 0) {
			
			break;
		}
	}
	 
	close(sock);
	return 0;
}

int try_send(int sock, char* message, int sequence, int time_execution) {

	int len = strlen(message);
	int recv_size;
	char buf[120], server_reply[2000];

	sprintf(buf, "%d_%d", sequence, len);

	if(send(sock, buf, strlen(buf) , 0) >= 0) {
			
		struct timeval timeout;
		timeout.tv_sec = 1 / 2;
		timeout.tv_usec = 0;
 
		fd_set in_set;

		FD_ZERO(&in_set);
		FD_SET(sock, &in_set);

		// select the set
		int cnt = select(sock + 1, &in_set, NULL, NULL, &timeout);
		
		if (FD_ISSET(sock, &in_set)) {

			//Receive a reply from the server
			if((recv_size = recv(sock , server_reply , 2000 , 0)) >= 0) {

				if(send(sock, message, strlen(message) , 0) >= 0) {
				memset(server_reply, 0, sizeof server_reply);
					if(recv(sock, server_reply , 2000 , 0) >= 0) {
					
						puts("Server reply :");
						puts(server_reply);
						return 0;
					} else {
						
						puts("error receive message");
						
						return 1;
					}
				} else {
					
					puts("error send message");
					
					return 1;
				}
			} else {
			
				puts("recv failed" + recv_size);
				return recv_size;
			}
		} else {
			
			printf("tentativa %d\n", time_execution);
			
			return try_send(sock, message, sequence, time_execution + 1);
		}
	} else {

		puts("Send failed");
		return 1;
	}
}