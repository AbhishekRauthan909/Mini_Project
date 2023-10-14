#ifndef ADMIN
#define ADMIN
#include "./constants.h"
#include "/Users/abhishekrauthan/Documents/Project/structure/student.h"
#include "/Users/abhishekrauthan/Documents/Project/structure/faculty.h"
#define LOGIN_ID_REAL "Abhishek"
#define PASSWORD "123456"
bool admin_operation_handler(int connectfd);
int add_customer(int connectfd);
bool login_handler(int connectfd)
{
    ssize_t rbytes,wbytes;
    char rbuffer[1000],wbuffer[1000],tempbuffer[1000];
    bzero(rbuffer,sizeof(rbuffer));
    bzero(wbuffer,sizeof(wbuffer));
    strcpy(wbuffer,LOGIN_WELCOME);
    strcat(wbuffer,"\n");
    strcat(wbuffer,LOGIN_ID);
    wbytes=write(connectfd,wbuffer,strlen(wbuffer));
    if(wbytes==-1)
    {
        perror("Error writing message to the client!");
        return false;
    }
    rbytes=read(connectfd,rbuffer,sizeof(rbuffer));
    if(rbytes==-1)
    {
        perror("Error reading login ID from client!");
        return false;
    }
    if(strcmp(rbuffer,LOGIN_ID_REAL)!=0)
    {
        bzero(wbuffer,sizeof(wbuffer));
        wbytes=write(connectfd,LOGIN_ID_DOESNT_EXIT,strlen(LOGIN_ID_DOESNT_EXIT));
        return false;
    }
    bzero(rbuffer,sizeof(rbuffer));
    wbytes=write(connectfd,IN_PASSWORD,strlen(IN_PASSWORD));
    if (wbytes==-1)
    {
        perror("Error writing message to client!");
        return false;
    }
    bzero(rbuffer,sizeof(rbuffer));
    rbytes=read(connectfd,rbuffer,sizeof(rbuffer));
    if(rbytes==-1)
    {
        perror("Error in reading message from client");
        return false;
    }
    if(strcmp(rbuffer,PASSWORD)==0)
    {
        return true;
    }
    bzero(wbuffer,sizeof(wbuffer));
    wbytes=write(connectfd,INVALID_LOGIN,strlen(INVALID_LOGIN));
    return false;
}
int add_student(int connect_fd)
{
    ssize_t rb,wb;//this is for read bytes and write bytes
    char rbuffer[1000],wbuffer[1000];
    struct student new_st,pre_st;
    int cfd=open("student_record.txt",O_RDWR);
    if (cfd==-1&&errno==ENOENT)//if file does not exit means this is the first student
    {
        new_st.id = 0;
    }
    else if(cfd==-1)
    {
        perror("\nThere is an error while opening the student file");
        return -1;
    }
    else
    {
        int offset =lseek(cfd,-sizeof(struct student),SEEK_END);
        if(offset==-1)
        {
            perror("There is while seeking the last student record!");
            return 0;
        }
        rb=read(cfd,&pre_st,sizeof(struct student));
        if (rb==-1)
        {
            perror("There is an error while reading the record!");
            return 0;
        }
        close(cfd);
        new_st.id=pre_st.id+1;
    }
    sprintf(wbuffer,"%s",ADD_NAME);
    wb=write(connect_fd,wbuffer,sizeof(wbuffer));
    if(wb==-1)
    {
        perror("There is an error while writing!");
        return 0;
    }
    rb=read(connect_fd,rbuffer,sizeof(rbuffer));
    if(rb==-1)
    {
        perror("There is an error while reading student name!");
        return 0;
    }
    strcpy(new_st.name,rbuffer);
    wb=write(connect_fd,ADD_GENDER,strlen(ADD_GENDER));
    if (wb==-1)
    {
        perror("There is an error while writing the message!");
        return 0;
    }
    bzero(rbuffer,sizeof(rbuffer));
    rb=read(connect_fd,rbuffer,sizeof(rbuffer));
    if(rb==-1)
    {
        perror("There is an error while reading!");
        return 0;
    }
    if(rbuffer[0]=='M'||rbuffer[0]=='F'||rbuffer[0]=='O')
        new_st.gender=rbuffer[0];
    else
    {
        wb=write(connect_fd,WRONG_GENDER,strlen(WRONG_GENDER));
        rb=read(connect_fd,rbuffer,sizeof(rbuffer));
        return 0;
    }
    bzero(wbuffer,sizeof(wbuffer));
    strcpy(wbuffer,ADD_AGE);
    wb=write(connect_fd,wbuffer,strlen(wbuffer));
    if (wb==-1)
    {
        perror("There is an error on writing!");
        return 0;
    }
    bzero(rbuffer,sizeof(rbuffer));
    rb=read(connect_fd,rbuffer,sizeof(rbuffer));
    if (rb==-1)
    {
        perror("There is an error while reading message from client!");
        return 0;
    }
    int student_age=atoi(rbuffer);
    new_st.age=student_age;
    strcpy(new_st.login,new_st.name);
    strcat(new_st.login,"-");
    sprintf(wbuffer,"%d",new_st.id);
    strcat(new_st.login,wbuffer);
    char hashedPassword[1000];
    strcpy(hashedPassword,crypt("iiitb","666"));
    strcpy(new_st.password,hashedPassword);
    cfd=open("student_record.txt",O_CREAT|O_APPEND|O_RDWR,S_IRWXU);
    if (cfd==-1)
    {
        perror("There is an error in file opening!");
        return 0;
    }
    new_st.status=1;//activate by default
    wb=write(cfd,&new_st,sizeof(new_st));
    if (wb==-1)
    {
        perror("There is an error while writing record to file!");
        return 0;
    }
    close(cfd);
    bzero(wbuffer,sizeof(wbuffer));
    sprintf(wbuffer,"%s\n%s",new_st.login,new_st.password);
    strcat(wbuffer,"^");
    wb=write(connect_fd,wbuffer,strlen(wbuffer));
    if (wb==-1)
    {
        perror("Error sending customer loginID and password to the client!");
        return 0;
    }
    rb=read(connect_fd,rbuffer,sizeof(rbuffer));
    return 1;
}

