#include<stdio.h>
void QuadraticProbing(int hashTable[], int num); //���������縦 ���� �ؽ����̺� �����͸� �����ϴ� �Լ�
int main()
{
	int array[8] = { 10,20,30,40,33,46,50,60 }; //���̺� ������ ������ �迭 ����

	int hashTable[13] = { 0 }; //�ؽ����̺� �ʱ�ȭ

	for (int i = 0; i < 8; i++) //array�� �ִ� ���� ���ʷ� hasbTable�� ���� 
	{
		printf("<%d ���� ����>\n", array[i]);
		QuadraticProbing(hashTable, array[i]);

		for (int j = 0; j < 13; j++)
		{
			printf("%d\n", hashTable[j]); //hashTable ������ ���
		}
		printf("\n\n");
	}
	return 0;
}
void QuadraticProbing(int hashTable[],int num)
{
	for (int j = 0; ; j++)
	{
		if (j == 13) //j�� 13�̶�� ������ �� 12���õ��� ����̹Ƿ� ���̺��� ���� á�� ���̴�.
		{
			printf("���̺��� ���� á���ϴ�.");
		}
		int store = (num + j * j) % 13; //������ ���縦 ���� ������ �ε����� ���Ѵ�. 
		if (hashTable[store] == 0) // ���̺��� ����ִٸ�
		{
			hashTable[store] = num; //�ش� ��ġ�� num�� ����
			return; //������
		}

	}
}