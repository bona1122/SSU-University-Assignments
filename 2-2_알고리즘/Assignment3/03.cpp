#include<stdio.h>
void LinearProbing(int hashTable[], int num); //�������縦 ���� �ؽ����̺� �����͸� �����ϴ� �Լ�
int main()
{
	int array[8] = { 10,20,30,40,33,46,50,60 }; //���̺� ������ ������ �迭 ����

	int hashTable[13] = { 0 }; //�ؽ����̺� �ʱ�ȭ

	for (int i = 0; i < 8; i++) //array�� �ִ� ���� ���ʷ� hasbTable�� ���� 
	{
		printf("<%d ���� ����>\n", array[i]);
		LinearProbing(hashTable, array[i]);

		for (int j = 0; j < 13; j++)
		{
			printf("%d\n", hashTable[j]); //hashTable ������ ���
		}
		printf("\n\n");
	}

	return 0;
}
void LinearProbing(int hashTable[],int num)
{
	int first = num % 13; //ó�� ����� �� ��ġ ����
	
	for (int k = first;  ; )
	{
		if (hashTable[k] == 0) // ���̺� 0�� ����Ǿ��ִٸ�(����ִٸ�)
		{
			hashTable[k] = num; //�ش� ��ġ�� num�� ����
			return; //������
		}
		else
		{
			k = (k + 1) % 13; //�ؽ����̺� �̹� ����Ǿ��ִٸ� ������ ����-��������
			if (k == first) //���� ���� ���� ������ ���� ���ٸ�
			{
				printf("���̺��� ���� á���ϴ�.");
				return; //������
			}
		}
	}
}