#include<stdio.h>
#include<stdlib.h>
#include<time.h>
void bubbleSort(float A[], int n);
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
		bubbleSort(floatArray, i); //���ľ˰����� �����Ѵ�.
		finish = clock(); //���ľ˰��� ���� �� �ð��� üũ�Ѵ�.

		printf("%d\n", (finish - start)); //���ľ˰����� ����ð��� ����Ѵ�.

		free(floatArray); //�����Ҵ�� �޸� ����
	}
	return 0;
}
void bubbleSort(float A[], int n)
{
	float temp; //�迭 ���� ���� ��ȯ�� �� ����� ����

	for (int i = n - 1; i >= 1; i--) //�迭�� ������ ���Һ��� 1�� ���ұ��� ������ ���̴�.(���ĵ� ���� ������ ���ϰ� �ȴ�.)
	{
		for (int j = 0; j <= i - 1; j++) //�迭�� 0�����Һ��� ������ �Ϸ�� i������ ������ ������ ���� ���Ѵ�.
		{
			if (A[j] > A[j + 1]) //������ ���ҿ� ���Ͽ� �� ū ���� ���������� �ű��.
			{
				temp = A[j + 1];
				A[j + 1] = A[j];
				A[j] = temp;
			}
		}
	}
}