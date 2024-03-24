#include <stdio.h>
int p[6] = { 0,20,30,35,12,3 };	//�� �������� ��ġ�� ������ �迭
int w[6] = { 0,2,5,7,3,1 };	//�� �������� ���Ը� ������ �迭
int include[6] = { 0 };	//�� �������� ���ԵǾ����� Ȯ���ϴ� �迭.
int W = 9;	//�ִ밡���� ����
int maxprofit = 0;	//���� �� �ִ� �ִ��� ��ġ
void knapsack(int i, int profit, int weight);	//���̿켱Ž������ knapsack������ �ذ��ϰ� �ִ� ������ ����ϴ� �Լ�
bool promising(int i, int profit, int weight);	//��尡 �������� �к��ϴ� �Լ�
int main()
{
	knapsack(0, 0, 0);	
	return 0;
}
void knapsack(int i, int profit, int weight)
{
	if (weight <= W && profit > maxprofit)	//�ִ빫�Ը� ���� �ʰ� ��������� ������ maxprofit���� ū���
	{
		maxprofit = profit;	//maxprofit���� ����
		for (int k = 1; k <= i; k++) {	
			if (include[k] != 0)
			{
				printf("%d�� ������ ����\n", k);  //������ ���ÿ��θ� ���
			}
		}
		printf("maxprofit : %d\n\n", maxprofit); //maxprofit�� ���
	}
	if (promising(i,profit,weight)) {	//���� ��尡 �����ϴٸ�
		include[i + 1] = 1;	//���� �������� ���� �� 
		knapsack(i + 1, profit + p[i + 1], weight + w[i + 1]);	//��͸� ���� ���̿켱Ž��
		include[i + 1] = 0;	//���� �������� �������� ���� �� 
		knapsack(i + 1, profit, weight);	//��͸� ���� ���̿켱Ž��
	}
}
bool promising(int i, int profit,int weight)
{
	int totweight;	//��������� ���Կ� W�� �ʰ����� �ʰԲ� ���������۵��� ������ ����
	float bound;	//������ ��ġ�� ������ ���� �� �ִ� �ִ� ��ġ���� ������ ��
	int j;	
	int k;

	if (weight >= W) return 0;	//��ǥ���Ը� �ʰ��ϴ� ��� false�� ����	
	else {
		j = i + 1;
		bound = profit;	//��������� ������ ����
		totweight = weight;	//��������� ���Ը� ����
		while ((j <= 5) && (totweight + w[j] <= W ) ) {	//totweight�� W�� �ʰ��ϱ� ������ ���������۵��� ���Ѵ�.
			totweight = totweight + w[j];
			bound = bound + p[j];
			j++;
		}
		k = j;	//totweight�� W�� �ʰ��ϰ� �Ǵ� �������� ����
		if (k <= 5)
		{
			bound = bound + (W - totweight) * p[k] / w[k];	//���� ������ ����ϴ¹��Ը�ŭ ������ �������� �Ϻκ��� ���ϰ� bound�� ���Ѵ�.
		}
		return bound > maxprofit;	//������ ���� �� �ִ� �ִ밡ġ���� ������ ���� maxprofit���� ū ��� true����
	}
}