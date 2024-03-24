#include <stdio.h>
#include <stdlib.h>
#include <time.h>
bool promising(int i, int weight, int total); //��尡 ��������	�к��ϴ� �Լ�
int estimate_sum_of_subsets(int n);	//����ī���� ����� ���� ��Ʈ��ŷ�˰����� ������ ��ο����� ����� ���� ���� �Լ�
int W = 52; //��ǥ�����̴�.
int w[7] = { 0,2,10,13,17,22,42 }; //�����Ը� �迭�� �����Ѵ�.(�ε��� 0���� �����Ұ��̹Ƿ� �׳� 0���� ä���ش�.)
int TOTAL_WEIGHT= 106;	//�� ���Ը� ��� ���� ���̴�.
int main()
{
	srand((unsigned int)time(NULL));
	int estimate = 0;
	int sum = 0;
	for (int i = 0; i < 30; i++)	//����ġ�� ���ϱ� ���� 30�� �ݺ��Ͽ� ����� ���� ���̴�.
	{
		estimate = estimate_sum_of_subsets(6);	//���°���Ʈ���� �������� ��θ� �������� �����Ͽ� �� ��λ��� ����� ���� �� ��
		sum = sum + estimate;	//���°��� ���Ѵ�.
		printf("%d : %d\n",i+1, estimate);		
	}
	printf("\n��� : %d\n", sum / 30);	//����ġ�� ����Ѵ�.
	return 0;
}
int estimate_sum_of_subsets(int n)
{
	int m = 1;	//������ �ڽĳ���� ������ ��Ÿ���� ����
	int mprod = 1;	//�� �������� ������ �ڽĵ�
	int numnodes = 1;	//����ī���� ����� ���� �� ����� ����
	int t;	//���(v)�� �ڽ��� ����
	int i = 0;	//��Ʈ������ ����
	int weight = 0; //��������� ����
	int total = 0;	//���� �͵��� ��ģ ����
	int include[7] = { 0, 0, 0, 0, 0, 0, 0 }; //�� ���԰� ������ �Ǿ����� �ȵǾ����� Ȯ���ϴ� �迭. 0�̸� ������. 1�̸� ����

	//srand((unsigned int)time(NULL));

	while (m != 0 && i<=n)	//������ �ڽĳ�尡 ���ų� ������忡 ������ ���.
	{
		t = 2;	//v�� �ڽ��� ������ ������ 2���ϰ�. Ư�� ���Ը� �����ϰų� �������ϰų�.
		mprod = mprod * m;
		numnodes = numnodes + mprod * t;
		
		i++; //�Ʒ��� ��������.
		m = 0;	//v�� �ڽ� �� ������ �ڽ��� ���� ������ ���̴�.
		weight = 0;
		total = 0;
		for (int j = 1; j < 7; j++)	//��������� total�� weight�� �˾ƺ���.
		{
			if (include[j] == 1)
			{
				weight = weight + w[j];
			}
		}
		total = TOTAL_WEIGHT - weight;

		int c1 = 0;		//2���� �ڽĿ� ���� ���캼 ���̴�.
		int c2 = 0;

		//�ڽĳ�� �� w[i]�� ���� �Ǵ°�찡 �������� ���캸��.
		if (promising(i, weight + w[i], total-w[i]))	//�ڽĳ�� �� w[i]�� ���� �Ǵ°�찡 �������� ���캸��.
		{
			c1 = 1; //�ڽ��� c1�� �����ϴٰ� üũ.
			m++; //������ �ڽĳ�� ���� 1 �ø�.
		}
	
		//�ڽĳ�� �� w[i]�� ������ �ȵǴ� ��찡 �������� ���캸��.
		if (promising(i, weight, total - w[i]))
		{
			c2 = 1;	//�ڽ��� c2�� �����ϴٰ� üũ.
			m++; //������ �ڽĳ�� ���� 1 �ø�.
		}	
		if (m != 0)		//������ �ڽ��� �ִٸ� �����ϰ� ����.
		{
			if (m == 1) //������ �ڽ��� �ϳ��� �� �ϳ��� �����Ѵ�.
			{
				if (c1 == 1)	//�ڽ� �� c1�� �����ߴٸ�
				{
					include[i] = 1;	
					weight = weight + w[i];
					total = total - w[i];
				}
				else	//�ڽ� �� c2�� �����ߴٸ�
				{
					total = total - w[i];
				}
			}
			else //������ �ڽ��� �ΰ���� �ΰ� �߿� �ϳ� �����Ѵ�.
			{
				int randChild = rand() % 2;
				if (randChild == 0) //c1���� ���� �� ����
				{
					include[i] = 1;
					weight = weight + w[i];
					total = total - w[i];
				}
				else //c2�� ���� �� ����
				{
					total = total - w[i];
				}
			}
		}
	}
	return numnodes;
}
bool promising(int i, int weight, int total)
{
	return (weight + total >= W) && (weight == W || weight + w[i+1] <= W);
}