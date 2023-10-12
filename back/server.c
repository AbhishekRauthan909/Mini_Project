/*
Author:Abhishek Rauthan MT2023106
File Description:Server program for academia registration
*/
#include<stdio.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/ip.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "/Users/abhishekrauthan/Documents/Project/functionality/admin.h"
void handle_the_connection(int connect_fd)
{
    char rbuffer[1500];
    char wbuffer[1500];
    ssize_t rb;//this is for read bytes
    ssize_t wb;//this is for write bytes
    char initial[]="...............Welcome Back to Acdemia:Course Registration.........................\nLogin type\nEnter your choice {1.Admin,2.Professor,3.Student}!";
    wb=write(connect_fd,initial,sizeof(initial));
    if(wb==-1)
    {
        printf("There is an error while sending the data to client\n");
    }
    else
    {
        bzero(rbuffer,sizeof(rbuffer));
        rb=read(connect_fd,rbuffer,sizeof(rbuffer));
        if(rb==-1)
        {
            printf("There is an error while reading the data from the client\n");
        }
        else if(rb==0)
        {
            printf("Client does not send any data\n");
        }
        else
        {
            int choice=atoi(rbuffer);
            switch(choice)
            {
                case 1:
                admin_impl(connect_fd);
                break;
                case 2://This is for professor
                break;
                case 3://This is for student
                break;
                default:
                exit;
            }
        }
    }
}
int main()
{
    int socket_fd,connect_fd;
    struct sockaddr_in seaddr,cliaddr;
    socket_fd=socket(AF_INET,SOCK_STREAM,0);//here we are creating the socket
    if(socket_fd==-1)
    {
        printf("There is an error while creating socket\n");
        exit(1);
    }
    seaddr.sin_family = AF_INET;              
    seaddr.sin_port = htons(8085); //specifying the port         
    seaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    int sock_bind=bind(socket_fd,(struct sockaddr *)&seaddr,sizeof(seaddr));
    if(sock_bind==-1)
    {
        printf("Socket can't bind to the address try after some time\n");
        exit(1);
    }
    printf("Socket bind successfully to address\n");
    int listen_status;
    listen_status=listen(socket_fd,10);//5 means maximum backlog client
    if(listen_status==-1)
    {
        printf("There is some error in listening");
        exit(1);
    }
    printf("Server is listening now you can ask for connection or you can connect\n");
    int csize;
    while(1)
    {
        csize=(int)sizeof(cliaddr);
        connect_fd=accept(socket_fd,(struct sockaddr *)&cliaddr,&csize);//here we are accepting the connection request from the client
        if(connect_fd==-1)
        {
            printf("There is an error while establishing a connection\n");
            close(connect_fd);
            exit(1);
        }
        else
        {
            printf("Congratulations connection establish successfully\n");
            if(!fork())//this part will be executed by the child
            {
                handle_the_connection(connect_fd);
                printf("Connection between client and socket is terminating as client has performed his operations\n");
                close(connect_fd);
                exit(0);
            }
        }
    }
    close(socket_fd);
}