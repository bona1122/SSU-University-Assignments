#include <stdio.h>
int tsp(int mask, int current, int W[][5]); //current�������� �����Ͽ� �ٸ� ��� ������ ���� ���ƿ��� �ּҺ���� �Լ�
void printPath(int start, int mask); //start�������� �����Ͽ� �ٸ� ��� ������ ���� ���ƿ��� ��θ� ����ϴ� �Լ�
int visit_all = (1 << 5) - 1; // �� �湮���� ���� ��Ÿ���� ���� ���� �� �ʱ�ȭ(00011111 --> 31 . �湮�� ������ 1�� ǥ��)
int path[5][32]; //��θ� �����ϱ����� �迭
int main()
{
	int W[5][5] = { {0,8,13,18,20},{3,0,7,8,10},{4,11,0,10,7},{6,6,7,0,11},{10,6,2,1,0} }; //����ġ ��� �ʱ�ȭ
	int cost;
	for (int i = 0; i < 5; i++) //path�迭 -1�� �ʱ�ȭ
	{
		for (int j = 0; j < 32; j++)
		{
			path[i][j] = -1;
		}
	}
	cost = tsp(1, 0, W); //����ġ ����� �̿��Ͽ� ����0���� ��������� ��ġ�� ���ƿ��� �ּҺ���� ���Ѵ�.
	printf("�ּ� ��� : %d\n", cost); //�ּҺ���� ����Ѵ�.
	printPath(0,1);	//����0���� ��� ������ ��ġ�� ���ƿ��� ��θ� ����Ѵ�.

	return 0;
}
int tsp(int mask, int current, int W[][5]) {
	if (mask == visit_all) { //��� �湮�� ���¶�� 
		return W[current][0]; //pos���� 0�� ���������� �����Ѵ�.
	}
	int cost = 999; //�ּҺ���� ���ϱ� ���� ���� ���� �� �ʱ�ȭ
	for (int city = 1; city < 5; city++) { //���� ��ġ �������� 1~4�������� ���� ���� ���ϱ� ���� �ݺ��Ѵ�.

		if ((mask & (1 << city)) == 0) { //city�������� �湮���� ���� ���
			//city�������� �湮�ϰ� city�� ���������� �ּҺ���� ���Ѵ�.
			int newCost = W[current][city] + tsp(mask | (1 << city), city, W); 
			if (newCost < cost) //���ο� ����� ������ ���� ���ؼ� �� �۴ٸ� ����� �� ���� ������ �����Ѵ�.
			{
				cost = newCost;
				path[current][mask] = city; //�ּҺ���� ������ city������ �����Ѵ�.
			}
		}
	}
	return cost;
}
void printPath(int start, int mask)
{
	if (start == -1) //�������� -1 �̶�� ��� ������ �湮�� �����ϰ��̴�. ���� 0�� ����ϰ� ������.
	{
		printf("%d\n", 0);
		return;
	}
	printf("%d --> ", start); //�������� ����Ѵ�.
	int next = path[start][mask]; //�� ���� ������ next�� �����Ѵ�.
	printPath(next, mask | (1<< next)); //�� ���� ������ �湮�ѻ��·� ���� ��θ� ����Ѵ�.
}