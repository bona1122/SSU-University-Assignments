#include <stdio.h>
void sum_of_subsets(int i, int weight, int total);	//목표 무게를 만족하는 답을 출력하는 함수
bool promising(int i, int weight, int total);	//현재 노드가 유망한지 분별하는 함수
int W = 52; //이 값을 만족해야함. 목표 무게이다.
int w[7] = { 0,2,10,13,17,22,42 }; //인덱스 0번은 사용안할것. 그냥 0으로 채워준다. 
int include[7] = {0}; //각 무게가 포함이 되었는지 안되었는지 확인하는 배열. 0이면 미포함. 1이면 포함
int main()
{
	sum_of_subsets(0,0,106);
	return 0;
}
void sum_of_subsets(int i, int weight, int total)
{
	if (promising(i,weight,total))	//현재 노드가 유망하다면
	{
		if (weight == W)	//답이면 출력
		{
			for (int i = 1; i < 7; i++)
			{
				if(include[i] == 1) //포함된것만 출력
					printf("%d + ",w[i]);
			}
			printf("\b\b= 52\n");
		}
		else   //답이 아니면
		{
			include[i + 1] = 1; //다음 것 포함시키고 진행.
			sum_of_subsets(i + 1, weight + w[i + 1], total - w[i + 1]);
			include[i + 1] = 0; //다음것 포함 안시키고 진행.
			sum_of_subsets(i + 1, weight, total - w[i + 1]);
		}
	}
}
bool promising(int i, int weight, int total)
{
	return (weight + total >= W) && (weight == W || weight + w[i + 1] <= W); /*현재 무게에다가 남은 모든 무게를 다 더해도 W보다 작은경우와
																				현재 무게에 다음 무게를 더하면 목표무게보다 넘치는 경우를 제외한다.*/
}
