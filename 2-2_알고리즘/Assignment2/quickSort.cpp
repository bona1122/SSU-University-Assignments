#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int partition(float A[], int left, int right);
void quickSort(float A[], int left, int right);
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
		quickSort(floatArray, 0, i - 1); //���ľ˰����� �����Ѵ�.
		finish = clock(); //���ľ˰��� ���� �� �ð��� üũ�Ѵ�.

		printf("%d\n", (finish - start)); //���ľ˰����� ����ð��� ����Ѵ�.

		free(floatArray); //�����Ҵ�� �޸� ����
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
	float pivot = A[right]; //�Ǻ��� ��ġ�� ���� ������ ���ҷ� �����Ѵ�.
	int i = left; //i�� ���� ���� ���ҷ� �����Ѵ�.
	int j = right - 1; //j�� �Ǻ��� ������ ���� ������ ���ҷ� �����Ѵ�.
	float temp;
	while (i <= j) //i�� j�� �������� ���� ������ �ݺ��Ѵ�.
	{
		while (A[i] < pivot) //i��° ���� �Ǻ����� ������ ���������͸� Ȯ���ϱ� ���� �Ѿ��. 
		{
			i++;
		}
		while (A[j] > pivot) //j��° ���� �Ǻ����� ũ�� ���������͸� Ȯ���ϱ� ���� �Ѿ��.
		{
			j--;
		}
		if (i <= j) //���� ���� �� , i�� j�� �������� �ʾҴٸ� i���� �����ʹ� �Ǻ����� ũ�� j��° �����ʹ� �Ǻ����� ���� ���̴�. ���� �� �����͸� ��ȯ���ָ� �迭�� 0��° ���Һ��� i��° ���ұ����� �Ǻ����� ���� �����͵��� ���� ���̰�, j��° ���Һ��� �迭�� ���������ұ����� �Ǻ����� ū �����͵��� ���� ���̴�.
		{

			temp = A[i];
			A[i] = A[j];
			A[j] = temp;
			i++;
			j--;
		}
	}
	//pivot�� i��°�� ���� ���� ��ȯ�Ͽ� �Ǻ��� �������� �Ǻ����� ū��, �Ǻ��� ������ �Ǻ����� ���������� ����� �ش�. 
	temp = A[i];
	A[i] = A[right];
	A[right] = temp;

	return i; //�Ű��� �Ǻ��� ��ġ�� ��ȯ�Ѵ�.
}