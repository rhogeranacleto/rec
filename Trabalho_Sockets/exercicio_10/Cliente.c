/*
    C ECHO client example using sockets
*/
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
 
int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
     
    //Create socket
    sock = socket(AF_INET , SOCK_DGRAM, IPPROTO_UDP);
    if (sock == -1)
    {
        printf("Could not create socket");
	return 1;
    }

    puts("Socket created");
     
    memset((char *) &server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);
 
     if (inet_aton("127.0.0.1", &server.sin_addr) == 0) 
    {
        printf("Could not create socket");
	return 1;
    }
     
    //keep communicating with server
    while(1)
    {
        printf("Enter message : ");
        fgets(message, 100, stdin);
         
        //Send some data
        if( sendto(sock , message , strlen(message), 0, (struct sockaddr *) &server, sizeof(server)) <= 0)
        {
            puts("Send failed");
            return 1;
        }
         
        //Receive a reply from the server
        if( recvfrom(sock, server_reply, 100, 0, (struct sockaddr *) &server, sizeof(server)) <= 0)
        {
            puts("recv failed");
            break;
        }
         
        puts("Server reply :");
        puts(server_reply);
    }
     
    close(sock);
    return 0;
}
