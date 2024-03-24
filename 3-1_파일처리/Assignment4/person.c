#include <stdio.h>
#include "person.h"
#include <string.h>
#include <stdlib.h>
void writePage(FILE* fp,const char* pagebuf,int pagenum);
void readPage(FILE* fp,char* pagebuf,int pagenum);
typedef struct _node node;
struct _node {
    int delete_page;
    int delete_record;
    node* next;
};
typedef struct _list{
        node * head;
        node* tail;
        int size;
}List;
void initial_list(List* lp) {
    lp->head = (node*)malloc(sizeof(node));
    lp->tail = (node*)malloc(sizeof(node));
    lp->head->next = lp->tail;
    lp->tail->next = lp->tail;
    lp->size = 0;
}

void addNode(List* lp, int page, int record) {
    node* newnode = (node*)malloc(sizeof(node)); 
    newnode->delete_page = page;
    newnode->delete_record = record;
    newnode->next = lp->head->next;
    lp->head->next = newnode;
}
void printList(List* lp) {
    node* curp = lp->head->next; //첫노드
    while (curp != lp->tail) {
        printf("page: %d, record: %d\n", curp->delete_page, curp->delete_record);
        curp = curp->next;
    }
}
void deleteNode(List* lp, int page, int record,FILE* fp) {
    char readP[PAGE_SIZE];
    int offset = 0;
    node* curp = lp->head; //헤드노드가리킴
	//printf("head->next->deletepage = %d",curp->next->delete_page);
	//printf("here!! page: %d, record:%d\n",page,record);
    while(1) {
        if (curp->next == lp->tail){ //노드의 다음거가 마지막이면
            break;
	}
        if ((curp->next->delete_page == page) && (curp->next->delete_record == record)) { //노드의 다음거가 지울거라면
            node* store;
            store = curp->next;
	

	    readPage(fp,readP,curp->next->delete_page);
	    memcpy(&offset,readP+4+8*(curp->next->delete_record),4);
	    int page = 0;
	    int record = 0;
	    memcpy(&page,readP+HEADER_AREA_SIZE+offset+1,4);
	    memcpy(&record,readP+HEADER_AREA_SIZE+offset+1+4,4);

	   if(curp == lp->head){
		fseek(fp,8,SEEK_SET);
		fwrite(&page,4,1,fp);
		fwrite(&record,4,1,fp);
	   }
	   else{
	    readPage(fp,readP,curp->delete_page);
	    memcpy(&offset,readP+4+8*(curp->delete_record),4);
	    memcpy(readP+HEADER_AREA_SIZE+offset+1,&page,4);
	    memcpy(readP+HEADER_AREA_SIZE+offset+1+4,&record,4);
	    writePage(fp,readP,curp->delete_page);}

            curp->next = curp->next->next;
            free(store);
	    lp->size--;
            break;
        }
        curp = curp->next;
    }
}
List list;


