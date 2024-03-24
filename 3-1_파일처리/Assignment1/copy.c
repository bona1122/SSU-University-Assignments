#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc,char *argv[]){
        int fd;
        int fd2;
        char buff[10];

        fd = open(argv[1],O_RDONLY,0777);
        fd2 = open(argv[2],O_RDWR|O_CREAT|O_TRUNC,0777);

        lseek(fd,0L,0);
        lseek(fd2,0L,0);

	while(1){
		int word = read(fd,buff,10);

		if(word==10){
			write(fd2,buff,10);
		}
		else if(word < 10 && word!=0){
			write(fd2,buff,word);
		}
		else{
			break;
		}
	}

        close(fd);
        close(fd2);
	
return 0;
}

