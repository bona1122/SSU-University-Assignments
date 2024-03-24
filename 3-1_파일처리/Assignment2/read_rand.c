#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>

#define SUFFLE_NUM 10000

void GenRecordSequence(int *list, int n);
void swap(int *a, int *b);

typedef struct _Student{
        char data[250];
}Student;

int main(int argc,char **argv)
{
        int *read_order_list;
        int num_of_records;

        struct timeval startTime,endTime;
        double diffTime;

        Student s1;
        int fd = open(argv[1],O_RDONLY,0777);
        lseek(fd,0,SEEK_SET);
        read(fd,&num_of_records,sizeof(num_of_records));

        read_order_list = (int*)malloc(sizeof(int)*num_of_records);
        printf("#records: %d ",num_of_records);

        GenRecordSequence(read_order_list,num_of_records);

        gettimeofday(&startTime,NULL);
        for(int i=0;i<num_of_records;i++){
                lseek(fd,4+250*read_order_list[i],SEEK_SET);
                read(fd,(char*)&s1,sizeof(s1));
        }
        gettimeofday(&endTime,NULL);
        diffTime=(endTime.tv_sec-startTime.tv_sec)*1000000+(endTime.tv_usec-startTime.tv_usec);
        printf("elapsed_time: %d us\n",(int)diffTime);

        close(fd);
	if(read_order_list!=NULL){
                free(read_order_list);
                read_order_list = NULL;
        }
        return 0;
}

void GenRecordSequence(int *list,int n)
{
        int i,j,k;

        srand((unsigned int)time(0));

        for(i=0; i<n; i++)
        {
                list[i] = i;
        }

        for(i=0; i<SUFFLE_NUM;i++)
        {
                j=rand()%n;
                k=rand()%n;
                swap(&list[j],&list[k]);
        }
}
void swap(int *a, int *b)
{
        int tmp;

        tmp=*a;
        *a=*b;
        *b=tmp;
}

