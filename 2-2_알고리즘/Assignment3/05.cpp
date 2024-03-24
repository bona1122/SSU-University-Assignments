#include<stdio.h>
#include<stdlib.h>
typedef struct Table { //�ؽ����̺��� �̷�� ��� ����
	int key;
	struct Table* p;
}Table;
void ChainedHashInsert(Table hashTable[], int num);
int main()
{
	int array[8] = { 10,20,30,40,33,46,50,60 }; //���̺� ������ ������ �迭 ����

	Table hashTable[13]; //�ؽ����̺� �غ�
	for (int i = 0; i < 13; i++) //�ؽ����̺� �ʱ�ȭ
	{
		hashTable[i].key = NULL;
		hashTable[i].p = NULL;
	}

	for (int i = 0; i < 8; i++) //array�� �ִ� ���� ���ʷ� hasbTable�� ���� 
	{
		printf("<%d ���� ����>\n", array[i]);
		ChainedHashInsert(hashTable, array[i]); //ü�̴��� �̿��Ͽ� ������ ����

		for (int j = 0; j < 13; j++)  //���� �� ��� ���
		{
			if (hashTable[j].p == NULL) //�ؽ����̺� j�� �ε����� �ƹ��͵� ����Ű�� �ʴ´ٸ�(j���ε����� �����Ͱ� ���ٸ�)
			{
				printf("����");
			}
			else //j���ε����� �����Ͱ� �ִٸ�
			{
				Table* start = hashTable[j].p; //ó�� ����� ��带 ����Ų��.
				while (true)
				{
					printf("%d ", start->key); //������ ���
					if (start->p != NULL) 
					{ start = start->p; } //��µ� ��尡 ����Ű�� ���� ��尡 �ִٸ� start�� �������� �ٲ��ش�.
					else //��µ� ��尡 ����Ű�� ���� ��尡 ���ٸ� �׸��Ѵ�.
					{
						break;
					}
				}
			}
			printf("\n");
		}
		printf("\n\n");
	}
	return 0;
}
void ChainedHashInsert(Table hashTable[], int num) 
{
	int store = num % 13; //�ؽ����̺� ����� ��ġ�� ���Ѵ�.
	if (hashTable[store].p == NULL) //����� ���� �����Ͱ� ���ٸ�(�ش��ε����� ����Ű�� ��尡 ���ٸ�)
	{
		hashTable[store].p = (Table*)calloc(1, sizeof(Table)); //�� ��带 �߰����ش�.
		hashTable[store].p->key = num; //����忡 num���� �����Ѵ�.
	}
	else //����� ���� �����Ͱ� �̹� �ִٸ�(�ش��ε����� ����Ű�� ��尡 �ִٸ�)
	{
		Table* newNode = (Table*)calloc(1, sizeof(Table)); //�� ��� �غ�
		newNode->key = num; //�� ��忡 num�� ����.
		newNode->p = hashTable[store].p; //����尡 ����� ���� ����Ű�� ��带 ����Ű�� �Ѵ�.(�� ��带 ���� �տ� �α� ����)
		hashTable[store].p = newNode; //����� ���� ����带 ����Ű�� �Ѵ�.(�� ��尡 ���� �տ� ���� ��)
	}
}