#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int main(int argc,char *argv[]){
	int fd = open(argv[1],O_RDWR,0777);
	int offsets = atoi(argv[2]);
	
	lseek(fd,offsets,0);

	write(fd,argv[3],strlen(argv[3]));

	close(fd);
	return 0;
}
