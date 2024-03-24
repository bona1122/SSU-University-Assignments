#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc,char *argv[]){
	int fd1;
	int fd2;
	int fd3;
	int offsets=0;
	char buff[200];
	char c;

	fd1 = open(argv[1],O_RDWR|O_CREAT|O_TRUNC,0777);
	fd2 = open(argv[2],O_RDONLY,0777);
	fd3 = open(argv[3],O_RDONLY,0777);

	lseek(fd1,0L,0);
	lseek(fd2,0L,0);
	lseek(fd3,0L,0);

	while(read(fd2,&c,1)!=0){
		write(fd1,&c,1);
		offsets++;
	}
	close(fd2);

	lseek(fd1,offsets-1,0);

	while(read(fd3,&c,1)!=0){
		write(fd1,&c,1);
		offsets++;
	}
	close(fd3);
	close(fd1);
return 0;
}
