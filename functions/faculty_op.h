#ifndef ADMIN_FUN
#define ADMIN_FUN
#include "./constants.h"
#include "/Users/abhishekrauthan/Documents/Project/structure/faculty.h"
bool faculty_operation_handler(int connectfd);
int login_faculty(int connectfd)
{
    ssize_t rb,wb;
    char rbuffer[1000],wbuffer[1000],tempbuffer[1000];
    struct faculty ft;
    int stfd;
    wb=write(connectfd,GET_ID,strlen(GET_ID));
    if(wb==-1)
    {
        perror("There is an error while writing the error message!");
        return -1;
    }
    bzero(rbuffer,sizeof(rbuffer));
    rb=read(connectfd,rbuffer,sizeof(rbuffer));
    if (rb==-1)
    {
            perror("There is an Error while getting ID from client!");
            return -1;
    }
    int id=atoi(rbuffer);
    bzero(wbuffer,sizeof(wbuffer));
    strcpy(wbuffer,IN_PASSWORD);
    wb=write(connectfd,wbuffer,strlen(wbuffer));
    if(wb==-1)
    {
        perror("There is an error while writing the error message!");
        return -1;
    }
    //66naBvad/64sg
    bzero(rbuffer,sizeof(rbuffer));
    rb=read(connectfd,rbuffer,sizeof(rbuffer));
    stfd=open("./faculty_record.txt",O_RDWR,0744);
    if (stfd==-1)
    {
        bzero(wbuffer,sizeof(wbuffer));
        strcpy(wbuffer,ID_DOESNT_EXIT);
        strcat(wbuffer,"^");
        wb=write(connectfd,wbuffer,strlen(wbuffer));
        if (wb==-1)
        {
            perror("There is an error while sending message to client!");
            return -1;
        }
        rb=read(connectfd,rbuffer,sizeof(rbuffer)); // Dummy read
        return -1;
    }
    int offset=lseek(stfd,id*sizeof(struct faculty),SEEK_SET);
    rb=read(stfd,&ft,sizeof(struct faculty));
    close(stfd);
    if(strcmp(rbuffer,ft.password)==0)
    {
        return id;
    }
    bzero(wbuffer,sizeof(wbuffer));
    wb=write(connectfd,INVALID_LOGIN,strlen(INVALID_LOGIN));
    return -1;
}


void change_password(int connectfd,int id)
{
    char rbuffer[1000],wbuffer[1000];
    struct faculty ft;
    bzero(rbuffer,sizeof(rbuffer));
    bzero(wbuffer,sizeof(wbuffer));
    strcpy(wbuffer,NEW_PASSWORD);
    ssize_t wb,rb;
    wb=write(connectfd,wbuffer,strlen(wbuffer));
    if(wb==-1)
    {
        perror("Error writing message to the client!");
        return;
    }
    rb=read(connectfd,rbuffer,sizeof(rbuffer));
    printf("%s",rbuffer);
    if(rb==-1)
    {
        perror("Error reading from client!");
        return;
    }
    int stdfd=open("./faculty_record.txt",O_RDWR);
    int offset=lseek(stdfd,id*sizeof(ft),SEEK_SET);//here from start of the file i want to move my file descriptor
    if(offset==-1)
    {
        printf("Error");
        return;
    }
    rb=read(stdfd,&ft,sizeof(ft));
    if(rb==-1)
    {
        perror("Error while reading!");
        return;
    }
    close(stdfd);
    stdfd=open("./faculty_record.txt",O_RDWR);
    offset=lseek(stdfd,id*sizeof(struct faculty),SEEK_SET);
    strcpy(ft.password,rbuffer);
    write(stdfd,&ft,sizeof(ft));
    close(stdfd);
    wb=write(connectfd,UPDATE_SUCCESS,strlen(UPDATE_SUCCESS));
    if(wb==-1)
    {
        perror("There is an error while writing to client");
        return;
    }
    rb=read(connectfd,rbuffer,sizeof(rbuffer));//dummy read
    return;
}


void faculty_logout_exit(int connectfd)
{
    char wbuffer[1000],rbuffer[1000];
    strcpy(wbuffer,"Logging you out !$");
    write(connectfd,wbuffer,sizeof(wbuffer));
}


bool faculty_operation_handler(int connectfd)
{
    int id=login_faculty(connectfd);
    if (id!=-1)//if login successfull then we have to do this
    {
        ssize_t wbytes,rbytes;            
        char rbuffer[1000], wbuffer[1000];
        bzero(wbuffer,sizeof(wbuffer));
        bzero(rbuffer,sizeof(rbuffer));
        while (1)
        {
            strcat(wbuffer,"\n");
            strcat(wbuffer,FACULTY_MENU);
            wbytes=write(connectfd,wbuffer,strlen(wbuffer));
            if(wbytes==-1)
            {
              perror("There is some erron on writing!");
              return false;
            }
              bzero(wbuffer,sizeof(wbuffer));
              bzero(rbuffer,sizeof(rbuffer));
              rbytes=read(connectfd,rbuffer,sizeof(rbuffer));
              if(rbytes==-1)
             {
                perror("There is an error while reading MENU");
                return false;
             }
            int choice=atoi(rbuffer);
            switch(choice)
            {
                case 1:
                break;
                case 2:
                break;
                case 3:
                break;
                case 4:
                break;
                case 5:
                change_password(connectfd,id);
                break;
                case 6:
                faculty_logout_exit(connectfd);
                break;
            }
        }
    }
    else
    {
        return false;
    }
    return true;
}


#endif
