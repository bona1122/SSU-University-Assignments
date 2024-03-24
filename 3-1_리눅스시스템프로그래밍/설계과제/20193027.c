#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/time.h>
#define BUFFER_SIZE 10000
#define MAX 10000
void file_write(int** p,int g,int m,int n);
void rename_output(int g);
void matrix_size(FILE* inpurtfile,char*buf,int*m,int*n);
void array_alloc(int*** matrix,int m,int n,FILE* fp,char* buf);
void print_matrix(int** matrix,int m,int n);
int*** generate(int*** matrix1,int***matrix2,int* order);
int m;
int n;
int** matrix;
int** matrix2;

//thread struct
typedef struct _thread_data{
	int start_row;
	int row_num;
	int** sepo1;
	int** sepo2;
	int order;
}thread_data;
//process struct
typedef struct _shm_info{
	int matrix[MAX][MAX];
	int matrix2[MAX][MAX];
	int order;
}Shared_memory;
void file_write_p(Shared_memory* share,int g,int m, int n);
void gen_pc(Shared_memory* share,int start_row,int num);
void print_matrix_p(int(*matrix)[MAX],int m,int n);
void array_alloc_p(int(*matrix)[MAX] ,int m, int n,FILE*fp,char*buf);
void* gen_th(void*arg);
void divide_row(int row, int task_num, int** array); 

struct timeval start_time,end_time;
double delay_time;

