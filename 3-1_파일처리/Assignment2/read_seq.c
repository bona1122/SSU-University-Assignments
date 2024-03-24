#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
typedef struct _Student{
        char data[250];
}Student;

int main(int argc, char **argv){
        struct timeval startTime, endTime;
        double diffTime;

        int fd = open(argv[1],O_RDONLY,0777);
        lseek(fd,0,SEEK_SET);
        Student s1;
        int num;
        int test;
        test=read(fd,&num,sizeof(num));
        printf("#records: %d ",num);

        gettimeofday(&startTime,NULL);
        for(int i=0; i<num; i++){
                read(fd,(char*)&s1,sizeof(s1));
        }
        gettimeofday(&endTime,NULL);
        diffTime = (endTime.tv_sec-startTime.tv_sec)*1000000 + (endTime.tv_usec-startTime.tv_usec);
        printf("elapsed_time: %d us\n",(int)diffTime);

        close(fd);

        return 0;
}

