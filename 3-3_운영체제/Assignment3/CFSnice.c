#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#define ROW 1000 
#define COL 1000
int main(){
        int status;
        int** matrix;
        pid_t pidArr[21] ={0};

        matrix = (int**)calloc(ROW,sizeof(int*));
        for(int i=0;i<ROW;i++){
                matrix[i]=(int*)calloc(COL,sizeof(int));
        }

        for(int num=0;num<21;num++){
                if((pidArr[num]=fork())<0){
                        fprintf(stderr, "fork error\n");
                        exit(1);
                }
                else if(pidArr[num]==0){
			int rep;
			int which = PRIO_PROCESS;

			if(num<7){
                                rep = 100;
				setpriority(which,0,19);
                        }
                        else if(num<14){
                                rep = 400;
				setpriority(which,0,5);
                        }
                        else{
                                rep = 800;
				setpriority(which,0,(-5));
                        }

			
                        for(int i=0;i<rep;i++){
                                for(int j=0;j<ROW;j++){
                                        for(int k=0;k<COL;k++){
                                                matrix[j][k]++;
                                        }
                                }
                        }
                        free(matrix);
                        printf(" %d process end\n",getpid());
                        exit(0);
                }
                else{
			printf(" %d process begin\n",pidArr[num]);
                }
        }
        for(int i=0;i<21;i++){
                status = waitpid(pidArr[i],&status,0);
        }
        free(matrix);
        return 0;
}

