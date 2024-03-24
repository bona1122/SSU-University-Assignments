#include<stdio.h>
#include<stdlib.h>
#include<time.h>
void bubbleSort(float A[], int n);
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
		bubbleSort(floatArray, i); //정렬알고리즘을 수행한다.
		finish = clock(); //정렬알고리즘 수행 후 시간을 체크한다.

		printf("%d\n", (finish - start)); //정렬알고리즘의 수행시간을 출력한다.

		free(floatArray); //동적할당된 메모리 해제
	}
	return 0;
}
void bubbleSort(float A[], int n)
{
	float temp; //배열 안의 값을 교환할 때 사용할 변수

	for (int i = n - 1; i >= 1; i--) //배열의 마지막 원소부터 1번 원소까지 결정될 것이다.(정렬된 것의 개수를 뜻하게 된다.)
	{
		for (int j = 0; j <= i - 1; j++) //배열의 0번원소부터 정렬이 완료된 i번원소 전까지 인접한 값을 비교한다.
		{
			if (A[j] > A[j + 1]) //인접한 원소와 비교하여 더 큰 값을 오른쪽으로 옮긴다.
			{
				temp = A[j + 1];
				A[j + 1] = A[j];
				A[j] = temp;
			}
		}
	}
}