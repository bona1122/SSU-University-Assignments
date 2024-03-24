#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int main(int argc,char *argv[]){
	int fd;
	char c;
	int readword;
	int offsets = atoi(argv[2]);
	fd = open(argv[1],O_RDWR,0777);
	
	char buff[500];

	lseek(fd,offsets+1,0);

	readword=read(fd,buff,200);
	buff[readword]='\0';

	lseek(fd,offsets+1,SEEK_SET);

	write(fd,argv[3],strlen(argv[3]));
	write(fd,buff,strlen(buff));

	close(fd);
	return 0;
}
