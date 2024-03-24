#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
int main(int argc,char* argv[])
{
	if(argc==2){
		int cnt = atoi(argv[1]);
		int c = 0;
		while(1){
			if(fork()==0){
				sleep(1);
				execlp("date","date",(char*)0);
			}
			else{
				while(wait((int*)0)!=-1);
				c++;
				//printf("c:%d\n",c);
				if(c==cnt)
					kill(getpid(),SIGKILL);
				else if(c>=cnt-5){
					printf("%d seconds before termination\n",cnt-c);}
				else
					;
			}
		}
	}	
	else{
		while(1){
			if(fork()==0){
				sleep(1);
				execlp("date","date",(char*)0);
			}
			else{
				while(wait((int*)0)!=-1);
			}
		}
	}
	return 0;
}
