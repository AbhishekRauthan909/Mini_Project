/*
Author:Abhishek Rauthan MT2023106
File Description:Client program for academia registration
*/
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/ip.h>
#include <string.h> 
#include<stdlib.h>
void connection_handler(int sockFD)
{
    char rbuffer[1000], wbuffer[1000]; // A buffer used for reading from / writting to the server
    ssize_t readBytes, writeBytes;            // Number of bytes read from / written to the socket
    char tempBuffer[1000];
    do
    {
        bzero(rbuffer,sizeof(rbuffer)); // Empty the read buffer
        bzero(tempBuffer, sizeof(tempBuffer));
        readBytes = read(sockFD,rbuffer,sizeof(rbuffer));
        if (readBytes == -1)
            perror("Error while reading from client socket!");
        else if (readBytes == 0)
            printf("No error received from server! Closing the connection to the server now!\n");
        else if (strchr(rbuffer,'^') != NULL)
        {
            // Skip read from client
            strncpy(tempBuffer, rbuffer, strlen(rbuffer) - 1);
            printf("%s\n", tempBuffer);
            writeBytes = write(sockFD, "^", strlen("^"));
            if (writeBytes == -1)
            {
                perror("Error while writing to client socket!");
                break;
            }
        }
        else if (strchr(rbuffer,'$') != NULL)
        {
            // Server sent an error message and is now closing it's end of the connection
            strncpy(tempBuffer,rbuffer,strlen(rbuffer) - 2);
            printf("%s\n", tempBuffer);
            printf("Closing the connection to the server now!\n");
            break;
        }
        else
        {
            bzero(wbuffer,sizeof(wbuffer)); // Empty the write buffer
            if (strchr(rbuffer,'#') != NULL)
                strcpy(wbuffer, getpass(rbuffer));
            else
            {
                printf("%s\n", rbuffer);
                scanf("%[^\n]%*c", wbuffer); // Take user input!
            }

            writeBytes = write(sockFD,wbuffer,strlen(wbuffer));
            if (writeBytes == -1)
            {
                perror("Error while writing to client socket!");
                printf("Closing the connection to the server now!\n");
                break;
            }
        }
    } while (readBytes > 0);

    close(sockFD);
}
int main()
{
    int socket_fd;
    struct sockaddr_in saddress;
    socket_fd=socket(AF_INET,SOCK_STREAM,0);
    if(socket_fd==-1)
    {
        printf("There is an error while creating the socket");
        exit(0);
    }
    saddress.sin_family = AF_INET;                
    saddress.sin_port = htons(8085);              
    saddress.sin_addr.s_addr = htonl(INADDR_ANY);
    int cstatus=connect(socket_fd, (struct sockaddr *)&saddress,sizeof(saddress));
    if(cstatus==-1)
    {
        printf("There is an error while connecting to the server");
        exit(1);
    }
    connection_handler(socket_fd);
    close(socket_fd);
}
