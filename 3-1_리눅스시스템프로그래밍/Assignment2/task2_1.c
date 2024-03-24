#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
int main() {
        int num = 0;
        char ch;
        char n='\n';
        char fileName[20];
        int fd;
        
        printf("input: ");
        scanf("%d", &num);
        getchar(); getchar();
        scanf("%c", &ch);
        getchar(); getchar();
        scanf("%s", fileName);

        fd=open(fileName,O_RDWR|O_CREAT|O_TRUNC,0666);

        if(num>0){
                for(int i=0;i<num;i++){
                        for(int j=0; j<i+1;j++){
                                write(fd,&ch,1);
                        }
                        write(fd,&n,1);
                }
        }
        else{
                for(int i=num; i<0;i++){
                        for(int j=i;j<0;j++){
                                write(fd,&ch,1);
                        }
                        write(fd,&n,1);
                }
        }
        close(fd);
return 0;
}