int add_faculty(int connect_fd)
{
    ssize_t rb,wb;//this is for read bytes and write bytes
    char rbuffer[1000],wbuffer[1000];
    struct student new_ft,pre_ft;
    int cfd=open("faculty_record.txt",O_RDWR);
    if (cfd==-1&&errno==ENOENT)//if file does not exit means this is the first faculty
    {
        new_ft.id = 0;
    }
    else if(cfd==-1)
    {
        perror("\nThere is an error while opening the faculty file");
        return -1;
    }
    else
    {
        int offset =lseek(cfd,-sizeof(struct student),SEEK_END);
        if(offset==-1)
        {
            perror("There is while seeking the last student record!");
            return 0;
        }
        rb=read(cfd,&pre_ft,sizeof(struct faculty));
        if (rb==-1)
        {
            perror("There is an error while reading the record!");
            return 0;
        }
        close(cfd);
        new_ft.id=pre_ft.id+1;
    }
    sprintf(wbuffer,"%s",ADD_NAME);
    wb=write(connect_fd,wbuffer,sizeof(wbuffer));
    if(wb==-1)
    {
        perror("There is an error while writing!");
        return 0;
    }
    rb=read(connect_fd,rbuffer,sizeof(rbuffer));
    if(rb==-1)
    {
        perror("There is an error while reading student name!");
        return 0;
    }
    strcpy(new_ft.name,rbuffer);
    wb=write(connect_fd,ADD_GENDER,strlen(ADD_GENDER));
    if (wb==-1)
    {
        perror("There is an error while writing the message!");
        return 0;
    }
    bzero(rbuffer,sizeof(rbuffer));
    rb=read(connect_fd,rbuffer,sizeof(rbuffer));
    if(rb==-1)
    {
        perror("There is an error while reading!");
        return 0;
    }
    if(rbuffer[0]=='M'||rbuffer[0]=='F'||rbuffer[0]=='O')
        new_ft.gender=rbuffer[0];
    else
    {
        wb=write(connect_fd,WRONG_GENDER,strlen(WRONG_GENDER));
        rb=read(connect_fd,rbuffer,sizeof(rbuffer));
        return 0;
    }
    bzero(wbuffer,sizeof(wbuffer));
    strcpy(wbuffer,ADD_AGE);
    wb=write(connect_fd,wbuffer,strlen(wbuffer));
    if (wb==-1)
    {
        perror("There is an error on writing!");
        return 0;
    }
    bzero(rbuffer,sizeof(rbuffer));
    rb=read(connect_fd,rbuffer,sizeof(rbuffer));
    if (rb==-1)
    {
        perror("There is an error while reading message from client!");
        return 0;
    }
    int faculty_age=atoi(rbuffer);
    new_ft.age=faculty_age;
    strcpy(new_ft.login,new_ft.name);
    strcat(new_ft.login,"-");
    sprintf(wbuffer,"%d",new_ft.id);
    strcat(new_ft.login,wbuffer);
    char hashedPassword[1000];
    strcpy(hashedPassword,crypt("iiitb","666"));
    strcpy(new_ft.password,hashedPassword);
    cfd=open("faculty_record.txt",O_CREAT|O_APPEND|O_RDWR,S_IRWXU);
    if (cfd==-1)
    {
        perror("There is an error in file opening!");
        return 0;
    }
    wb=write(cfd,&new_ft,sizeof(new_ft));
    if (wb==-1)
    {
        perror("There is an error while writing record to file!");
        return 0;
    }
    close(cfd);
    bzero(wbuffer,sizeof(wbuffer));
    sprintf(wbuffer,"%s\n%s",new_ft.login,new_ft.password);
    strcat(wbuffer,"^");
    wb=write(connect_fd,wbuffer,strlen(wbuffer));
    if (wb==-1)
    {
        perror("Error sending loginID and password to the client!");
        return 0;
    }
    rb=read(connect_fd,rbuffer,sizeof(rbuffer));
    return 1;
}




