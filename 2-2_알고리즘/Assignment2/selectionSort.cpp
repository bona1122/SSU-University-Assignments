#include<stdio.h>
#include<stdlib.h>
#include<time.h>
void selectionSort(float A[], int n);
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
		selectionSort(floatArray, i); //���ľ˰����� �����Ѵ�.
		finish = clock(); //���ľ˰��� ���� �� �ð��� üũ�Ѵ�.

		printf("%d\n", (finish - start)); //���ľ˰����� ����ð��� ����Ѵ�.

		free(floatArray); //�����Ҵ�� �޸� ����
	}
	return 0;
}
void selectionSort(float A[], int n)
{
	float temp; //��ȯ�Ҷ� ����� ����

	for (int i = n - 1; i > 0; i--) //���� ū ���� ã�� ���������Һ��� �ι�° ���ұ��� �����Ұ��̹Ƿ� n-1�� �ݺ��Ѵ�.
	{
		for (int j = i - 1; j >= 0; j--) //������ ������ �պ��� index = 0 ���� �� ���ϸ鼭 ���� ū���� ã�´�.
		{
			if (A[j] > A[i]) //���ĵ� i��°�� ���� ���Ͽ� j��°�� ���� �� ũ�� i��°�� ��ȯ�Ѵ�.
			{
				temp = A[i];
				A[i] = A[j];
				A[j] = temp;
			}
		}
	}
}