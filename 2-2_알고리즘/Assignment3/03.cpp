#include<stdio.h>
void LinearProbing(int hashTable[], int num); //선형조사를 통해 해시테이블에 데이터를 저장하는 함수
int main()
{
	int array[8] = { 10,20,30,40,33,46,50,60 }; //테이블에 저장할 데이터 배열 정의

	int hashTable[13] = { 0 }; //해시테이블 초기화

	for (int i = 0; i < 8; i++) //array에 있는 값을 차례로 hasbTable에 저장 
	{
		printf("<%d 저장 시작>\n", array[i]);
		LinearProbing(hashTable, array[i]);

		for (int j = 0; j < 13; j++)
		{
			printf("%d\n", hashTable[j]); //hashTable 데이터 출력
		}
		printf("\n\n");
	}

	return 0;
}
void LinearProbing(int hashTable[],int num)
{
	int first = num % 13; //처음 저장될 곳 위치 저장
	
	for (int k = first;  ; )
	{
		if (hashTable[k] == 0) // 테이블에 0이 저장되어있다면(비어있다면)
		{
			hashTable[k] = num; //해당 위치에 num값 저장
			return; //끝내기
		}
		else
		{
			k = (k + 1) % 13; //해시테이블에 이미 저장되어있다면 다음것 조사-선형조사
			if (k == first) //다음 것이 원래 저장할 곳과 같다면
			{
				printf("테이블이 가득 찼습니다.");
				return; //끝내기
			}
		}
	}
}