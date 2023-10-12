#ifndef ADMIN_H
#define ADMIN_H
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#define ADMIN_USERNAME "Abhishek"
#define ADMIN_PASSWORD "2023"
#define get "Enter username"
#define pass "Enter password:\n#"
#define pwdmsg "Password or Username does not match!$"
#define successmsg "Verification Done"
#define ADMIN_MENU "......Welcome to Admin Menu..........\n1.Add Student\n2.View Student Details\n3.Add Faculty\n4.View Faculty Details\n5.Activate Student\n6.Block Student\n7.Modify Student Details\n8.Modify Faculty Details\n9.Logout and Exit\nPress any other key to logout"
#include "/Users/abhishekrauthan/Documents/Project/struct/stud.h"
#include "/Users/abhishekrauthan/Documents/Project/struct/faculty.h"
#define askname "name?"
#define STUDENT_AGE "Age?"
#define askgender "What is the customer's gender?\nEnter M for male, F for female and O for others"
int verify_admin(int connect_fd)
{
    ssize_t rb,wb;
    char ubuffer[1000],wbuffer[1000];
    char pbuffer[1000];
    bzero(ubuffer,sizeof(ubuffer));
    bzero(wbuffer,sizeof(wbuffer));
    bzero(pbuffer,sizeof(pbuffer));
    int status=write(connect_fd,get,strlen(get));
    if(status==-1)
    {
        printf("There is error in writing");
        return 0;
    }
    status=read(connect_fd,ubuffer,sizeof(ubuffer));
    if(status==-1)
    {
        printf("There is an error in reading\n");
        return 0;
    }
    status=write(connect_fd,pass,strlen(pass));
    if(status==-1)
    {
        printf("There is an error in writing\n");
        return 0;
    }
    status=read(connect_fd,pbuffer,sizeof(pbuffer));
    if(status==-1)
    {
        printf("There is an error in reading");
        return 0;
    }
    int a=strcmp(ubuffer,ADMIN_USERNAME);
    int b=strcmp(pbuffer,ADMIN_PASSWORD);
    if(a==0&&b==0)
    {
        write(connect_fd,successmsg,strlen(successmsg));
        return 1;
    }
    write(connect_fd,pwdmsg,strlen(pwdmsg));
    return 0;
}

