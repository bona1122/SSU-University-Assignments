#include <stdio.h>
int p[6] = { 0,20,30,35,12,3 };	//각 아이템의 가치를 저장한 배열
int w[6] = { 0,2,5,7,3,1 };	//각 아이템의 무게를 저장한 배열
int include[6] = { 0 };	//각 아이템이 포함되었는지 확인하는 배열.
int W = 9;	//최대가능한 무게
int maxprofit = 0;	//얻을 수 있는 최대의 가치
void knapsack(int i, int profit, int weight);	//깊이우선탐색으로 knapsack문제를 해결하고 최대 이익을 출력하는 함수
bool promising(int i, int profit, int weight);	//노드가 유망한지 분별하는 함수
int main()
{
	knapsack(0, 0, 0);	
	return 0;
}
void knapsack(int i, int profit, int weight)
{
	if (weight <= W && profit > maxprofit)	//최대무게를 넘지 않고 현재까지의 이익이 maxprofit보다 큰경우
	{
		maxprofit = profit;	//maxprofit값을 갱신
		for (int k = 1; k <= i; k++) {	
			if (include[k] != 0)
			{
				printf("%d번 아이템 선택\n", k);  //아이템 선택여부를 출력
			}
		}
		printf("maxprofit : %d\n\n", maxprofit); //maxprofit을 출력
	}
	if (promising(i,profit,weight)) {	//현재 노드가 유망하다면
		include[i + 1] = 1;	//다음 아이템을 포함 후 
		knapsack(i + 1, profit + p[i + 1], weight + w[i + 1]);	//재귀를 통해 깊이우선탐색
		include[i + 1] = 0;	//다음 아이템을 포함하지 않은 후 
		knapsack(i + 1, profit, weight);	//재귀를 통해 깊이우선탐색
	}
}
bool promising(int i, int profit,int weight)
{
	int totweight;	//현재까지의 무게에 W를 초과하지 않게끔 다음아이템들을 저장한 무게
	float bound;	//현재의 가치에 앞으로 얻을 수 있는 최대 가치까지 포함한 값
	int j;	
	int k;

	if (weight >= W) return 0;	//목표무게를 초과하는 경우 false를 리턴	
	else {
		j = i + 1;
		bound = profit;	//현재까지의 이익을 저장
		totweight = weight;	//현재까지의 무게를 저장
		while ((j <= 5) && (totweight + w[j] <= W ) ) {	//totweight이 W를 초과하기 전까지 다음아이템들을 취한다.
			totweight = totweight + w[j];
			bound = bound + p[j];
			j++;
		}
		k = j;	//totweight이 W를 초과하게 되는 아이템을 저장
		if (k <= 5)
		{
			bound = bound + (W - totweight) * p[k] / w[k];	//남은 공간이 허용하는무게만큼 마지막 아이템의 일부분을 취하고 bound에 더한다.
		}
		return bound > maxprofit;	//앞으로 얻을 수 있는 최대가치까지 포함한 값이 maxprofit보다 큰 경우 true리턴
	}
}