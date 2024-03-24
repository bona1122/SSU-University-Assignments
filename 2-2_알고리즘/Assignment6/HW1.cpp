#include<stdio.h>
int D[7][7] = { 0 }; //��� �� �ִܰ�θ� �����ϱ� ���� ���
void simpleShortestPath(int W[][7]); //����ġ����� �̿��Ͽ� ��� �� �ִܰ�θ� ���ϴ� �Լ�
int INF = 9999;	//������ �̾������� ������� �ִ밪���� ǥ���ϱ� ���� ����
int main()
{
	int W[7][7] = {			//����ġ ��� �ʱ�ȭ
		{0,4,INF,INF,INF,10,INF},
		{3,0,INF,18,INF,INF,INF},
		{INF,6,0,INF,INF,INF,INF},
		{INF,5,15,0,2,19,5},
		{INF,INF,12,1,0,INF,INF},
		{INF,INF,INF,INF,INF,0,10},
		{INF,INF,INF,8,INF,INF,0} };

	simpleShortestPath(W); //��� �� �ִܰ�� ���ϱ�

	for (int i = 0; i < 7; i++) //��� �� �ִܰ�θ� ���
	{
		for (int j = 0; j < 7; j++)
		{
			printf("v%d���� v%d������ �ִܰ��: %d\n", i + 1, j + 1, D[i][j]); 
		}
		printf("\n");
	}
	return 0;
}
void simpleShortestPath(int W[][7])
{
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			D[i][j] = W[i][j]; //1���� ������ ����Ͽ� ����i���� ����j�� �̸��� �ִܰŸ��� ����
		}
	}
	for (int m = 1; m < 6; m++) //�ִ� 2��~6���� ������ ����Ͽ� ����i���� ����j�� �̸��� �ִܰŸ��� ����
	{
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				int min = INF;	//�ִܰ�θ� ���� �� ����� ����
				for (int k = 0; k < 7; k++)	//���� k�� ���ļ� ���� �ִܰ�ε��� ���Ѵ�. 
				{					
					if (D[i][k] != INF && W[k][j] != INF) //i����k������ �ִܰ�ΰ� �����ϰ� k���� j���� ����Ǿ��ִٸ� 
					{
						int value = D[i][k] + W[k][j]; //i���� k������ �ִܰ�ο� k����j������ ��θ� ���Ѵ�.
						if (value < min)	//���ο� ���� �� ���� ��� min�� �����Ѵ�.
							min = value;
					}	
				}
				D[i][j] = min;	//i���� j������ �ִܰ�θ� min������ �����Ѵ�.
			}
		}
	}
}