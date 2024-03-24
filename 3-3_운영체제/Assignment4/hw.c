#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define BUFFER_SIZE 1024

typedef struct _have_flag {
	unsigned int num : 32;
	unsigned int flag : 1;
}have_flag;

void print_OPT();
void print_FIFO();
void print_LRU();
void print_Second_Chance();
int hit(int ref,int* frame);
int hit_for_SC(int ref,have_flag* frame);
int search_vic(int*refer,int ref_pos,int* frame);
int search_vic_LRU(int*refer,int ref_pos,int* frame);

int refer[30]={0};
int ref_num = 0;
int frame_num=0;
char refer_string[BUFFER_SIZE];
int main(){
	char buf[BUFFER_SIZE];
	char file_name[20];
	FILE* fp;
	
	printf("input file name: ");
	scanf("%s",file_name);

	if((fp=fopen(file_name,"r"))==NULL){
		fprintf(stderr,"fopen error for %s\n",file_name);
		exit(1);
	}
	fgets(buf,BUFFER_SIZE,fp);
	buf[strlen(buf)-1]='\0';
	frame_num = atoi(buf);

	memset(buf,'\0',BUFFER_SIZE);

	fgets(buf,BUFFER_SIZE,fp);
	strcpy(refer_string,buf);
	buf[strlen(buf)-1]='\0';


	char* ptr = strtok(buf," ");
	while(ptr!=NULL){
		refer[ref_num++]=atoi(ptr);
		ptr = strtok(NULL," ");
	}
	
	printf("\n******************************************************\n");
	print_OPT();
	printf("\n******************************************************\n");
	print_FIFO();
	printf("\n******************************************************\n");
	print_LRU();
	printf("\n******************************************************\n");
	print_Second_Chance();
	
	return 0;
}
void print_Second_Chance(){
	printf("Used method : Second-Chance\n");
        printf("page reference string : %s\n",refer_string); 
        printf("%13s","frame");
        printf("%4d",1);
        for(int i=2;i<=frame_num;i++){
               printf("%10d",i);
        }
        printf("\t\t%s\n","page fault");
        printf("time\n");


	have_flag* frame_arr = (have_flag*)calloc(frame_num,sizeof(have_flag));
	int page_fault = 0;
	char hit_ch = 'F';
	int empty = 0;
	int frame_ptr = 0;

	for(int i=0;i<ref_num;i++){	//each reference
		if(hit_for_SC(refer[i],frame_arr)){	//hit 
			hit_ch = '\0';
			for(int k=0;k<frame_num;k++){
				if(frame_arr[k].num == refer[i]){
					frame_arr[k].flag = 1;
				}
			}
		}
		else{
			hit_ch = 'F';
			page_fault++;
			for(int j=0;j<frame_num;j++){	//search empty pos
				if(frame_arr[j].num==0){	//put empty
					frame_arr[j].num=refer[i];
					empty = 1;
					break;	
				}
			}
			if(empty == 1){
				empty = 0;
			}
			else{	
				while(1){
					if(frame_arr[frame_ptr%frame_num].flag ==0){
						frame_arr[frame_ptr%frame_num].num = refer[i];
						frame_ptr++;
						break;
					}
					else{
						frame_arr[frame_ptr%frame_num].flag = 0;
						frame_ptr++;
					}
				}
			}
		}

		printf("%-13d",i+1);
		if(frame_arr[0].num==0){printf("\t");}
		else{printf("%4d",frame_arr[0].num);}
		for(int i=1;i<frame_num;i++){
			if(frame_arr[i].num==0) printf("\t");
			else printf("%10d",frame_arr[i].num);
		}
		printf("\t\t%c\n",hit_ch);
	}
	printf("Number of page faults : %d times\n",page_fault);
	free(frame_arr);


}
void print_LRU(){
	printf("Used method : LRU\n");
        printf("page reference string : %s\n",refer_string); 
        printf("%13s","frame");
        printf("%4d",1);
        for(int i=2;i<=frame_num;i++){
               printf("%10d",i);
        }
        printf("\t\t%s\n","page fault");
        printf("time\n");


	int page_fault = 0;
	char hit_ch = 'F';
	int* frame_arr = (int*)calloc(frame_num,sizeof(int));
	int empty = 0;

	for(int i=0;i<ref_num;i++){	//each reference
		if(hit(refer[i],frame_arr)){	//hit 
			hit_ch = '\0';
		}
		else{
			hit_ch = 'F';
			page_fault++;
			for(int j=0;j<frame_num;j++){	//search empty pos
				if(frame_arr[j]==0){	//put empty
					frame_arr[j]=refer[i];
					empty = 1;
					break;	
				}
			}
			if(empty == 1){
				empty = 0;
			}
			else{	
				frame_arr[search_vic_LRU(refer,i,frame_arr)] =  refer[i]; 
			}
		}

		printf("%-13d",i+1);
		if(frame_arr[0]==0){printf("\t");}
		else{printf("%4d",frame_arr[0]);}
		for(int i=1;i<frame_num;i++){
			if(frame_arr[i]==0) printf("\t");
			else printf("%10d",frame_arr[i]);
		}
		printf("\t\t%c\n",hit_ch);
	}
	printf("Number of page faults : %d times\n",page_fault);
	free(frame_arr);
}
void print_FIFO(){
	printf("Used method : FIFO\n");
        printf("page reference string : %s\n",refer_string);
        printf("%13s","frame");
        printf("%4d",1);
        for(int i=2;i<=frame_num;i++){
               printf("%10d",i);
        }
        printf("\t\t%s\n","page fault");
        printf("time\n");


        int page_fault = 0;
        char hit_ch = 'F';
        int* frame_arr = (int*)calloc(frame_num,sizeof(int));
        int empty = 0;
	int frame_ptr = 0;

	for(int i=0;i<ref_num;i++){     //each reference
                if(hit(refer[i],frame_arr)){    //hit 
                        hit_ch = '\0';
                }
                else{	//no hit
                        hit_ch = 'F';
                        page_fault++;
                        for(int j=0;j<frame_num;j++){   //search empty pos
                                if(frame_arr[j]==0){    //put empty
                                        frame_arr[j]=refer[i];
                                        empty = 1;
                                        break;
                                }
                        }
                        if(empty == 1){
                                empty = 0;
                        }
                        else{
				frame_arr[frame_ptr%frame_num] = refer[i];
				frame_ptr++;
                        }
                }

                printf("%-13d",i+1);
                if(frame_arr[0]==0){printf("\t");}
                else{printf("%4d",frame_arr[0]);}
                for(int i=1;i<frame_num;i++){
                        if(frame_arr[i]==0) printf("\t");
                        else printf("%10d",frame_arr[i]);
                }
                printf("\t\t%c\n",hit_ch);
        }
        printf("Number of page faults : %d times\n",page_fault);
	free(frame_arr);
}
void print_OPT(){
	printf("Used method : OPT\n");
        printf("page reference string : %s\n",refer_string); 
        printf("%13s","frame");
        printf("%4d",1);
        for(int i=2;i<=frame_num;i++){
               printf("%10d",i);
        }
        printf("\t\t%s\n","page fault");
        printf("time\n");


	int page_fault = 0;
	char hit_ch = 'F';
	int* frame_arr = (int*)calloc(frame_num,sizeof(int));
	int empty = 0;


	for(int i=0;i<ref_num;i++){	//each reference
		if(hit(refer[i],frame_arr)){	//hit 
			hit_ch = '\0';
		}
		else{
			hit_ch = 'F';
			page_fault++;
			for(int j=0;j<frame_num;j++){	//search empty pos
				if(frame_arr[j]==0){	//put empty
					frame_arr[j]=refer[i];
					empty = 1;
					break;	
				}
			}
			if(empty == 1){
				empty = 0;
			}
			else{	
				frame_arr[search_vic(refer,i,frame_arr)] =  refer[i];   
			}
		}

		printf("%-13d",i+1);
		if(frame_arr[0]==0){printf("\t");}
		else{printf("%4d",frame_arr[0]);}
		for(int i=1;i<frame_num;i++){
			if(frame_arr[i]==0) printf("\t");
			else printf("%10d",frame_arr[i]);
		}
		printf("\t\t%c\n",hit_ch);
	}
	printf("Number of page faults : %d times\n",page_fault);
	free(frame_arr);
}
int search_vic(int*refer,int ref_pos,int* frame){
	int pos;
	int* pos_arr = (int*)calloc(frame_num,sizeof(int));
	int idx = 0;
	
	for(int i=0; i<frame_num;i++){
		for(int j=ref_pos+1;j<ref_num;j++){
			if(refer[j]==frame[i]){
				pos_arr[idx] = j;
				break;
			}
		}
		idx++;
	}
	for(int i=0;i<frame_num;i++){
		if(pos_arr[i]==0){
			pos = i;
			free(pos_arr);
			return pos;
		}
	}
	
	int max = pos_arr[0];
	for(int i=0;i<frame_num;i++){
		if(pos_arr[i] > max) max = pos_arr[i];
	}
	for(int i=0;i<frame_num;i++){
		if(pos_arr[i] == max){
			pos = i;
			break;
		}
	}
	free(pos_arr);
	return pos;
}
int hit(int ref,int* frame){
	for(int i=0;i<frame_num;i++){
		if(ref==frame[i])
			return 1;
	}
	return 0;
}
int search_vic_LRU(int*refer,int ref_pos,int* frame){
	int pos;
	int* pos_arr = (int*)calloc(frame_num,sizeof(int));
	int idx = 0;

	for(int i=0;i<frame_num;i++){
		pos_arr[i] = -1;
	}
	
	for(int i=0; i<frame_num;i++){
		for(int j=ref_pos-1;j>=0;j--){
			if(refer[j]==frame[i]){
				pos_arr[idx] = j;
				break;
			}
		}
		idx++;
	}

	for(int i=0;i<frame_num;i++){
		if(pos_arr[i]==(-1)){
			pos = i;
			free(pos_arr);
			return pos;
		}
	} 
	
	int min = pos_arr[0];
	for(int i=0;i<frame_num;i++){
		if(pos_arr[i] < min) min = pos_arr[i];
	}
	for(int i=0;i<frame_num;i++){
		if(pos_arr[i] == min){
			pos = i;
			break;
		}
	}
	free(pos_arr);
	return pos;
}
int hit_for_SC(int ref,have_flag* frame){
	for(int i=0;i<frame_num;i++){
		if(ref==frame[i].num)
			return 1;
	}
	return 0;
}
