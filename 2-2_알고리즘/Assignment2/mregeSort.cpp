#include<stdio.h>
#include<stdlib.h>
#include<time.h>
void mergeSort(float A[], int p, int r);
void merge(float A[], int p, int q, int r);
int num = 0; //merge함수에서 동적할당 시 배열의 데이터 개수를 알아야 하므로 전역변수로 선언해준다.
int main()
{
	clock_t start, finish; //정렬 알고리즘의 수행시간을 측정하기 위한 변수들

	srand(1234); //시드값 설정

	for (int i = 1000; i <= 20000; i += 1000) //데이터의 개수를 1000부터 20000까지 1000단위로 반복한다.
	{
		num = i; //배열의 데이터 개수 저장
		float* floatArray = (float*)malloc(sizeof(float) * i); //배열을 동적할당한다.
		for (int j = 0; j < i; j++)
		{
			floatArray[j] = (rand() / (float)RAND_MAX * 2.0f) - 1; //-1~1사이의 실수 난수 배열을 생성한다.
		}

		start = clock(); //정렬알고리즘 수행 전 시간을 체크한다.
		mergeSort(floatArray, 0, i - 1); //정렬알고리즘을 수행한다.
		finish = clock(); //정렬알고리즘 수행 후 시간을 체크한다.

		printf("%d\n", (finish - start)); //정렬알고리즘의 수행시간을 출력한다.

		free(floatArray); //동적할당된 메모리 해제
	}
	return 0;
}
void mergeSort(float A[], int p, int r)
{
	int mid;
	if (p < r)
	{
		mid = (p + r) / 2; //중간지점(mid)을 구한다.
		mergeSort(A, p, mid); //p부터 중간지점(mid)까지를 정렬한다.
		mergeSort(A, mid + 1, r); //중간지점(mid) 이후부터 r까지를 정렬한다.
		merge(A, p, mid, r); //정렬된 두 개를 합친다.
	}
}
void merge(float A[], int p, int q, int r) {
	int i = p; //배열의 index는 p부터 r까지 있을 것이고 i는 p부터 시작한다. 
	int j = q + 1; //j는 배열의 중간지점(q)의 다음인 q+1부터 시작한다.
	int newSave = p; //새롭게 저장될 위치를 나타내는 변수이다. p부터 r까지 저장될 위치가 변할 것이므로 먼저 p로 초기화한다. 
	float* sortArray = (float*)malloc(sizeof(float) * num); //정렬된 데이터를 저장할 배열을 동적할당 해준다.
	while (i <= q && j <= r) { //i가 배열의 중간지점인 q를 넘지않고 j는 배열의 마지막지점인 r을 넘기 않는다면(이때, i부터 q까지는 정렬되어있으며 j부터 r까지도 정렬되어있을 것이다.)
		if (A[i] <= A[j]) //각 위치의 데이터를 비교하여  i번째의 데이터가 더 작거나 같다면 더 작은 데이터를 새로운배열에 저장한다. 그러면 i번째 데이터는 정렬이 완료되었으므로 다음 데이터로 i를 옮긴다.
		{
			sortArray[newSave] = A[i++];
		}
		else //j번째 데이터가 더 작다면 더작은 데이터를 새로운 배열에 저장한다. 그러면 j번째 데이터는 정렬이 완료되었으므로 다음 데이터로 j를 옮긴다.
		{
			sortArray[newSave] = A[j++];
		}
		newSave++; //새롭게 저장될 위치를 변경한다
	}
	//위의 while문이 끝난 것은 i가 p부터 q까지의 데이터를 모두 처리했거나 j가 q+1부터 r까지의 데이터를 모두 처리한 경우이다.
	while (i <= q) //만약 i가 q까지의 데이터를 모두 처리하지 못한경우 남은 데이터를 모두 새로운 배열에 저장한다.
	{
		sortArray[newSave++] = A[i++];
	}
	while (j <= r)//만약 j가 r까지의 데이터를 모두 처리하지 못한경우 남은 데이터를 모두 새로운 배열에 저장한다.
	{
		sortArray[newSave++] = A[j++];
	}
	for (int i = p; i <= r; i++) //정렬된 배열을 원래 배열에 복사해준다.
	{
		A[i] = sortArray[i];
	}
	free(sortArray); //동적할당된 배열을 해제한다.
}
