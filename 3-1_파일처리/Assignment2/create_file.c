#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
typedef struct _Student{
        char data[250];
}Student;

int main(int argc, char **argv)
{
        int num = atoi(argv[1]);
        int fd;
        Student s1;
        for(int i=0;i<250;i++){
                s1.data[i]='\0';
        }

        fd = open(argv[2],O_RDWR|O_CREAT|O_TRUNC,0666);

        write(fd,&num,4);

        for(int i=0;i<num;i++){
                write(fd,(char*)&s1,sizeof(s1));
        }
        close(fd);

        return 0;
}

