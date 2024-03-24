// 주의사항
// 1. blockmap.h에 정의되어 있는 상수 변수를 우선적으로 사용해야 함
// 2. blockmap.h에 정의되어 있지 않을 경우 본인이 이 파일에서 만들어서 사용하면 됨
// 3. 필요한 data structure가 필요하면 이 파일에서 정의해서 쓰기 바람(blockmap.h에 추가하면 안됨)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include "blockmap.h"
// 필요한 경우 헤더 파일을 추가하시오.
int dd_read(int ppn,char*pagebuf);
int dd_write(int ppn,char*pagebuf);
int dd_erase(int pbn);
typedef struct {
	int lbn;
	int pbn;
} addressMapTableEntry;
typedef struct {
	addressMapTableEntry tableEntry[DATABLKS_PER_DEVICE];
} AddressMapTable;
AddressMapTable table;
int freeBlkPbn;

//
// flash memory를 처음 사용할 때 필요한 초기화 작업, 예를 들면 address mapping table에 대한
// 초기화 등의 작업을 수행한다. 따라서, 첫 번째 ftl_write() 또는 ftl_read()가 호출되기 전에
// file system에 의해 반드시 먼저 호출이 되어야 한다.
//
void ftl_open()
{
	//
	// address mapping table 초기화 또는 복구
	// free block's pbn 초기화
    	// address mapping table에서 lbn 수는 DATABLKS_PER_DEVICE 동일
	
	char readPage[PAGE_SIZE];
	int lbn = -1;
	int exist = FALSE;

	for (int i = 0; i < BLOCKS_PER_DEVICE; i++) {
		dd_read(i*PAGES_PER_BLOCK,readPage); //각 블록의 첫번째 페이지 가져오기 
		memcpy(&lbn, readPage + SECTOR_SIZE, 4); //첫번째 페이지 spare에서 lbn값 가져오기
		if (lbn != -1) //lbn이 -1이 아닌게 하나라도 있으면 처음테이블초기화하는경우이다.
			exist = TRUE;
	}
	if (exist == FALSE) { //테이블 처음 초기화하는 작업
		for (int i = 0; i < DATABLKS_PER_DEVICE; i++) {
			table.tableEntry[i].pbn = -1;
			table.tableEntry[i].lbn = i;
		}
		//free block 초기화. 마지막 블록을 먼저 free블록으로 놓는다.
		freeBlkPbn = DATABLKS_PER_DEVICE; 
	}
	else { //이미 파일 존재하는경우 테이블 복구 작업
		for (int i = 0; i < DATABLKS_PER_DEVICE; i++) { //먼저 pbn 다 -1해두고
			table.tableEntry[i].pbn = -1;
			table.tableEntry[i].lbn = i;
		}
		for (int i = 0; i < BLOCKS_PER_DEVICE; i++) { //file스캔하며 table 변경
			dd_read(i * PAGES_PER_BLOCK, readPage); //각블록 첫번째 페이지 읽고
			memcpy(&lbn, readPage + SECTOR_SIZE, 4); //첫번째 페이지 spare에서 lbn값 가져오기
			if (lbn != -1) { //lbn이 -1이 아니면 할당이 되어있는것.
				table.tableEntry[lbn].pbn = i;
			}
		}
		//복구끝나고 free블럭 초기화
		for (int i = 0; i < BLOCKS_PER_DEVICE; i++) { //블럭이 할당됐는지 확인
			int count = 0;
			for (int j = 0; j < DATABLKS_PER_DEVICE; j++) {
				if (table.tableEntry[j].pbn == i) {
					count++;
				}
			}
			if (count == 0) {
				freeBlkPbn = i;
				break;
			}
		}
	}
	return;
}
//
// 이 함수를 호출하는 쪽(file system)에서 이미 sectorbuf가 가리키는 곳에 512B의 메모리가 할당되어 있어야 함
// (즉, 이 함수에서 메모리를 할당 받으면 안됨)
//
void ftl_read(int lsn, char *sectorbuf)
{
	int lbn = lsn / PAGES_PER_BLOCK;
	int offset = lsn % PAGES_PER_BLOCK;
	int ppn = (table.tableEntry[lbn].pbn)*PAGES_PER_BLOCK + offset;
	char readPage[PAGE_SIZE];
	dd_read(ppn,readPage);
	memcpy(sectorbuf,readPage,SECTOR_SIZE);

	return;
}
//
// 이 함수를 호출하는 쪽(file system)에서 이미 sectorbuf가 가리키는 곳에 512B의 메모리가 할당되어 있어야 함
// (즉, 이 함수에서 메모리를 할당 받으면 안됨)
//
void ftl_write(int lsn, char *sectorbuf)
{
	//lsn번 page에 저장하고싶다.
	int lbn = lsn / PAGES_PER_BLOCK; //lbn구하고
	int pbn = table.tableEntry[lbn].pbn; //pbn구하고
	char readPage[PAGE_SIZE];
	char writePage[PAGE_SIZE];
	int offset = lsn % PAGES_PER_BLOCK; //몇번째 물리적 page에 저장할지
	int ppn = (table.tableEntry[lbn].pbn) * PAGES_PER_BLOCK + offset;
	char firstPage[PAGE_SIZE];
	int exist = -1;
	memset(firstPage, 0xFF, PAGE_SIZE); //처음 블록이 할당될 때 쓸 것
	memset(readPage, 0xFF, PAGE_SIZE);
	memset(writePage, 0xFF, PAGE_SIZE);

	//쓸 페이지 작성
	memcpy(writePage, sectorbuf, SECTOR_SIZE); //sector 쓰고
	memcpy(writePage + SECTOR_SIZE + 4, &lsn, 4); //spare의 두번째 4B에 lsn저장
	
	if (pbn == -1) { //블록에 처음 쓰여지는 것.

		for (int i = 0; i < BLOCKS_PER_DEVICE; i++) { //모든 블럭 스캔해서 빈블록 하나 할당하자
			int count = 0;
			if (i != freeBlkPbn) { //free블럭 아닌것중에 고르기
				for (int j = 0; j < DATABLKS_PER_DEVICE; j++) {
					if (table.tableEntry[j].pbn == i) { //i번째 블록이 다른 곳에 할당 중인지 확인. 할당중이라면
						count++;
					}
				}
				if (count == 0) {
					table.tableEntry[lbn].pbn = i;
					break;
				}
			}
		}
		pbn = table.tableEntry[lbn].pbn;
		ppn = (table.tableEntry[lbn].pbn) * PAGES_PER_BLOCK + offset; //ppn계산
		if(offset == 0){
			memcpy(writePage + SECTOR_SIZE, &lbn,4);
			dd_write(ppn,writePage);
		}
		else{
			dd_write(ppn,writePage);
			memcpy(firstPage + SECTOR_SIZE, &lbn, 4);
			dd_write((table.tableEntry[lbn].pbn)*PAGES_PER_BLOCK,firstPage);
		}
		//dd_write(ppn, writePage); 

		//처음 블록에 쓰여지는 것이니까 해당 블록의 첫페이지의 spare영역에 lbn저장해야됨.
		//memcpy(firstPage + SECTOR_SIZE, &lbn, 4);
		//dd_write((table.tableEntry[lbn].pbn) * PAGES_PER_BLOCK,firstPage); //블록첫페이지에 씀
	}
	else {	//이미 블록이 할당되어있음
		dd_read(ppn, readPage); //페이지 읽어옴.페이지의 spare area의 lsn을 읽어서 판단해야됨.
		memcpy(&exist, readPage + SECTOR_SIZE + 4, 4); //exist에 lsn값 읽어옴
		if (exist == -1) { //해당 페이지가 비어있는경우
			if (offset == 0) { //빈 페이지가 첫번째 페이지인 경우
				memcpy(writePage + SECTOR_SIZE, &lbn, 4); //lbn값까지 포함시키고 쓰기
				dd_write(ppn, writePage);
			}
			else {
				dd_write(ppn, writePage); //빈페이지에 쓰기
			}
		}
		else { //이미 쓰여져 있는경우 ->복잡
			//free블록에 갱신하자
			for (int i = 0; i < PAGES_PER_BLOCK; i++) { //기존블록 안겹치는 부분 복사
				if (i != offset) {
					dd_read(pbn * PAGES_PER_BLOCK + i, readPage);
					dd_write(freeBlkPbn * PAGES_PER_BLOCK + i, readPage);
				}
			}
			if (offset == 0) {
				memcpy(writePage + SECTOR_SIZE, &lbn, 4); //lbn값까지 포함시키고 쓰기
				dd_write(freeBlkPbn * PAGES_PER_BLOCK + offset, writePage); //free블록에 sector영역 저장
			}
			else {
				dd_write(freeBlkPbn * PAGES_PER_BLOCK + offset, writePage); //free블록에 sector영역 저장
			}
			table.tableEntry[lbn].pbn = freeBlkPbn;
			dd_erase(pbn);	//기존 블록 지우기
			freeBlkPbn = pbn; //free 블록 변경 
		}
	}
	return;
}
void ftl_print()
{
	printf("lbn pbn\n");
	for(int i = 0; i < DATABLKS_PER_DEVICE; i++) {
		printf("%d %d\n", table.tableEntry[i].lbn,table.tableEntry[i].pbn);
	}
	printf("free block's pbn=%d\n", freeBlkPbn);
	return;
}
