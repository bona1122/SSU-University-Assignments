#include<stdio.h>
int INF = 9999;	//������ �̾������� ������� �ִ밪���� ǥ���ϱ� ���� ����
void prim(int W[][8]);	//�����˰������� �ּҺ�����Ʈ���� ����ϴ� �Լ�
int main()
{
	int W[8][8] = {					//����ġ �迭 �ʱ�ȭ
		{0,11,9,8,INF,INF,INF,INF},
		{11,0,3,INF,8,8,INF,INF},
		{9,3,0,15,INF,12,1,INF},
		{8,INF,15,0,INF,INF,10,INF},
		{INF,8,INF,INF,0,7,INF,4},
		{INF,8,12,INF,7,0,INF,5},
		{INF,INF,1,10,INF,INF,0,2},
		{INF,INF,INF,INF,4,5,2,0} };

	prim(W);	//�ּҺ�����Ʈ�� ȣ��
	return 0;
}
void prim(int W[][8])
{
	int nearest[8];	//Y�� ���� ���� �߿��� vi���� ���� ����� ������ �ε����� �����ϱ� ���� �迭
	int distance[8];	//vi�� nearest[i]�� �մ� ����ġ�� �����ϱ� ���� �迭
	int vnear = 0;	//Y�� ���� �߰��� ������ ������ ����
	int sum = 0;	//�ּҺ�����Ʈ���� �� ����� ������ ����

	for (int i = 1; i < 8; i++)
	{
		nearest[i] = 1;	//vi���� ���� ����� ������ v1�� �ʱ�ȭ
		distance[i] = W[0][i];	//vi�� v1�� �մ� ����ġ�� �ʱ�ȭ
	}
	printf("%d ���� ����\n", vnear + 1);	//v1������ Y�� ���ԵǾ����Ƿ� ����Ѵ�.

	for (int j = 1; j < 8; j++) //������ n-1���� ������ Y�� �߰��Ѵ�
	{
		int min = INF;
		for (int i = 1; i < 8; i++)	//�� ������ ���ؼ�
		{
			if (0 <= distance[i] && distance[i] <= min) //distance[i]�� �˻��Ͽ� ���� ������ �ִ� vnear�� ã�´�.
			{
				min = distance[i];
				vnear = i;
			}
		}
		sum = sum + distance[vnear];	//���� �����̿� �ִ� ������ �߰��ϸ鼭 ����� �߰��Ѵ�.
		printf("%d ���� ����, ��� : %d\n", vnear+1, distance[vnear]);	//�߰��� ������ ������ ����ġ�� ����Ѵ�.
		distance[vnear] = -1;	//ã�� ������ Y�� �߰��Ѵ�.
		for (int i = 1; i < 8; i++)
		{
			if (W[i][vnear] < distance[i])	//Y�� ���� �� ������ ���ؼ� distance[i]�� �����Ѵ�.
			{
				distance[i] = W[i][vnear];
				nearest[i] = vnear;
			}
		}
	}
	printf("\n�ּҽ���Ʈ�� ���: %d\n", sum);	//�ּҺ�����Ʈ���� �� ����� ����Ѵ�.
}