#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#define BUFFER_SIZE 10000
void matrix_size(FILE* inpurtfile,char*buf,int*m,int*n);
void array_alloc(int*** matrix,int m,int n,FILE* fp,char* buf);
void print_matrix(int** matrix,int m,int n);
int*** generate(int*** matrix1,int***matrix2,int* order);
void file_write(int** p,int g,int m,int n);
void rename_output(int g);
int m;
int n;
int main(int argc,char* argv[]){
	FILE* inputfile = fopen(argv[1],"r");
	char buf[BUFFER_SIZE];
	int** matrix;
	int** matrix2;
	m=0;n=0;
	matrix_size(inputfile,buf,&m,&n);
	array_alloc(&matrix,m,n,inputfile,buf);
	array_alloc(&matrix2,m,n,inputfile,buf);
	
	int g=1;
	int seq=1;
	int*** gen_p=&matrix;
	for(int i=0;i<1;i++){
		gen_p = generate(&matrix,&matrix2,&seq);
		file_write(*gen_p,g,m,n);
		seq++;
		g++;	
	}
	//print_matrix(*gen_p,m,n);
	rename_output(g);

	fclose(inputfile);
	return 0;
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
void file_write(int**p,int g,int m, int n){
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
