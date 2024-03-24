#include<stdio.h>
#include<stdlib.h>
typedef struct Table { //해시테이블을 이루는 노드 정의
	int key;
	struct Table* p;
}Table;
void ChainedHashInsert(Table hashTable[], int num);
int main()
{
	int array[8] = { 10,20,30,40,33,46,50,60 }; //테이블에 저장할 데이터 배열 정의

	Table hashTable[13]; //해시테이블 준비
	for (int i = 0; i < 13; i++) //해시테이블 초기화
	{
		hashTable[i].key = NULL;
		hashTable[i].p = NULL;
	}

	for (int i = 0; i < 8; i++) //array에 있는 값을 차례로 hasbTable에 저장 
	{
		printf("<%d 저장 시작>\n", array[i]);
		ChainedHashInsert(hashTable, array[i]); //체이닝을 이용하여 데이터 저장

		for (int j = 0; j < 13; j++)  //저장 후 결과 출력
		{
			if (hashTable[j].p == NULL) //해시테이블 j번 인덱스가 아무것도 가리키지 않는다면(j번인덱스에 데이터가 없다면)
			{
				printf("없음");
			}
			else //j번인덱스에 데이터가 있다면
			{
				Table* start = hashTable[j].p; //처음 출력할 노드를 가리킨다.
				while (true)
				{
					printf("%d ", start->key); //데이터 출력
					if (start->p != NULL) 
					{ start = start->p; } //출력된 노드가 가리키는 다음 노드가 있다면 start를 다음노드로 바꿔준다.
					else //출력된 노드가 가리키는 다음 노드가 없다면 그만한다.
					{
						break;
					}
				}
			}
			printf("\n");
		}
		printf("\n\n");
	}
	return 0;
}
void ChainedHashInsert(Table hashTable[], int num) 
{
	int store = num % 13; //해시테이블에 저장될 위치를 구한다.
	if (hashTable[store].p == NULL) //저장될 곳에 데이터가 없다면(해당인덱스가 가리키는 노드가 없다면)
	{
		hashTable[store].p = (Table*)calloc(1, sizeof(Table)); //새 노드를 추가해준다.
		hashTable[store].p->key = num; //새노드에 num값을 저장한다.
	}
	else //저장될 곳에 데이터가 이미 있다면(해당인덱스가 가리키는 노드가 있다면)
	{
		Table* newNode = (Table*)calloc(1, sizeof(Table)); //새 노드 준비
		newNode->key = num; //새 노드에 num값 저장.
		newNode->p = hashTable[store].p; //새노드가 저장될 곳이 가리키는 노드를 가리키게 한다.(새 노드를 가장 앞에 두기 위해)
		hashTable[store].p = newNode; //저장될 곳이 새노드를 가리키게 한다.(새 노드가 가장 앞에 오게 됨)
	}
}