void add_student(int connect_fd)
{
    ssize_t rb,wb;//for read bytes and write bytes
    char rbuffer[1000],wbuffer[1000];
    struct Student st,pst;//one for current student and another for previous student
    int stdfd=open("st_record.txt",O_RDONLY);
    if (stdfd==-1)
    {
        st.id=0;
    }
    else
    {
        rb=read(stdfd,&pst,sizeof(struct Student));
        if (rb==-1)
        {
            printf("There is an error while reading the file");
        }
        close(stdfd);
        st.id=st.id + 1;
    }
    sprintf(wbuffer,"%s",askname);
    wb=write(connect_fd,wbuffer,sizeof(wbuffer));
    if(wb==-1)
    {
        printf("\nThere is an error while writing");
        return;
    }
    bzero(rbuffer,sizeof(rbuffer));
    rb=read(connect_fd,rbuffer,sizeof(rbuffer));
    if(rb==-1)
    {
        printf("\nThere is an error while reading the file");
        return;
    }
    strcpy(st.name,rbuffer);//here we are copying the name from read buffer to student name
    wb=write(connect_fd,askgender,strlen(askgender));
    if(wb==-1)
    {
        printf("\nThere is an error while writing");
        return;
    }
    bzero(rbuffer,sizeof(rbuffer));
    rb=read(connect_fd,rbuffer,sizeof(rbuffer));
    if(rb==-1)
    {
        printf("\nThere is an error wile reading");
        return;
    }
    st.gender=rbuffer[0];
    bzero(wbuffer,sizeof(wbuffer));
    strcpy(wbuffer,STUDENT_AGE);
    wb=write(connect_fd,wbuffer,strlen(wbuffer));
    if(wb==-1)
    {
        printf("\nThere is an error in writing");
        return;
    }
    bzero(rbuffer,sizeof(rbuffer));
    rb=read(connect_fd,rbuffer,sizeof(rbuffer));
    if(rb==-1)
    {
        printf("\nThere is an error in reading");
        return;
    }
    bzero(wbuffer,sizeof(wbuffer));
    int age=atoi(rbuffer);
    st.age=age;
    strcpy(st.login,st.name);
    strcat(st.login,"-");
    sprintf(wbuffer,"%d",st.id);
    strcat(st.login,wbuffer);
    char haspass[1000];
    strcpy(haspass,crypt("rishi","666"));
    strcpy(st.password,haspass);
    stdfd=open("st_record.txt",O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
    if(stdfd==-1)
    {
        printf("There is an error in opening the file");
        return;
    }
    wb=write(stdfd,&st,sizeof(st));
    close(stdfd);
}

void view_student(int connect_fd)
{
    ssize_t rb, wb;
    char rbuffer[1000], wbuffer[1000];
    int stdfd = open("st_record.txt", O_RDONLY);  
    if (stdfd == -1)
    {
        printf("Error opening student record file\n");
        return;
    }   
    // Implement file locking
    struct flock fl;
    fl.l_type = F_RDLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len=0;
    if (fcntl(stdfd, F_SETLKW, &fl) == -1)
    {
        printf("Error locking the student record file\n");
        close(stdfd);
        return;
    }
    struct Student st;  
    while (1)
    {
        rb = read(stdfd, &st, sizeof(struct Student));
        if (rb == -1)
        {
            printf("Error reading student record file\n");
            break;
        }
        if (rb == 0)
        {
            // End of file
            break;
        }      
        // Now, you can send the student information to the client
        sprintf(wbuffer, "Student ID: %d\nStudent Name: %s\nAge: %d\nGender: %c\n", st.id, st.name, st.age, st.gender);
        wb = write(connect_fd, wbuffer, strlen(wbuffer));
        if (wb == -1)
        {
            printf("Error writing student information to the client\n");
            break;
        }  
        // You may add more formatting or a separator here if needed
    }    
    // Release the file lock
    fl.l_type = F_UNLCK;
    if (fcntl(stdfd, F_SETLK, &fl) == -1)
    {
        printf("Error releasing the file lock\n");
    }   
    close(stdfd);
}

void add_faculty(int connect_fd)
{
    ssize_t rb, wb;
    char rbuffer[1000], wbuffer[1000];
    struct Faculty ft, pft; // One for the current faculty and another for the previous faculty
    int facfd = open("faculty_record.txt", O_RDONLY);   
    if (facfd == -1)
    {
        ft.id = 0;
    }
    else
    {
        rb = read(facfd, &pft, sizeof(struct Faculty));
        if (rb == -1)
        {
            printf("There is an error while reading the file");
            return;
        }
        close(facfd);
        ft.id = pft.id + 1;
    } 
    sprintf(wbuffer, "%s", askname);
    wb = write(connect_fd, wbuffer, sizeof(wbuffer));
    if (wb == -1)
    {
        printf("\nThere is an error while writing");
        return;
    }   
    bzero(rbuffer, sizeof(rbuffer));
    rb = read(connect_fd, rbuffer, sizeof(rbuffer));
    if (rb == -1)
    {
        printf("\nThere is an error while reading the file");
        return;
    }
    strcpy(ft.name, rbuffer);   
    wb = write(connect_fd, askgender, strlen(askgender));
    if (wb == -1)
    {
        printf("\nThere is an error while writing");
        return;
    }  
    bzero(rbuffer, sizeof(rbuffer));
    rb = read(connect_fd, rbuffer, sizeof(rbuffer));
    if (rb == -1)
    {
        printf("\nThere is an error while reading");
        return;
    }
    ft.gender = rbuffer[0];  
    bzero(wbuffer, sizeof(wbuffer));
    strcpy(wbuffer, STUDENT_AGE);
    wb = write(connect_fd, wbuffer, strlen(wbuffer));
    if (wb == -1)
    {
        printf("\nThere is an error in writing");
        return;
    }
    bzero(rbuffer, sizeof(rbuffer));
    rb = read(connect_fd, rbuffer, sizeof(rbuffer));
    if (rb == -1)
    {
        printf("\nThere is an error in reading");
        return;
    }  
    bzero(wbuffer, sizeof(wbuffer));
    int age = atoi(rbuffer);
    ft.age = age;
    strcpy(ft.login, ft.name);
    strcat(ft.login, "-");
    sprintf(wbuffer, "%d", ft.id);
    strcat(ft.login, wbuffer);
    char haspass[1000];
    strcpy(haspass, crypt("fapa", "666"));
    strcpy(ft.password, haspass);    
    facfd = open("faculty_record.txt", O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
    if (facfd == -1)
    {
        printf("There is an error in opening the file");
        return;
    } 
    wb = write(facfd, &ft, sizeof(ft));
    close(facfd);
}


void view_faculty(int connect_fd)
{
    ssize_t rb, wb;
    char rbuffer[1000], wbuffer[1000];
    int facultyfd = open("faculty_record.txt", O_RDONLY);
    if (facultyfd==-1)
    {
        printf("Error opening student record file\n");
        return;
    }
    // Implement file locking
    struct flock fl;
    fl.l_type = F_RDLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    if (fcntl(facultyfd,F_SETLKW,&fl)==-1)
    {
        printf("Error locking the student record file\n");
        close(facultyfd);
        return;
    }
    struct Faculty ft;
    while (1)
    {
        rb=read(facultyfd,&ft,sizeof(struct Faculty));
        if(rb==-1)
        {
            printf("Error reading faculty record file\n");
            break;
        }
        if (rb==0)
        {
            // rb 0 means end of the file
            break;
        }
        sprintf(wbuffer, "Faculty ID: %d\nFaculty Name: %s\nAge: %d\nGender: %c\n", ft.id, ft.name, ft.age, ft.gender);
        wb = write(connect_fd, wbuffer, strlen(wbuffer));
        if (wb == -1)
        {
            printf("Error writing faculty information to the client\n");
            break;
        }
    }   
    // Release the file lock
    fl.l_type = F_UNLCK;
    if (fcntl(facultyfd,F_SETLK, &fl) == -1)
    {
        printf("Error releasing the file lock\n");
    }  
    close(facultyfd);
}





void admin_impl(int connect_fd)
{
    if(verify_admin(connect_fd))
    {
        ssize_t wbytes, rbytes;            
        char rbuffer[1000], wbuffer[1000]; 
        bzero(wbuffer,sizeof(wbuffer));
        while(1)
        {
            strcat(wbuffer,ADMIN_MENU);
            wbytes=write(connect_fd,wbuffer,strlen(wbuffer));
            if(wbytes==-1)
            {
                printf("Error while writing ADMIN_MENU to client!");
                return;
            }
            bzero(wbuffer,sizeof(wbuffer));
            rbytes=read(connect_fd,rbuffer,sizeof(rbuffer));
            if (rbytes==-1)
            {
                printf("Error while reading client's choice for ADMIN_MENU");
                return;
            }
            int choice=atoi(rbuffer);
           switch (choice)
            {
            case 1:
                add_student(connect_fd);
                break;
            case 2:
                view_student(connect_fd);
                break;
            case 3: 
                add_faculty(connect_fd);
                break;
            case 4:
                view_faculty(connect_fd);
                break;
            case 7:
                modiy_student(connect_fd);
                break;
            case 8:
                //modify_customer_info(connFD);
                break;
            default:
               break;
                //writeBytes = write(connFD, ADMIN_LOGOUT, strlen(ADMIN_LOGOUT));
                //return false;
            }
        }
    }
    else
    {

    }  
}
#endif
