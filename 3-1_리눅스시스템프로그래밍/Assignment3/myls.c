#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

typedef struct _tarray{
        time_t mtime;
        char fname[20];
}Table;
void print_array(Table A[],int n);
void Sort(Table A[],int n);

char type(mode_t);
char* perm(mode_t);
void printStat(char*,char*,struct stat*);

int main(int argc,char **argv){
        DIR *dp;
        char *dir;
        struct stat st;
        struct dirent *d;
        char path[1024];

 	Table ta;
        Table* tarray = (Table*)malloc(sizeof(Table)*20);
        int i=0;

        if(argc==1) dir = ".";
	else{
		if(strcmp(argv[1],"-l")==0 | strcmp(argv[1],"-t")==0 | strcmp(argv[1],"-i")==0){
		dir = ".";}
		else{
			dir = argv[1];
		}
	}

        if(argc == 1){
		if((dp=opendir(dir))==NULL)
			perror(dir);
                while((d=readdir(dp))!=NULL){
                if(strcmp(d->d_name,".")!=0 && strcmp(d->d_name,"..")!=0){
                printf("%s  ",d->d_name);
                }}
                printf("\n");
		closedir(dp);
        }
	else {
		if(strcmp(argv[1],"-l")==0){
			if((dp=opendir(dir))==NULL)
                        perror(dir);
			
			if(argc == 2){
			while((d=readdir(dp))!=NULL){
			if(strcmp(d->d_name,".")!=0 && strcmp(d->d_name,"..")!=0){
                       		sprintf(path,"%s/%s",dir,d->d_name);
                        	if(lstat(path,&st)<0)
                                	perror(path);
                        	printStat(path,d->d_name,&st);
                        	//putchar('\n');
                	}}}
			else{
				lstat(argv[2],&st);
				if(S_ISDIR(st.st_mode)){
					dir=argv[2];
					closedir(dp);
					if((dp=opendir(dir))==NULL)
						perror(dir);
					while((d=readdir(dp))!=NULL){
					 if(strcmp(d->d_name,".")!=0 && strcmp(d->d_name,"..")!=0){
                               			 sprintf(path,"%s/%s",dir,d->d_name);
                                		 if(lstat(path,&st)<0)
                                        		perror(path);
                                		printStat(path,d->d_name,&st);
                       		 	}
					}
				}
				else{
					sprintf(path,"%s/%s",dir,argv[2]);
                                	printStat(path,argv[2],&st);
				}
					
			}
			//printf("\n");
			closedir(dp);
		}
		else if(strcmp(argv[1],"-i")==0){
			if((dp=opendir(dir))==NULL)
				perror(dir);
			if(argc==2){
				while((d=readdir(dp))!=NULL){
				if(strcmp(d->d_name,".")!=0 && strcmp(d->d_name,"..")!=0){
				sprintf(path,"%s/%s",dir,d->d_name);
				if(lstat(path,&st)<0)
					perror(path);
				printf("%ld ",st.st_ino);
				printf("%s  ",d->d_name);
			}}}
			else{
				lstat(argv[2],&st);
				if(S_ISDIR(st.st_mode)){
					dir=argv[2];
					closedir(dp);
					if((dp=opendir(dir))==NULL)
						perror(dir);
					while((d=readdir(dp))!=NULL){
						if(strcmp(d->d_name,".")!=0 && strcmp(d->d_name,"..")!=0){
                                			sprintf(path,"%s/%s",dir,d->d_name);
                                			if(lstat(path,&st)<0)
                                        			perror(path);
                                			printf("%ld ",st.st_ino);
                                			printf("%s  ",d->d_name);
                        			}
					}
				}
				else{
					  d=readdir(dp);
                                	sprintf(path,"%s/%s",dir,argv[2]);
                                	if(lstat(path,&st)<0)
                                        	perror(path);
                                	printf("%ld ",st.st_ino);
                                	printf("%s  ",argv[2]);
				}
			}
			printf("\n");
			closedir(dp);
		}
		else if(strcmp(argv[1],"-t")==0){
			if((dp=opendir(dir))==NULL)
                		perror(dir);
        	
			if(argc==2){	
				while((d=readdir(dp))!=NULL){
                		if(strcmp(d->d_name,".")!=0 && strcmp(d->d_name,"..")!=0){
                        		sprintf(path,"%s/%s",dir,d->d_name);
                        		if(lstat(path,&st)<0)
                                		perror(path);
                        		strcpy(ta.fname,d->d_name);
                        		ta.mtime = st.st_mtime;
                        		tarray[i++]=ta;
                		}
        			}
        			Sort(tarray,i);
        			print_array(tarray,i);
        			free(tarray);
				closedir(dp);
			}
			else{
				lstat(argv[2],&st);
				if(S_ISDIR(st.st_mode)){
					closedir(dp);
					dir=argv[2];
					if((dp=opendir(dir))==NULL)
						perror(dir);
					while((d=readdir(dp))!=NULL){
                                		if(strcmp(d->d_name,".")!=0 && strcmp(d->d_name,"..")!=0){
                                        		sprintf(path,"%s/%s",dir,d->d_name);
                                        		if(lstat(path,&st)<0)
                                                		perror(path);
                                        		strcpy(ta.fname,d->d_name);
                                        		ta.mtime = st.st_mtime;
                                        		tarray[i++]=ta;
                                		}
                                	}
                                	Sort(tarray,i);
                                	print_array(tarray,i);
                                	free(tarray);
                                	closedir(dp);
				}
				else{
					printf("%s\n",argv[2]);
				}
			}
		}
		else{
			if(lstat(argv[1],&st)<0)
				perror(path);
			if(S_ISDIR(st.st_mode)){
				if((dp=opendir(dir))==NULL)
                        		perror(dir);
                		while((d=readdir(dp))!=NULL){
                			if(strcmp(d->d_name,".")!=0 && strcmp(d->d_name,"..")!=0){
                				printf("%s  ",d->d_name);
                		}}
                		printf("\n");
                		closedir(dp);
			}
			else{
				printf("%s\n",argv[1]);
			}
		}
	}
        exit(0);
}
void printStat(char *pathname, char *file, struct stat *st){
        //printf("%5ld ",st->st_blocks);
        printf("%c%s ",type(st->st_mode),perm(st->st_mode));
        printf("%3ld ",st->st_nlink);
        printf("%s %s ",getpwuid(st->st_uid)->pw_name,getgrgid(st->st_gid)->gr_name);
        printf("%9ld ",st->st_size);
        printf("%.12s ",ctime(&st->st_mtime)+4);
        printf("%s\n",file);
}
char type(mode_t mode){
        if(S_ISREG(mode))
                return('-');
        if(S_ISDIR(mode))
                return('d');
        if(S_ISCHR(mode))
                return('c');
        if(S_ISBLK(mode))
                return('b');
        if(S_ISLNK(mode))
                return('l');
        if(S_ISFIFO(mode))
                return('p');
        if(S_ISSOCK(mode))
                return('s');
}
char* perm(mode_t mode){
        int i;
        static char perms[10]="---------";
	for(int k=0; k<9;k++){
		perms[k]='-';
	}
	perms[9]='\0';
        for(i=0;i<3;i++){
                if(mode&(S_IRUSR>>i*3))
                        perms[i*3]='r';
                if(mode&(S_IWUSR>>i*3))
                        perms[i*3+1]='w';
                if(mode&(S_IXUSR>>i*3))
                        perms[i*3+2]='x';
        }
        return (perms);
}
void print_array(Table A[],int n){
        for(int i=0; i<n; i++){
                printf("%s  ",A[i].fname);
        }
        printf("\n");
}
void Sort(Table A[],int n)
{
        Table temp;
        for(int i=n-1; i>=1; i--)
        {
                for(int j=0; j<=i-1;j++)
                {
                        if(A[j].mtime < A[j+1].mtime)
                        {
                                temp = A[j+1];
                                A[j+1] = A[j];
                                A[j] = temp;
                        }
                }
        }
}