void writePage(FILE* fp, const char* pagebuf, int pagenum);
void plusfield(char*recordbuf,const char* field,int*sum);
void readPage(FILE* fp, char* pagebuf, int pagenum);
void unpack(const char* recordbuf, Person* p);
void delete(FILE* fp,const char*id){
	//printf("id = %s",id);
	Person p;
	int exist_page=0;
	char readP[PAGE_SIZE];
	char recordbuf[MAX_RECORD_SIZE];
	memset(recordbuf,0,MAX_RECORD_SIZE);
	int recent_dP = 0;
	int recent_dR = 0;
	int offset = 0;
	int length = 0;
	int find = 0;
	char read_mark;
	char delete_mark = '*';
	int record_num = 0;
	fseek(fp,0,SEEK_SET);
	fread(&exist_page,4,1,fp);
	fseek(fp,8,SEEK_SET);
	fread(&recent_dP,4,1,fp);
	fread(&recent_dR,4,1,fp);
	
	
	for(int i=0;i<exist_page;i++){
		readPage(fp,readP,i);//bring page
		memcpy(&record_num,readP,4); //bring record_num in page
		//printf("record_num : %d",record_num);
		for(int j=0;j<record_num;j++){
			memcpy(&offset,readP+4+j*8,4);//bring startoffset
			memcpy(&length,readP+4+j*8+4,4);//bring length
			
			//printf("offset : %d\n",offset);
			//printf("length: %d\n",length);
			memcpy(recordbuf,readP+HEADER_AREA_SIZE+offset,length);
			
			memcpy(&read_mark,readP+HEADER_AREA_SIZE+offset,1);
			if(read_mark != '*'){
				unpack(recordbuf,&p);
				if(strcmp(p.id,id)==0){
					//printf("find");
					memcpy(readP+HEADER_AREA_SIZE+offset,&delete_mark,1);
					memcpy(readP+HEADER_AREA_SIZE+offset+1,&recent_dP,4);
					memcpy(readP+HEADER_AREA_SIZE+offset+1+4,&recent_dR,4);
					writePage(fp,readP,i);
					//revise header
					fseek(fp,8,SEEK_SET);
					fwrite(&i,4,1,fp);
					//printf("j=%d",j);
					fwrite(&j,4,1,fp);
					find = 1;
					addNode(&list,i,j);
					break;
				}
			}
		}
		if(find==1)
			break;
	}
		
}
void unpack(const char* recordbuf,Person* p)
{
	char buf[MAX_RECORD_SIZE];
	memcpy(buf,recordbuf,MAX_RECORD_SIZE);
	char *ptr=strtok(buf,"#");
	strcpy(p->id,ptr);
	ptr = strtok(NULL,"#");
	strcpy(p->name,ptr);
	ptr = strtok(NULL,"#");
        strcpy(p->age,ptr);
	ptr = strtok(NULL,"#");
        strcpy(p->addr,ptr);
	ptr = strtok(NULL,"#");
        strcpy(p->phone,ptr);
	ptr = strtok(NULL,"#");
        strcpy(p->email,ptr);
}
void pack(char*recordbuf,const Person* p)
{
	int sum=0;
	
	plusfield(recordbuf,p->id,&sum);
	plusfield(recordbuf,p->name,&sum);
	plusfield(recordbuf,p->age,&sum);
	plusfield(recordbuf,p->addr,&sum);
	plusfield(recordbuf,p->phone,&sum);
	plusfield(recordbuf,p->email,&sum);

	recordbuf[sum]='\0';
}
void plusfield(char*recordbuf,const char* field,int*sum){
	char deli[2]="#";
	strncpy(recordbuf+*sum,field,strlen(field));
	(*sum)+=strlen(field);
	strncpy(recordbuf+*sum,deli,strlen(deli));
	(*sum)++;
}
void readPage(FILE* fp,char* pagebuf,int pagenum){
	fseek(fp,16+pagenum*PAGE_SIZE,SEEK_SET);
	fread(pagebuf,PAGE_SIZE,1,fp);
}
void writePage(FILE*fp,const char* pagebuf,int pagenum){
	fseek(fp, 16 + pagenum * PAGE_SIZE, SEEK_SET); //페이지 위치로 오프셋 이동
	fwrite(pagebuf,PAGE_SIZE,1,fp);
}
void add(FILE*fp,const Person* p){
	char recordbuf[MAX_RECORD_SIZE];
	char pagebuf[PAGE_SIZE];
	char readP[PAGE_SIZE];
	int data = 0;
	memset(recordbuf,'\0',MAX_RECORD_SIZE);
	pack(recordbuf,p);
	
	int exist_page = 0;
	int exist_record = 0;
	int pagenum = 0;
	int recordnum = 0;
	
	fseek(fp,0,SEEK_SET);
	fread(&exist_page,sizeof(int),1,fp);
	fread(&exist_record,4,1,fp);
	
	if(exist_page==0){	//no page, add page, -->first Write
		//header record revise
		fseek(fp,0,SEEK_SET);
		exist_page = 1;
		fwrite(&exist_page,4,1,fp);
		exist_record = 1;
		fwrite(&exist_record,4,1,fp);
	
		//pagewrite
		memset(pagebuf,'\0',PAGE_SIZE);
		data = 1;
		memcpy(pagebuf,&data,4);
		data = 0;
		memcpy(pagebuf+4,&data,4);
		data = strlen(recordbuf);
		memcpy(pagebuf+4+4,&data,4);
		memcpy(pagebuf+HEADER_AREA_SIZE,recordbuf,sizeof(recordbuf));
		writePage(fp,pagebuf,0);
	}
	else{
		int final = 0;
		int deleted = 0;
		fseek(fp,8,SEEK_SET);
		fread(&deleted,sizeof(int),1,fp);
		if(deleted != -1){//confirm he removed record exist
			int delete_inPage = 0;
			int delete_record = 0;
			fseek(fp,8,SEEK_SET);
			fread(&delete_inPage,4,1,fp);
			fseek(fp,12,SEEK_SET);
			fread(&delete_record,4,1,fp);
			
			int updateP = delete_inPage;
			int updateR = delete_record;
			int length = 0;
			int offset = 0;
			while(1){
				readPage(fp,readP,updateP);
				memcpy(&offset,readP+4+8*updateR,4);
				memcpy(&length,readP+4+8*updateR+4,4);

				if(length>=strlen(recordbuf)){
					//printf("herepage: %d, herepage: %d\n",updateP,updateR);
					//store
					//memcpy(&offset,readP+4+8*updateR,4);
					deleteNode(&list,updateP,updateR,fp);
					readPage(fp,readP,updateP);
					memcpy(readP+HEADER_AREA_SIZE+offset,recordbuf,strlen(recordbuf));
					//printf("overwite page: %d, overwite page: %d
					writePage(fp,readP,updateP);
					final = 1;
					break;	
				}
				int next=0;
				memcpy(&next,readP+HEADER_AREA_SIZE+offset+1,4);
				if(next!=-1){
					updateP = next;
					memcpy(&updateR,readP+HEADER_AREA_SIZE+offset+1+4,4);
				}
				else
					break;
			}
		}
		if(final==0){
			//store end.
			int stored_record = 0;
			int remain = 0;
			int size = 0;

			fseek(fp,0,SEEK_SET);
			fread(&exist_page,4,1,fp);
			final = exist_page-1;
			readPage(fp,readP,exist_page-1);//final page
			memcpy(&stored_record,readP,4);
			
			memcpy(&size,readP+4+(stored_record-1)*8,4);//final record's offset
			int plus = 0;
			memcpy(&plus,readP+4+(stored_record-1)*8+4,4);//final record's length
			int final_end_offset = size+plus;
			size=final_end_offset+HEADER_AREA_SIZE;
			remain = PAGE_SIZE - size;
			if(remain>=strlen(recordbuf) && (HEADER_AREA_SIZE-4)/8>stored_record)
			{
				//add end of final page	
				int update_record_num = stored_record+1;
				readPage(fp,pagebuf,final);
				memcpy(pagebuf+HEADER_AREA_SIZE+final_end_offset,recordbuf,strlen(recordbuf));
				memcpy(pagebuf,&update_record_num,4);
				memcpy(pagebuf+4+stored_record*8,&final_end_offset,4);
				int new_length = strlen(recordbuf);
				memcpy(pagebuf+4+stored_record*8+4,&new_length,4);
				writePage(fp,pagebuf,final);	
				
				//header updata
				fseek(fp,4,SEEK_SET);
				exist_record++;
				fwrite(&exist_record,4,1,fp);
			}	
			else{//new page add
				//printf("???");
				fseek(fp,0,SEEK_SET);
				exist_page++;
				fwrite(&exist_page,4,1,fp);
				exist_record++;
				fwrite(&exist_record,4,1,fp);

				memset(pagebuf,'\0',PAGE_SIZE);
				data = 1;
				memcpy(pagebuf,&data,4);
				data = 0;
				memcpy(pagebuf+4,&data,4);
				data = strlen(recordbuf);
				memcpy(pagebuf+4+4,&data,4);
				memcpy(pagebuf+HEADER_AREA_SIZE,recordbuf,data);
				writePage(fp,pagebuf,exist_page-1);
			}
			
		}
	}
}
void addNodeatLast(List* lp,int page,int record){
	node* newnode = (node*)malloc(sizeof(node));
	newnode->delete_page = page;
	newnode->delete_record = record;
	newnode->next = lp->tail;

	node* curp = lp->head;
	while(1){
		if(curp->next == lp->tail)
			break;
		curp=curp->next;
	}
	curp->next = newnode;
}
void makeList(FILE* fp,List* lp){
	char readP[PAGE_SIZE];
	int delete_page = 0;
	int delete_record = 0;
	fseek(fp,8,SEEK_SET);
	fread(&delete_page,4,1,fp);
	fread(&delete_record,4,1,fp);

	int page = delete_page;
	int record = delete_record;
	int offset = 0;
	while(1){
		if(page==-1){
			addNodeatLast(&list,-1,-1);
			break;
		}
		addNodeatLast(&list,page,record);	
		readPage(fp,readP,page);
		memcpy(&offset,readP+4+8*record,4);
		
		memcpy(&page,readP+HEADER_AREA_SIZE+offset+1,4);
		memcpy(&record,readP+HEADER_AREA_SIZE+offset+1+4,4);
	}
}
void freeList(List* lp){
	node* curp = lp->head;
	node* nextnode = curp->next;
	while(1){
		if(curp==lp->tail) break;
		free(curp);
		curp = nextnode;
		nextnode = nextnode->next;
	}
	free(curp);
}
int main(int argc,char* argv[]){
	FILE* fp;
	char buf[30];
	Person p;
	int dataPageNum = 0;
	int recordNum = 0;
	
	//fp = fopen(argv[2],"w+");
	//int header = 0;
	//fwrite(&header,sizeof(int),1,fp);
	//int k = 1;
	//fread(&k,sizeof(int),1,fp);
	//printf("k:%d\n",k);
	initial_list(&list);
	fp = fopen(argv[2],"r+");
	if(fp==NULL){
		//printf("first???");
		fp = fopen(argv[2],"w+");
		int header = 0;
		fwrite(&header,sizeof(int),1,fp);
		fwrite(&header,4,1,fp);
		header = -1;
		fwrite(&header,sizeof(int),1,fp);
		fwrite(&header,4,1,fp);
	}
	makeList(fp,&list);
		
	if(*(argv[1])=='a'){
		for(int i=3;i<9;i++){
			if(i==3){
				strcpy(p.id,argv[i]);
			}
			else if(i==4){
				strcpy(p.name,argv[i]);
			}
			else if(i==5){
				strcpy(p.age,argv[i]);
			}
			else if(i==6)
				strcpy(p.addr,argv[i]);
			else if(i==7)
				strcpy(p.phone,argv[i]);
			else
				strcpy(p.email,argv[i]);
		}
		add(fp,&p);
	}
	else{
		//printf("%s",argv[3]);
		delete(fp,argv[3]);
	}
	
	/*int delete_page = 0;
	int delete_record = 0;
	fseek(fp,8,SEEK_SET);
	fread(&delete_page,4,1,fp);
	fread(&delete_record,4,1,fp);
	
	printf("delete_page: %d\n",delete_page);
	printf("delete_record: %d\n",delete_record);

	printList(&list);*/
	freeList(&list);
	
	fclose(fp);
	return 1;
}
