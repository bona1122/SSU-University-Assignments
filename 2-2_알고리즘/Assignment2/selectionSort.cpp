#include<stdio.h>
#include<stdlib.h>
#include<time.h>
void selectionSort(float A[], int n);
int main()
{
	clock_t start, finish; //정렬 알고리즘의 수행시간을 측정하기 위한 변수들

	srand(1234); //시드값 설정

	for (int i = 1000; i <= 20000; i += 1000) //데이터의 개수를 1000부터 20000까지 1000단위로 반복한다.
	{
		float* floatArray = (float*)malloc(sizeof(float) * i); //배열을 동적할당한다.
		for (int j = 0; j < i; j++)
		{
			floatArray[j] = (rand() / (float)RAND_MAX * 2.0f) - 1; //-1~1사이의 실수 난수 배열을 생성한다.
		}

		start = clock(); //정렬알고리즘 수행 전 시간을 체크한다.
		selectionSort(floatArray, i); //정렬알고리즘을 수행한다.
		finish = clock(); //정렬알고리즘 수행 후 시간을 체크한다.

		printf("%d\n", (finish - start)); //정렬알고리즘의 수행시간을 출력한다.

		free(floatArray); //동적할당된 메모리 해제
	}
	return 0;
}
void selectionSort(float A[], int n)
{
	float temp; //교환할때 사용할 변수

	for (int i = n - 1; i > 0; i--) //가장 큰 값을 찾아 마지막원소부터 두번째 원소까지 정렬할것이므로 n-1번 반복한다.
	{
		for (int j = i - 1; j >= 0; j--) //결정된 원소의 앞부터 index = 0 까지 값 비교하면서 가장 큰값을 찾는다.
		{
			if (A[j] > A[i]) //정렬될 i번째의 값과 비교하여 j번째의 값이 더 크면 i번째와 교환한다.
			{
				temp = A[i];
				A[i] = A[j];
				A[j] = temp;
			}
		}
	}
}