int main(int argc,char* argv[]){
	int way=0;
	int gen_num=0;
	char buf[BUFFER_SIZE];
        //int** matrix;
        //int** matrix2;
        m=0;n=0;

	FILE* inputfile = fopen(argv[1],"r");
	
	while(1){
	printf("(1) exit (2) seq (3) process (4) thread : ");
	scanf("%d",&way);
	
	if(way==1) exit(0);
	switch(way){
		case 1:
			exit(0);
			break;
		case 2:
		{
			int seq=1;
			int g=1;
			m=0;n=0;
			printf("gen? : ");
			scanf("%d",&gen_num);

			gettimeofday(&start_time,NULL);
			matrix_size(inputfile,buf,&m,&n);
			array_alloc(&matrix,m,n,inputfile,buf);
			array_alloc(&matrix2,m,n,inputfile,buf);
			int***gen_p=&matrix;
			for(int i=0;i<gen_num;i++){
				gen_p = generate(&matrix,&matrix2,&seq);
				file_write(*gen_p,g,m,n);
				seq++;
				g++;
			}
			//print_matrix(*gen_p,m,n);
			rename_output(g);
			gettimeofday(&end_time,NULL);
			delay_time = (end_time.tv_sec-start_time.tv_sec)*1000+((end_time.tv_usec-start_time.tv_usec)/1000);
			printf("%f ms\n",delay_time);
			break;
		}
		case 3:
		{	
			int g=1;
			pid_t pid;
			int shmid;
			int gen_num=0;
			int process_num=0;
			Shared_memory* shared = NULL;
			void *memory_segment = NULL;
			m=0;n=0;

			printf("process num? : ");
                        scanf("%d",&process_num);
                        printf("gen num? : ");
                        scanf("%d",&gen_num);

			pid_t* p = (pid_t*)malloc(sizeof(pid_t)*process_num);

			gettimeofday(&start_time,NULL);
			matrix_size(inputfile,buf,&m,&n);
			if((shmid = shmget((key_t)1234,sizeof(Shared_memory),IPC_CREAT|0666))==-1)
			{ 
				printf("shmget failed\n");
				exit(0);
			}
			if((memory_segment=shmat(shmid,NULL,0))==(void*)-1){
				printf("shmat failed\n");
				exit(0);
			}
			shared=(Shared_memory*)memory_segment;
			(*shared).order = 1;
			array_alloc_p((shared->matrix),m,n,inputfile,buf);
			array_alloc_p((shared->matrix2),m,n,inputfile,buf);

			/*printf("process num? : ");
			scanf("%d",&process_num);	
			printf("gen num? : ");	
			scanf("%d",&gen_num);*/
		
			int* array = (int*)malloc(sizeof(int)*process_num);
			divide_row(m,process_num,&array);
			for(int i=0;i<gen_num;i++){
				int row_number = 1;
				int seq = 0;
				for(int j=0;j<process_num;j++,seq++){
					if((pid=fork())<0){
						printf("fork failed\n");
						exit(0);
					}
					else if(pid==0){
						gen_pc(shared,row_number,array[j]);
						exit(0);
					}
					else{
						p[seq] = pid;
					}
					row_number+=array[j];
				}
				while(wait((int*)0)!=-1);
				file_write_p(shared,g,m,n);
				(*shared).order++;
				g++;
			}
			rename_output(g);
			/*if(((*shared).order-1)%2==1){
				print_matrix_p((*shared).matrix2,m,n);	
			}
			else{
				print_matrix_p((*shared).matrix,m,n);
			}*/
			gettimeofday(&end_time,NULL);
			printf("used process id list\n");
			for(int i=0;i<process_num;i++){
				printf("id : %d\n",p[i]);
			}
                        delay_time = (end_time.tv_sec-start_time.tv_sec)*1000+((end_time.tv_usec-start_time.tv_usec)/1000);
                        printf("%f ms\n",delay_time);
			break;
		}
		case 4:
		{
			m=0;n=0;

                        int thread_num = 0;
                        int gen_num = 0;
                        printf("thread num? : ");
                        scanf("%d",&thread_num);
                        printf("gen num? : ");
                        scanf("%d",&gen_num);
			
			gettimeofday(&start_time,NULL);
			matrix_size(inputfile,buf,&m,&n);
        		array_alloc(&matrix,m,n,inputfile,buf);
        		array_alloc(&matrix2,m,n,inputfile,buf);
			
			/*int thread_num = 0;
			int gen_num = 0;
			printf("thread num? : ");
			scanf("%d",&thread_num);
			printf("gen num? : ");
			scanf("%d",&gen_num);*/

			int* array = (int*)malloc(sizeof(int)*thread_num);
			pthread_t* tid_array = (pthread_t*)malloc(sizeof(pthread_t)*thread_num);
			divide_row(m,thread_num,&array);

			thread_data* pThreadData = NULL;
			pThreadData = (thread_data*)malloc(sizeof(thread_data)*thread_num);
			int g = 1;
			int order_num = 1;	
			for(int k = 0; k<gen_num;k++){
				int row_number = 1;
				for(int i=0;i<thread_num;i++){
					pThreadData[i].start_row=row_number;
					pThreadData[i].row_num=array[i];
					pThreadData[i].sepo1 = matrix;
					pThreadData[i].sepo2 = matrix2;
					pThreadData[i].order = order_num;
					if(pthread_create(&tid_array[i],NULL,gen_th,(void*)&(pThreadData[i]))!=0)
					{
						fprintf(stderr,"pthread_create error\n");
						exit(1);
					}
					row_number+=array[i];
				}
				for(int t=0;t<thread_num;t++){
					pthread_join(tid_array[t],NULL);
				}
				//order_num++;
				if(order_num%2==1){
					file_write(matrix2,g,m,n);
				}
				else{
					file_write(matrix,g,m,n);
				}
				g++;
				order_num++;
			}
			rename_output(g);
			gettimeofday(&end_time,NULL);
			printf("used thread id list\n");
			for(int i=0;i<thread_num;i++){
				printf("id: %ld\n",tid_array[i]);
			}
                        delay_time = (end_time.tv_sec-start_time.tv_sec)*1000+((end_time.tv_usec-start_time.tv_usec)/1000);
                        printf("%f ms\n",delay_time);

			free(array);
			free(pThreadData);
			free(tid_array);
			break;
		}
	}
	printf("\n");
	}

	fclose(inputfile);
	return 0;
}
void file_write_p(Shared_memory* share,int g,int m, int n){
	int(*p)[MAX];
	char filename[30];
	char* head = "gen_";
	char* tail = ".matrix";
	char mybuf[10000];
	char mybuf2[10000];

	memset(filename,0,sizeof(filename));
	sprintf(mybuf,"%d",g); 
	strcat(filename,head);
	strcat(filename,mybuf);
	strcat(filename,tail);
	//printf("%d : %s\n",g,filename);
	
	FILE* fp = fopen(filename,"w+");

	if((*share).order%2==1){
		p=(*share).matrix2;
	}
	else{
		p=(*share).matrix;
	}
	

	int row = 1; int col = 1;
	int offset = 0;
	for(int i=row;i<=m;i++){
		offset=0;
		for(int j=col;j<=n;j++){
			if(p[i][j]==0){
				mybuf2[offset++]='0';
			}
			else{
				mybuf2[offset++]='1';
			}
			if(j==n){
				if(i==m)
					;
				else
					mybuf2[offset++]='\n';
				mybuf2[offset]='\0';
				break;
			}
			mybuf2[offset++]=' ';
		}
		fputs(mybuf2,fp);
	}
	fclose(fp);
}
void matrix_size(FILE* inputfile,char* buf, int* m, int*n){
	fseek(inputfile,0,SEEK_SET);
	while(fgets(buf,BUFFER_SIZE,inputfile)!=NULL){
		(*m)++;
	}
	for(int i=0;buf[i]!='\0';i++){
		if(buf[i]=='0'|buf[i]=='1')
			(*n)++;
	}
	return;	
}
void array_alloc(int*** matrix,int m, int n,FILE*fp,char*buf){
	int row=1;
	int col=1;
	*matrix = (int**)malloc(sizeof(int*)*(m+2));
	for(int i=0; i<m+2;i++){
		(*matrix)[i] = (int*)malloc(sizeof(int)*(n+2));
		memset((*matrix)[i], 0, sizeof(int)*(n+2));
	}
	fseek(fp,0,SEEK_SET);
	while(fgets(buf,BUFFER_SIZE,fp)!=NULL){
		for(int i=0;buf[i]!='\0';i++){
			if(buf[i]=='0' | buf[i]=='1')
				(*matrix)[row][col++]=buf[i]-'0';
		}
		row++;
		col=1;
	}
	return;
}
void print_matrix(int** matrix,int m,int n){
	for(int i=0;i<m+2;i++){
                for(int j=0;j<n+2;j++)
                        printf("%d ",matrix[i][j]);
                printf("\n");
        }
}
int*** generate(int***matrix1,int***matrix2,int*order){
	int k=1;
	int j=1;
	int live = 0;
	int*** p;
	int*** q;
	if((*order)%2==1){
		p=matrix1;
		q=matrix2;
	}
	else{
		p=matrix2;
		q=matrix1;
	}
	while(k!=m+1){
		live=0;
		for (int row = k - 1; row < k + 2; row++) { 
			for (int col = j - 1; col < j + 2; col++) {
				if (row == k && col == j)
					;
				else {
					if ((*p)[row][col] == 1)
						live++;
				}
			}
		}
		if ((*p)[k][j] == 1) { //세포가 살아있는 경우
			if (live <= 2 | live >= 7)
				(*q)[k][j] = 0;
			else
				(*q)[k][j] = 1;
		}
		else {	//세포가 죽어있는 경우
			if (live == 4)
				(*q)[k][j] = 1;
			else
				(*q)[k][j] = 0;
		}
		j++;
		if (j == n + 1) {
			k++;
			j = 1;
		}
	}
	return q;
}
void* gen_th(void*arg){
thread_data* data;
	data = (thread_data*)arg;
	int live = 0;
	int** p;
	int** q;
	if ((data->order) % 2 == 1) {
		p = data->sepo1;
		q = data->sepo2;
	}
	else {
		//printf("sepo2->sepo1\n");
		p = data->sepo2;
		q = data->sepo1;
	
		//printf("start\n");
		//print_matrix(p,m,n);
	}
	int k = data->start_row; //이 행부터 시작해서 바꿀것.
	int j = 1; //이 열부터 확인할것
	while (1) {
		if (k == data->start_row + data->row_num)
			break;
		live=0;
		for (int row = k - 1; row < k + 2; row++) {
			for (int col = j - 1; col < j + 2; col++) {
				if (row == k && col == j)
					;
				else {
					if (p[row][col] == 1)
						live++;
				}
			}
		}
		if (p[k][j] == 1) {
			//if (live <= 2 | live >= 7)
			//	q[k][j] = 0;
			if(live<=2)
				q[k][j] = 0;
			else if(live>=7)
				q[k][j] = 0;
			else
				q[k][j] = 1;
		}
		else {
			if (live == 4)
				q[k][j] = 1;
			else
				q[k][j] = 0;
		}
		j++;
		if (j == n + 1) {
			k++;
			j = 1;
		}
	}
}
void divide_row(int row, int task_num, int** array) {
	for (int i = 0; i < task_num; i++) {
		(*array)[i] = row / task_num;
	}
	int r = row % task_num;
	for (int i = 0; i < r; i++) {
		(*array)[i]++;
	}
}
void gen_pc(Shared_memory* share,int start_row,int row_num){
	//(*share).matrix[0][0] = 123;
	int live = 0;
	int(*p)[MAX];
	int(*q)[MAX];

	if((*share).order%2==1){
		p = (*share).matrix;
		q = (*share).matrix2;
	} 
	else{
		p = (*share).matrix2;
		q = (*share).matrix;
	}
	int k = start_row;
	int j = 1;
	while(1){
		if(k==start_row+row_num)
			break;
		live = 0;
		for (int row = k - 1; row < k + 2; row++) {
			for (int col = j - 1; col < j + 2; col++) {
				if (row == k && col == j)
					;
				else {
					if (p[row][col] == 1)
						live++;
				}
			}
		}
		if (p[k][j] == 1) {
			//if (live <= 2 | live >= 7)
			//	q[k][j] = 0;
			if(live<=2)
				q[k][j] = 0;
			else if(live>=7)
				q[k][j] = 0;
			else
				q[k][j] = 1;
		}
		else {
			if (live == 4)
				q[k][j] = 1;
			else
				q[k][j] = 0;
		}
		j++;
		if (j == n + 1) {
			k++;
			j = 1;
		}
	}
	return; 	
}
void array_alloc_p(int(*matrix)[MAX],int m, int n,FILE*fp,char*buf){
	int row=1;
	int col=1;
	memset(matrix,0,sizeof(matrix));
	fseek(fp,0,SEEK_SET);
	while(fgets(buf,BUFFER_SIZE,fp)!=NULL){
		for(int i=0;buf[i]!='\0';i++){
			if(buf[i]=='0' | buf[i]=='1')
				matrix[row][col++]=buf[i]-'0';
		}
		row++;
		col=1;
	}
	return;
}
void print_matrix_p(int(*matrix)[MAX],int m,int n){
	for(int i=0;i<m+2;i++){
                for(int j=0;j<n+2;j++)
                        printf("%d ",matrix[i][j]);
                printf("\n");
        }
}
void rename_output(int g){
	char oldname[30];
        char* head = "gen_";
        char* tail = ".matrix";
        char mybuf[10000];
	g--;

        memset(oldname,0,sizeof(oldname));
        sprintf(mybuf,"%d",g);
        strcat(oldname,head);
        strcat(oldname,mybuf);
	strcat(oldname,tail);

	FILE*fp = fopen("output.matrix","w+");
	fclose(fp);
	remove("output.matrix");
	rename(oldname,"output.matrix");
}
void file_write(int**p,int g,int m, int n){
	char filename[30];
	char* head = "gen_";
	char* tail = ".matrix";
	char mybuf[10000];
	char mybuf2[10000];

	memset(filename,0,sizeof(filename));
	sprintf(mybuf,"%d",g); 
	strcat(filename,head);
	strcat(filename,mybuf);
	strcat(filename,tail);
	//printf("%d : %s\n",g,filename);
	
	FILE* fp = fopen(filename,"w+");

	int row = 1; int col = 1;
	int offset = 0;
	for(int i=row;i<=m;i++){
		offset=0;
		for(int j=col;j<=n;j++){
			if(p[i][j]==0){
				mybuf2[offset++]='0';
			}
			else{
				mybuf2[offset++]='1';
			}
			if(j==n){
				if(i==m)
					;
				else
					mybuf2[offset++]='\n';
				mybuf2[offset]='\0';
				break;
			}
			mybuf2[offset++]=' ';
		}
		fputs(mybuf2,fp);
	}
	fclose(fp);
}
