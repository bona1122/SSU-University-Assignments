#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc,char *argv[]){
	int fd;
	int offsets;
	int word;
	char buff[100];

	offsets =atoi(argv[2]);
	fd = open(argv[1],O_RDONLY,0777);
	word = atoi(argv[3]);

	lseek(fd,offsets,0);

	int success = read(fd,buff,word);

	if(success == word){
		write(1,buff,word);
	}
	else if(success < word && word !=0){
		write(1,buff,word);
	}
	close(fd);
return 0;
}
