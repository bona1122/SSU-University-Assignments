#include<stdio.h>
#include<stdlib.h>
#include<time.h>
void heapSort(float A[], int n);
void buildHeap(float A[], int n);
void heapify(float A[], int root, int n);
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
		heapSort(floatArray, i); //정렬알고리즘을 수행한다.
		finish = clock(); //정렬알고리즘 수행 후 시간을 체크한다.

		printf("%d\n", (finish - start)); //정렬알고리즘의 수행시간을 출력한다.

		free(floatArray); //동적할당된 메모리 해제
	}
	return 0;
}
void heapSort(float A[], int n)
{
	float temp; //배열 내 데이터를 교환하기 위한 변수
	buildHeap(A, n); //배열에 저장된 데이터를 최대 힙으로 만든다.
	for (int i = n - 1; i > 0; i--) //최대힙에서 계속 루트노드를 빼면서 왼쪽부터 차례로 정렬할 것이고, 이 과정에서 루트노드 자리에 최대 힙의 마지막 원소를 채워넣을 것이다.
	{								//이때, 마지막원소는 계속해서 줄어들 것이므로 총 n-1번 반복된다. 
		temp = A[0]; //루트노드와 정렬된 원소를 제외한 마지막 원소를 바꿔준다.
		A[0] = A[i];
		A[i] = temp;

		heapify(A, 0, i - 1); //정렬된 원소 전까지 다시 최대 힙으로 만든다.
	}
}
void buildHeap(float A[], int n) //최대힙은 부모가 자식보다 크기만 하면 된다.
{
	for (int i = 0; i <= n; i++) //각 노드의 부모노드값이 각 노드의 값보다 큰지 확인하면 된다. 따라서 각 노드별로 확인하기 위해 n번 반복된다.
	{
		int current = i; //현재 노드
		while (true)
		{
			if (current == 0) break; //현재 노드가 루트노드라면 부모노드가 없을 것이므로 확인을 종료한다.

			int parent = (current - 1) / 2; //현재 노드의 부모노드를 가리키는 parent변수 초기화
			if (A[parent] < A[current]) //현재 노드가 부모노드보다 크다면 최대 힙을 만족시키기 위해 둘을 바꿔준다.
			{
				float temp = A[parent];
				A[parent] = A[current];
				A[current] = temp;

				current = parent; //바꾼 후에는 부모노드가 최대힙을 만족하는지 확인하기 위해 현재노드를 부모노드로 옮겨준다.
			}
			else //현재노드가 부모노드보다 크지 않다면 최대힙을 만족하므로 반복문을 종료한다.
			{
				break;
			}
		}
	}
}
void heapify(float A[], int root, int n) //루트노드의 왼쪽트리와 오른쪽 트리는 모두 최대힙을 만족하므로 루트노드를 최대힙구조에 맞게 이동시키는 함수이다.
{
	float e = A[root]; // 루트노드의 값을 저장해둘 변수 선언 및 초기화
	int j; //루트노드의 자식 노드를 가리킬 변수 선언
	for (j = 2 * root + 1; j <= n; j = j * 2 + 1) //j는 루트노드의 왼쪽 자식부터 시작해서 n까지 증가한다.
	{
		if (j < n && A[j] < A[j + 1]) //j가 트리내에 있고, 왼쪽 자식과 오른쪽 자식을 비교하여 오른쪽 자식이 더 큰 값이라면 
			j++; //j를 오른쪽 자식을 가리키도록 한다.
		if (e >= A[j]) //자식들 중 최대 자식(j)과 비교하여 e값이 더 크거나 같다면 최대힙 조정을 멈춘다.
			break;
		else //자식들 중 최대자식(j)과 비교하여 e값이 더 작다면
		{
			A[(j - 1) / 2] = A[j]; //j번째를 트리 위로 이동시킨다.(자식노드를 부모노드 위치로 이동)
		}
	}
	A[(j - 1) / 2] = e; //e를 j의 부모노드에 삽입하고 끝낸다.
}