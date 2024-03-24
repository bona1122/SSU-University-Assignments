#include<stdio.h>
#include<stdlib.h>
#include<time.h>
void mergeSort(float A[], int p, int r);
void merge(float A[], int p, int q, int r);
int num = 0; //merge�Լ����� �����Ҵ� �� �迭�� ������ ������ �˾ƾ� �ϹǷ� ���������� �������ش�.
int main()
{
	clock_t start, finish; //���� �˰����� ����ð��� �����ϱ� ���� ������

	srand(1234); //�õ尪 ����

	for (int i = 1000; i <= 20000; i += 1000) //�������� ������ 1000���� 20000���� 1000������ �ݺ��Ѵ�.
	{
		num = i; //�迭�� ������ ���� ����
		float* floatArray = (float*)malloc(sizeof(float) * i); //�迭�� �����Ҵ��Ѵ�.
		for (int j = 0; j < i; j++)
		{
			floatArray[j] = (rand() / (float)RAND_MAX * 2.0f) - 1; //-1~1������ �Ǽ� ���� �迭�� �����Ѵ�.
		}

		start = clock(); //���ľ˰��� ���� �� �ð��� üũ�Ѵ�.
		mergeSort(floatArray, 0, i - 1); //���ľ˰����� �����Ѵ�.
		finish = clock(); //���ľ˰��� ���� �� �ð��� üũ�Ѵ�.

		printf("%d\n", (finish - start)); //���ľ˰����� ����ð��� ����Ѵ�.

		free(floatArray); //�����Ҵ�� �޸� ����
	}
	return 0;
}
void mergeSort(float A[], int p, int r)
{
	int mid;
	if (p < r)
	{
		mid = (p + r) / 2; //�߰�����(mid)�� ���Ѵ�.
		mergeSort(A, p, mid); //p���� �߰�����(mid)������ �����Ѵ�.
		mergeSort(A, mid + 1, r); //�߰�����(mid) ���ĺ��� r������ �����Ѵ�.
		merge(A, p, mid, r); //���ĵ� �� ���� ��ģ��.
	}
}
void merge(float A[], int p, int q, int r) {
	int i = p; //�迭�� index�� p���� r���� ���� ���̰� i�� p���� �����Ѵ�. 
	int j = q + 1; //j�� �迭�� �߰�����(q)�� ������ q+1���� �����Ѵ�.
	int newSave = p; //���Ӱ� ����� ��ġ�� ��Ÿ���� �����̴�. p���� r���� ����� ��ġ�� ���� ���̹Ƿ� ���� p�� �ʱ�ȭ�Ѵ�. 
	float* sortArray = (float*)malloc(sizeof(float) * num); //���ĵ� �����͸� ������ �迭�� �����Ҵ� ���ش�.
	while (i <= q && j <= r) { //i�� �迭�� �߰������� q�� �����ʰ� j�� �迭�� ������������ r�� �ѱ� �ʴ´ٸ�(�̶�, i���� q������ ���ĵǾ������� j���� r������ ���ĵǾ����� ���̴�.)
		if (A[i] <= A[j]) //�� ��ġ�� �����͸� ���Ͽ�  i��°�� �����Ͱ� �� �۰ų� ���ٸ� �� ���� �����͸� ���ο�迭�� �����Ѵ�. �׷��� i��° �����ʹ� ������ �Ϸ�Ǿ����Ƿ� ���� �����ͷ� i�� �ű��.
		{
			sortArray[newSave] = A[i++];
		}
		else //j��° �����Ͱ� �� �۴ٸ� ������ �����͸� ���ο� �迭�� �����Ѵ�. �׷��� j��° �����ʹ� ������ �Ϸ�Ǿ����Ƿ� ���� �����ͷ� j�� �ű��.
		{
			sortArray[newSave] = A[j++];
		}
		newSave++; //���Ӱ� ����� ��ġ�� �����Ѵ�
	}
	//���� while���� ���� ���� i�� p���� q������ �����͸� ��� ó���߰ų� j�� q+1���� r������ �����͸� ��� ó���� ����̴�.
	while (i <= q) //���� i�� q������ �����͸� ��� ó������ ���Ѱ�� ���� �����͸� ��� ���ο� �迭�� �����Ѵ�.
	{
		sortArray[newSave++] = A[i++];
	}
	while (j <= r)//���� j�� r������ �����͸� ��� ó������ ���Ѱ�� ���� �����͸� ��� ���ο� �迭�� �����Ѵ�.
	{
		sortArray[newSave++] = A[j++];
	}
	for (int i = p; i <= r; i++) //���ĵ� �迭�� ���� �迭�� �������ش�.
	{
		A[i] = sortArray[i];
	}
	free(sortArray); //�����Ҵ�� �迭�� �����Ѵ�.
}
