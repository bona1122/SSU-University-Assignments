#include <stdio.h>
void sum_of_subsets(int i, int weight, int total);	//��ǥ ���Ը� �����ϴ� ���� ����ϴ� �Լ�
bool promising(int i, int weight, int total);	//���� ��尡 �������� �к��ϴ� �Լ�
int W = 52; //�� ���� �����ؾ���. ��ǥ �����̴�.
int w[7] = { 0,2,10,13,17,22,42 }; //�ε��� 0���� �����Ұ�. �׳� 0���� ä���ش�. 
int include[7] = {0}; //�� ���԰� ������ �Ǿ����� �ȵǾ����� Ȯ���ϴ� �迭. 0�̸� ������. 1�̸� ����
int main()
{
	sum_of_subsets(0,0,106);
	return 0;
}
void sum_of_subsets(int i, int weight, int total)
{
	if (promising(i,weight,total))	//���� ��尡 �����ϴٸ�
	{
		if (weight == W)	//���̸� ���
		{
			for (int i = 1; i < 7; i++)
			{
				if(include[i] == 1) //���ԵȰ͸� ���
					printf("%d + ",w[i]);
			}
			printf("\b\b= 52\n");
		}
		else   //���� �ƴϸ�
		{
			include[i + 1] = 1; //���� �� ���Խ�Ű�� ����.
			sum_of_subsets(i + 1, weight + w[i + 1], total - w[i + 1]);
			include[i + 1] = 0; //������ ���� �Ƚ�Ű�� ����.
			sum_of_subsets(i + 1, weight, total - w[i + 1]);
		}
	}
}
bool promising(int i, int weight, int total)
{
	return (weight + total >= W) && (weight == W || weight + w[i + 1] <= W); /*���� ���Կ��ٰ� ���� ��� ���Ը� �� ���ص� W���� ��������
																				���� ���Կ� ���� ���Ը� ���ϸ� ��ǥ���Ժ��� ��ġ�� ��츦 �����Ѵ�.*/
}