bool view_student_details(int connectfd,int id)
{
    ssize_t rb,wb;
    char rbuffer[1000],wbuffer[10000],tempBuffer[1000];
    struct student st;
    int stfd;
    struct flock lock = {F_RDLCK, SEEK_SET, 0, sizeof(struct student), getpid()};
    if (id==-1)
    {
        wb=write(connectfd,GET_ID,strlen(GET_ID));
        if (wb==-1)
        {
            perror("There is an error while writing the error message!");
            return false;
        }
        bzero(rbuffer,sizeof(rbuffer));
        rb=read(connectfd,rbuffer,sizeof(rbuffer));
        if (rb==-1)
        {
            perror("There is an Error while getting ID from client!");
            return false;
        }
        id=atoi(rbuffer);
    }
    stfd=open("./student_record.txt",O_RDONLY);
    if (stfd==-1)
    {
        bzero(wbuffer,sizeof(wbuffer));
        strcpy(wbuffer,ID_DOESNT_EXIT);
        strcat(wbuffer,"^");
        wb=write(connectfd,wbuffer,strlen(wbuffer));
        if (wb==-1)
        {
            perror("There is an error while sending message to client!");
            return false;
        }
        rb=read(connectfd,rbuffer,sizeof(rbuffer)); // Dummy read
        return false;
    }
    int offset=lseek(stfd,id*sizeof(struct student),SEEK_SET);
    rb=read(stfd,&st,sizeof(struct student));
    bzero(wbuffer,sizeof(wbuffer));
    sprintf(wbuffer,"Student Details - \n\tID :%d\n\tName : %s\n\tGender : %c\n\tAge: %d\n\tLoginID : %s\n\tStatus : %d",st.id,st.name,st.gender,st.age,st.login,st.status);
    strcat(wbuffer,"\n\nYou'll now be redirected to the main menu...^");
    wb=write(connectfd,wbuffer,strlen(wbuffer));
    if (wb==-1)
    {
        perror("Error writing customer info to client!");
        return false;
    }
    rb=read(connectfd,rbuffer,sizeof(rbuffer)); // Dummy read
    return true;
}


bool view_faculty_details(int connectfd,int id)
{
    ssize_t rb,wb;
    char rbuffer[1000],wbuffer[10000],tempBuffer[1000];
    struct faculty ft;
    int stfd;
    if (id==-1)
    {
        wb=write(connectfd,GET_ID,strlen(GET_ID));
        if (wb==-1)
        {
            perror("There is an error while writing the error message!");
            return false;
        }
        bzero(rbuffer,sizeof(rbuffer));
        rb=read(connectfd,rbuffer,sizeof(rbuffer));
        if (rb==-1)
        {
            perror("There is an Error while getting ID from client!");
            return false;
        }
        id=atoi(rbuffer);
    }
    stfd=open("./faculty_record.txt",O_RDONLY);
    if (stfd==-1)
    {
        bzero(wbuffer,sizeof(wbuffer));
        strcpy(wbuffer,ID_DOESNT_EXIT);
        strcat(wbuffer,"^");
        wb=write(connectfd,wbuffer,strlen(wbuffer));
        if (wb==-1)
        {
            perror("There is an error while sending message to client!");
            return false;
        }
        rb=read(connectfd,rbuffer,sizeof(rbuffer)); // Dummy read
        return false;
    }
    int offset=lseek(stfd,id*sizeof(struct faculty),SEEK_SET);
    rb=read(stfd,&ft,sizeof(struct faculty));
    bzero(wbuffer,sizeof(wbuffer));
    sprintf(wbuffer,"Faculty Details - \n\tID :%d\n\tName : %s\n\tGender : %c\n\tAge: %d\n\tLoginID : %s",ft.id,ft.name,ft.gender,ft.age,ft.login);
    strcat(wbuffer,"\n\nYou'll now be redirected to the main menu...^");
    wb=write(connectfd,wbuffer,strlen(wbuffer));
    if (wb==-1)
    {
        perror("Error writing info to client!");
        return false;
    }
    rb=read(connectfd,rbuffer,sizeof(rbuffer)); // Dummy read
    return false;
}

