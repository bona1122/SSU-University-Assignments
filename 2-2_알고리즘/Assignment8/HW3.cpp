#include <stdio.h>
#include <stdlib.h>
#include <time.h>
bool promising(int i, int weight, int total); //노드가 유망한지	분별하는 함수
int estimate_sum_of_subsets(int n);	//몬테카를로 기법을 통해 백트레킹알고리즘의 전형적 경로에서의 노드의 수를 세는 함수
int W = 52; //목표무게이다.
int w[7] = { 0,2,10,13,17,22,42 }; //각무게를 배열에 저장한다.(인덱스 0번은 사용안할것이므로 그냥 0으로 채워준다.)
int TOTAL_WEIGHT= 106;	//각 무게를 모두 더한 값이다.
int main()
{
	srand((unsigned int)time(NULL));
	int estimate = 0;
	int sum = 0;
	for (int i = 0; i < 30; i++)	//추정치를 구하기 위해 30번 반복하여 평균을 구할 것이다.
	{
		estimate = estimate_sum_of_subsets(6);	//상태공간트리의 전형적인 경로를 무작위로 생성하여 그 경로상의 노드의 수를 센 것
		sum = sum + estimate;	//나온값을 더한다.
		printf("%d : %d\n",i+1, estimate);		
	}
	printf("\n평균 : %d\n", sum / 30);	//추정치를 출력한다.
	return 0;
}
int estimate_sum_of_subsets(int n)
{
	int m = 1;	//유망한 자식노드의 개수를 나타내는 변수
	int mprod = 1;	//각 레벨에서 유망한 자식들
	int numnodes = 1;	//몬테카를로 기법을 통해 센 노드의 개수
	int t;	//노드(v)의 자식의 개수
	int i = 0;	//루트노드부터 시작
	int weight = 0; //현재까지의 무게
	int total = 0;	//남은 것들을 합친 무게
	int include[7] = { 0, 0, 0, 0, 0, 0, 0 }; //각 무게가 포함이 되었는지 안되었는지 확인하는 배열. 0이면 미포함. 1이면 포함

	//srand((unsigned int)time(NULL));

	while (m != 0 && i<=n)	//유망한 자식노드가 없거나 리프노드에 도달한 경우.
	{
		t = 2;	//v의 자식의 개수는 무조건 2개일것. 특정 무게를 포함하거나 안포함하거나.
		mprod = mprod * m;
		numnodes = numnodes + mprod * t;
		
		i++; //아래로 내려간다.
		m = 0;	//v의 자식 중 유망한 자식의 수를 저장할 것이다.
		weight = 0;
		total = 0;
		for (int j = 1; j < 7; j++)	//현재까지의 total과 weight를 알아본다.
		{
			if (include[j] == 1)
			{
				weight = weight + w[j];
			}
		}
		total = TOTAL_WEIGHT - weight;

		int c1 = 0;		//2개의 자식에 대해 살펴볼 것이다.
		int c2 = 0;

		//자식노드 중 w[i]가 포함 되는경우가 유망한지 살펴보자.
		if (promising(i, weight + w[i], total-w[i]))	//자식노드 중 w[i]가 포함 되는경우가 유망한지 살펴보자.
		{
			c1 = 1; //자식인 c1이 유망하다고 체크.
			m++; //유망한 자식노드 개수 1 늘림.
		}
	
		//자식노드 중 w[i]가 포함이 안되는 경우가 유망한지 살펴보자.
		if (promising(i, weight, total - w[i]))
		{
			c2 = 1;	//자식인 c2가 유망하다고 체크.
			m++; //유망한 자식노드 개수 1 늘림.
		}	
		if (m != 0)		//유망한 자식이 있다면 랜덤하게 고른다.
		{
			if (m == 1) //유망한 자식이 하나면 그 하나를 선택한다.
			{
				if (c1 == 1)	//자식 중 c1이 유망했다면
				{
					include[i] = 1;	
					weight = weight + w[i];
					total = total - w[i];
				}
				else	//자식 중 c2가 유망했다면
				{
					total = total - w[i];
				}
			}
			else //유망한 자식이 두개라면 두개 중에 하나 골라야한다.
			{
				int randChild = rand() % 2;
				if (randChild == 0) //c1으로 가는 걸 선택
				{
					include[i] = 1;
					weight = weight + w[i];
					total = total - w[i];
				}
				else //c2로 가는 걸 선택
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