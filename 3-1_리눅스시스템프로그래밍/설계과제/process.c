#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>
#define BUFFER_SIZE 10000
#define MAX 10000
void divide_row(int row, int task_num, int** array);
void print_matrix(int(*matrix)[MAX],int m,int n);
void array_alloc(int(*matrix)[MAX] ,int m, int n,FILE*fp,char*buf);
void matrix_size(FILE* inputfile,char* buf, int* m, int*n);
typedef struct _shm_info{
	int matrix[MAX][MAX];
	int matrix2[MAX][MAX];
	int order;
}Shared_memory;
void file_write(Shared_memory* share,int g,int m, int n);
void rename_output(int g);
void gen_pc(Shared_memory* share,int start_row,int num);
int m,n;
int main(int argc,char* argv[]){
	pid_t pid;
	int shmid;
	Shared_memory* shared = NULL;
	void *memory_segment = NULL;

	FILE* inputfile = fopen(argv[1],"r");
	char buf[BUFFER_SIZE];
	m=0;n=0;
	matrix_size(inputfile,buf,&m,&n);

	if((shmid = shmget((key_t)3856,sizeof(Shared_memory),IPC_CREAT|0666))==-1)
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
	array_alloc((shared->matrix),m,n,inputfile,buf);
	array_alloc((shared->matrix2),m,n,inputfile,buf);

	/*print_matrix((*shared).matrix,m,n);
	printf("\n");
	print_matrix((*shared).matrix2,m,n);*/

	int g = 1;
	int gen_num = 2;
	printf("gen? : ");
	scanf("%d",&gen_num);
	int process_num = 5;
	int* array = (int*)malloc(sizeof(int)*process_num);
	divide_row(m,process_num,&array);
	for(int i=0;i<gen_num;i++){
		int row_number = 1;
		for(int j=0;j<process_num;j++){
			if((pid=fork())<0){
				printf("ford failed\n");
				exit(0);
			}
			else if(pid==0){
				gen_pc(shared,row_number,array[j]);
				exit(0);
			}
			else{
				;
			}
			row_number+=array[j];
		}
		while(wait((int*)0)!=-1);
		file_write(shared,g,m,n);
		(*shared).order++;
		g++;
	}
	rename_output(g);
	return 0;
}
void file_write(Shared_memory* share,int g,int m, int n){
	int(*p)[MAX];
	char filename[30];
	char* head = "mygen_";
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
void array_alloc(int(*matrix)[MAX],int m, int n,FILE*fp,char*buf){
	int row=1;
	int col=1;
	//*matrix = (int**)malloc(sizeof(int*)*(m+2));
	/*for(int i=0; i<m+2;i++){
		(*matrix)[i] = (int*)malloc(sizeof(int)*(n+2));
		memset((*matrix)[i], 0, sizeof(int)*(n+2));
	}*/
	matrix[0][0]=123;
	memset(matrix,0,sizeof(matrix));
	print_matrix(matrix,m,n);
	printf("\n");
	fseek(fp,0,SEEK_SET);
	memset(buf,0,sizeof(buf));
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
void print_matrix(int(*matrix)[MAX],int m,int n){
	for(int i=0;i<m+2;i++){
                for(int j=0;j<n+2;j++)
                        printf("%d ",matrix[i][j]);
                printf("\n");
        }
}
void matrix_size(FILE* inputfile,char* buf, int* m, int*n){
	while(fgets(buf,BUFFER_SIZE,inputfile)!=NULL){
		(*m)++;
	}
	for(int i=0;buf[i]!='\0';i++){
		if(buf[i]=='0'|buf[i]=='1')
			(*n)++;
	}
	return;	
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
void rename_output(int g){
	char oldname[30];
        char* head = "mygen_";
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
