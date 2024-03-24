#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
int main(){
        int fd;
        int fdNew;
        char fileName[20];
        char transFileName[20];
        char c;

        printf("input fileName : ");

        scanf("%s",fileName);

        int strNum = strlen(fileName);

        for(int i=0; i<strNum;i++){
                if(fileName[i] >='A'&& fileName[i]<='Z'){
                        transFileName[i] = tolower(fileName[i]);
                }
                else if(fileName[i] >='a' && fileName[i] <='z'){
                         transFileName[i] = toupper(fileName[i]);
                }
                else{
                        transFileName[i] = fileName[i];
                }
                if(i+1 ==strNum){
                         transFileName[i+1] ='\0';
                }
        }

        fd=open(fileName,O_RDONLY,0666);
        fdNew=open(transFileName,O_RDWR|O_CREAT|O_TRUNC,0666);
        
        while(1){
                if(read(fd,&c,1)>0){
                        if(c>='A' && c<='Z'){
                                c=tolower(c);
                        }
                        else if(c>='a' && c<='z'){
                                 c=toupper(c);
                        }
                        else{
                                ;
                        }
                         write(fdNew,&c,1);
}
                else
                        break;
        }
        close(fd);
        close(fdNew);
return 0;
}
