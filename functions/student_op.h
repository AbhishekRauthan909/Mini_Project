#ifndef STUDENT_FUN
#define STUDENT_FUN
#include "./constants.h"
#include "/Users/abhishekrauthan/Documents/Project/structure/student.h"
bool student_operation_handler(int connectfd);
int login_student(int connectfd);
void change_password(int connectfd,int id);
int login_student(int connectfd)
{
    ssize_t rb,wb;
    char rbuffer[1000],wbuffer[1000],tempbuffer[1000];
    struct student ft;
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
    stfd=open("./student_record.txt",O_RDWR);
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
    int offset=lseek(stfd,id*sizeof(struct student),SEEK_SET);
    rb=read(stfd,&ft,sizeof(struct student));
    close(stfd);
    if(strcmp(rbuffer,ft.password)==0)
    {
        return id;
    }
    bzero(wbuffer,sizeof(wbuffer));
    wb=write(connectfd,INVALID_LOGIN,strlen(INVALID_LOGIN));
    return -1;
}


void change_student_password(int connectfd,int id)
{
    ssize_t rb,wb;//this is for counting number of bytes read and write
    char rbuffer[1000],wbuffer[1000];//this is for buffer or to store
    struct student st;//here we are declaring variable of struct type
    int stfd;//student file descriptor
    stfd=open("./student_record.txt",O_RDWR);
    int offset=lseek(stfd,id*sizeof(struct student),SEEK_SET);//means from begining we have to move this
    if(offset==-1)//means there is some error in seeking
    {
        return;
    }
    rb=read(stfd,&st,sizeof(st));//here we are reading that specific record that password we have to change
    if(rb==-1)//means there is some error in reading
    {
        return;
    }
    bzero(wbuffer,sizeof(wbuffer));
    strcpy(wbuffer,NEW_PASSWORD);
    wb=write(connectfd,wbuffer,strlen(wbuffer));
    if(wb==-1)//means there is some error in writing
    {
        return;
    }
    bzero(rbuffer,sizeof(rbuffer));
    rb=read(connectfd,rbuffer,sizeof(rbuffer));//we are here reading new password from client
    if(rb==-1)//measn there is some error in reading from the client
    {
        return;
    }
    printf("\nNa:%s",rbuffer);
    strcpy(st.password,rbuffer);
    offset=lseek(stfd,-sizeof(struct student),SEEK_CUR);//here we are moving back to that record position as after read offset will be set after that record so we are moving it back
    if(offset==-1)//there is some error in setting the offset
    {
        return;
    }
    wb=write(stfd,&st,sizeof(st));//we are writing that record to student record
    if(wb==-1)//there is some error in writing
    {
        return;
    }
    close(stfd);
     wb=write(connectfd,UPDATE_SUCCESS, strlen(UPDATE_SUCCESS));
    if(wb==-1)//there is some error writing this error message
     {
         return;
    }
    rb=read(connectfd,rbuffer,sizeof(rbuffer));//dummy read
    return;
}


void student_logout_exit(int connectfd)
{
    char wbuffer[1000],rbuffer[1000];
    strcpy(wbuffer,"Logging you out !$");
    write(connectfd,wbuffer,sizeof(wbuffer));
}

bool student_operation_handler(int connectfd)
{
    int id=login_student(connectfd);
    if (id!=-1)//if login successfull then we have to do this
    {
        ssize_t wbytes,rbytes;            
        char rbuffer[1000], wbuffer[1000];
        bzero(wbuffer,sizeof(wbuffer));
        bzero(rbuffer,sizeof(rbuffer));
        while (1)
        {
            strcat(wbuffer,"\n");
            strcat(wbuffer,STUDENT_MENU);
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
                change_student_password(connectfd,id);
                break;
                case 6:
                student_logout_exit(connectfd);
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
