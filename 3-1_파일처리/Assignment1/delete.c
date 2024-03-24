#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main(int argc,char *argv[]){
	int fd = open(argv[1],O_RDWR,0777);
	int offsets = atoi(argv[2]);
	int readword;
	int remove = atoi(argv[3]);
	char buff[500];
	char front_buff[500];
	char end_buff[500];
	int frontWord=0;

	frontWord=read(fd,front_buff,offsets);	
	front_buff[frontWord]='\0';
	lseek(fd,offsets,SEEK_SET);
	
	if(read(fd,buff,remove)==remove){
		lseek(fd,offsets+remove,SEEK_SET);
		readword=read(fd,end_buff,sizeof(end_buff));
		end_buff[readword]='\0';
		
		close(fd);
		fd = open(argv[1],O_RDWR|O_TRUNC,0777);
		write(fd,front_buff,strlen(front_buff));
		write(fd,end_buff,strlen(end_buff));
	}
	else{
		close(fd);
		fd = open(argv[1],O_RDWR|O_TRUNC,0777);
		write(fd,front_buff,strlen(front_buff));
	}
	close(fd);
	return 0;
}
