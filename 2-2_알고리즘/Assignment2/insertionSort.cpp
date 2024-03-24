#include<stdio.h>
#include<stdlib.h>
#include<time.h>
void insertionSort(float A[], int n);
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
		insertionSort(floatArray, i); //정렬알고리즘을 수행한다.
		finish = clock(); //정렬알고리즘 수행 후 시간을 체크한다.

		printf("%d\n", (finish - start)); //정렬알고리즘의 수행시간을 출력한다.

		free(floatArray); //동적할당된 메모리 해제
	}
	return 0;
}
void insertionSort(float A[], int n)
{
	float temp; //교환할때 사용할 변수
	for (int i = 1; i < n; i++) //두번째 원소부터 마지막 원소까지 차례로 삽입하며 정렬할 것이므로  n-1번 반복된다. (정렬된 원소의 개수를 뜻한다.)
	{
		int j;
		temp = A[i]; //정렬될 i번째 원소를 저장해둔다.
		for (j = i - 1; j >= 0; j--)  //0번부터 i-1번째 원소까지는 정렬된 상태이므로 i-1번째 원소부터 새롭게 정렬될 값과 비교한다.
		{
			if (temp < A[j]) //j번째 원소가 더 크다면 정렬될 원소는 j보다 왼쪽에 있어야 하므로 j번째원소를 j+1번째 원소로 옮겨준다.
			{
				A[j + 1] = A[j];
			}
			else //j번째 원소가 더 작다면 정렬될 원소는 j번째 원소 다음인 j+1번째에 저장되면 된다.
			{
				break;
			}
		}
		A[j + 1] = temp;
	}
}