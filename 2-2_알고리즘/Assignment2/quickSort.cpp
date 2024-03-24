#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int partition(float A[], int left, int right);
void quickSort(float A[], int left, int right);
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
		quickSort(floatArray, 0, i - 1); //정렬알고리즘을 수행한다.
		finish = clock(); //정렬알고리즘 수행 후 시간을 체크한다.

		printf("%d\n", (finish - start)); //정렬알고리즘의 수행시간을 출력한다.

		free(floatArray); //동적할당된 메모리 해제
	}
	return 0;
}
void quickSort(float A[], int left, int right)
{
	int q;
	if (left < right)
	{
		q = partition(A, left, right);
		quickSort(A, left, q - 1);
		quickSort(A, q + 1, right);
	}
}
int partition(float A[], int left, int right)
{
	float pivot = A[right]; //피봇의 위치는 가장 오른쪽 원소로 지정한다.
	int i = left; //i는 가장 왼쪽 원소로 지정한다.
	int j = right - 1; //j는 피봇을 제외한 가장 오른쪽 원소로 지정한다.
	float temp;
	while (i <= j) //i와 j가 교차하지 않을 때까지 반복한다.
	{
		while (A[i] < pivot) //i번째 값이 피봇보다 작으면 다음데이터를 확인하기 위해 넘어간다. 
		{
			i++;
		}
		while (A[j] > pivot) //j번째 값이 피봇보다 크면 다음데이터를 확인하기 위해 넘어간다.
		{
			j--;
		}
		if (i <= j) //위의 과정 후 , i와 j가 교차하지 않았다면 i번재 데이터는 피봇보다 크고 j번째 데이터는 피봇보다 작을 것이다. 따라서 두 데이터를 교환해주면 배열의 0번째 원소부터 i번째 원소까지는 피봇보다 작은 데이터들이 있을 것이고, j번째 원소부터 배열의 마지막원소까지는 피봇보다 큰 데이터들이 있을 것이다.
		{

			temp = A[i];
			A[i] = A[j];
			A[j] = temp;
			i++;
			j--;
		}
	}
	//pivot과 i번째의 값을 서로 교환하여 피봇의 오른쪽은 피봇보다 큰값, 피봇의 왼쪽은 피봇보다 작은값으로 만들어 준다. 
	temp = A[i];
	A[i] = A[right];
	A[right] = temp;

	return i; //옮겨진 피봇의 위치를 반환한다.
}