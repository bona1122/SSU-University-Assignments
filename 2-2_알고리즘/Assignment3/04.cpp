#include<stdio.h>
void QuadraticProbing(int hashTable[], int num); //이차원조사를 통해 해시테이블에 데이터를 저장하는 함수
int main()
{
	int array[8] = { 10,20,30,40,33,46,50,60 }; //테이블에 저장할 데이터 배열 정의

	int hashTable[13] = { 0 }; //해시테이블 초기화

	for (int i = 0; i < 8; i++) //array에 있는 값을 차례로 hasbTable에 저장 
	{
		printf("<%d 저장 시작>\n", array[i]);
		QuadraticProbing(hashTable, array[i]);

		for (int j = 0; j < 13; j++)
		{
			printf("%d\n", hashTable[j]); //hashTable 데이터 출력
		}
		printf("\n\n");
	}
	return 0;
}
void QuadraticProbing(int hashTable[],int num)
{
	for (int j = 0; ; j++)
	{
		if (j == 13) //j가 13이라면 저장을 총 12번시도한 결과이므로 테이블이 가득 찼을 것이다.
		{
			printf("테이블이 가득 찼습니다.");
		}
		int store = (num + j * j) % 13; //이차원 조사를 통해 저장할 인덱스를 정한다. 
		if (hashTable[store] == 0) // 테이블이 비어있다면
		{
			hashTable[store] = num; //해당 위치에 num값 저장
			return; //끝내기
		}

	}
}