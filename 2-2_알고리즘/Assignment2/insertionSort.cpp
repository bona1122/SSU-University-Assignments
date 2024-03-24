#include<stdio.h>
#include<stdlib.h>
#include<time.h>
void insertionSort(float A[], int n);
int main()
{
	clock_t start, finish; //���� �˰����� ����ð��� �����ϱ� ���� ������

	srand(1234); //�õ尪 ����

	for (int i = 1000; i <= 20000; i += 1000) //�������� ������ 1000���� 20000���� 1000������ �ݺ��Ѵ�.
	{
		float* floatArray = (float*)malloc(sizeof(float) * i); //�迭�� �����Ҵ��Ѵ�.
		for (int j = 0; j < i; j++)
		{
			floatArray[j] = (rand() / (float)RAND_MAX * 2.0f) - 1; //-1~1������ �Ǽ� ���� �迭�� �����Ѵ�.
		}

		start = clock(); //���ľ˰��� ���� �� �ð��� üũ�Ѵ�.
		insertionSort(floatArray, i); //���ľ˰����� �����Ѵ�.
		finish = clock(); //���ľ˰��� ���� �� �ð��� üũ�Ѵ�.

		printf("%d\n", (finish - start)); //���ľ˰����� ����ð��� ����Ѵ�.

		free(floatArray); //�����Ҵ�� �޸� ����
	}
	return 0;
}
void insertionSort(float A[], int n)
{
	float temp; //��ȯ�Ҷ� ����� ����
	for (int i = 1; i < n; i++) //�ι�° ���Һ��� ������ ���ұ��� ���ʷ� �����ϸ� ������ ���̹Ƿ�  n-1�� �ݺ��ȴ�. (���ĵ� ������ ������ ���Ѵ�.)
	{
		int j;
		temp = A[i]; //���ĵ� i��° ���Ҹ� �����صд�.
		for (j = i - 1; j >= 0; j--)  //0������ i-1��° ���ұ����� ���ĵ� �����̹Ƿ� i-1��° ���Һ��� ���Ӱ� ���ĵ� ���� ���Ѵ�.
		{
			if (temp < A[j]) //j��° ���Ұ� �� ũ�ٸ� ���ĵ� ���Ҵ� j���� ���ʿ� �־�� �ϹǷ� j��°���Ҹ� j+1��° ���ҷ� �Ű��ش�.
			{
				A[j + 1] = A[j];
			}
			else //j��° ���Ұ� �� �۴ٸ� ���ĵ� ���Ҵ� j��° ���� ������ j+1��°�� ����Ǹ� �ȴ�.
			{
				break;
			}
		}
		A[j + 1] = temp;
	}
}