void activate_student(int connectfd)
{
    ssize_t rb,wb;
    char rbuffer[1000],wbuffer[1000],tempbuffer[1000];
    struct student st;
    int stfd;
    wb=write(connectfd,GET_ID,strlen(GET_ID));
    if(wb==-1)
    {
        perror("There is an error while writing the error message!");
        return;
    }
    bzero(rbuffer,sizeof(rbuffer));
    rb=read(connectfd,rbuffer,sizeof(rbuffer));
    if (rb==-1)
    {
            perror("There is an Error while getting ID from client!");
            return;
    }
    int id=atoi(rbuffer);
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
            return;
        }
        rb=read(connectfd,rbuffer,sizeof(rbuffer)); // Dummy read
        return;
    }
    int offset=lseek(stfd,id*sizeof(struct student),SEEK_SET);
    rb=read(stfd,&st,sizeof(struct student));
    bzero(wbuffer,sizeof(wbuffer));
    st.status=1;
    offset=lseek(stfd,id*sizeof(struct student),SEEK_SET);
    wb=write(stfd,&st,sizeof(struct student));
    bzero(wbuffer,sizeof(wbuffer));
    sprintf(wbuffer,"%s","\n\nYou'll now be redirected to the main menu...^");
    wb=write(connectfd,wbuffer,strlen(wbuffer));
    if(wb==-1)
    {
        perror("Error to client!");
        return;
    }
    rb=read(connectfd,rbuffer,sizeof(rbuffer)); // Dummy read
    return;
}


void block_student(int connectfd)
{
    ssize_t rb,wb;
    char rbuffer[1000],wbuffer[1000],tempbuffer[1000];
    struct student st;
    int stfd;
    wb=write(connectfd,GET_ID,strlen(GET_ID));
    if(wb==-1)
    {
        perror("There is an error while writing the error message!");
        return;
    }
    bzero(rbuffer,sizeof(rbuffer));
    rb=read(connectfd,rbuffer,sizeof(rbuffer));
    if (rb==-1)
    {
            perror("There is an Error while getting ID from client!");
            return;
    }
    int id=atoi(rbuffer);
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
            return;
        }
        rb=read(connectfd,rbuffer,sizeof(rbuffer)); // Dummy read
        return;
    }
    int offset=lseek(stfd,id*sizeof(struct student),SEEK_SET);
    rb=read(stfd,&st,sizeof(struct student));
    bzero(wbuffer,sizeof(wbuffer));
    st.status=0;
    offset=lseek(stfd,id*sizeof(struct student),SEEK_SET);
    wb=write(stfd,&st,sizeof(struct student));
    bzero(wbuffer,sizeof(wbuffer));
    sprintf(wbuffer,"%s","\n\nYou'll now be redirected to the main menu...^");
    wb=write(connectfd,wbuffer,strlen(wbuffer));
    if(wb==-1)
    {
        perror("Error to client!");
        return;
    }
    rb=read(connectfd,rbuffer,sizeof(rbuffer)); // Dummy read
    return;
}

bool admin_operation_handler(int connectfd)
{
    if (login_handler(connectfd))//if login successfull then we have to do this
    {
        ssize_t wbytes,rbytes;            
        char rbuffer[1000], wbuffer[1000];
        bzero(wbuffer,sizeof(wbuffer));
        while (1)
        {
            strcat(wbuffer,"\n");
            strcat(wbuffer,MENU);
            wbytes=write(connectfd,wbuffer,strlen(wbuffer));
            if(wbytes==-1)
            {
              perror("There is some erron on writing!");
              return false;
            }
              bzero(wbuffer,sizeof(wbuffer));
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
                add_student(connectfd);
                break;
                case 2:
                view_student_details(connectfd,-1);
                break;
                case 3:
                add_faculty(connectfd);
                break;
                case 4:
                view_faculty_details(connectfd,-1);
                break;
                case 5:
                activate_student(connectfd);
                break;
                case 6:
                block_student(connectfd);
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
