#include<stdio.h>
int INF = 9999;	//정점이 이어져있지 않은경우 최대값으로 표기하기 위한 변수
void prim(int W[][8]);	//프림알고리즘으로 최소비용신장트리를 출력하는 함수
int main()
{
	int W[8][8] = {					//가중치 배열 초기화
		{0,11,9,8,INF,INF,INF,INF},
		{11,0,3,INF,8,8,INF,INF},
		{9,3,0,15,INF,12,1,INF},
		{8,INF,15,0,INF,INF,10,INF},
		{INF,8,INF,INF,0,7,INF,4},
		{INF,8,12,INF,7,0,INF,5},
		{INF,INF,1,10,INF,INF,0,2},
		{INF,INF,INF,INF,4,5,2,0} };

	prim(W);	//최소비용신장트리 호출
	return 0;
}
void prim(int W[][8])
{
	int nearest[8];	//Y에 속한 정점 중에서 vi에서 가장 가까운 정점의 인덱스를 저장하기 위한 배열
	int distance[8];	//vi와 nearest[i]를 잇는 가중치를 저장하기 위한 배열
	int vnear = 0;	//Y에 새로 추가할 정점을 저장할 변수
	int sum = 0;	//최소비용신장트리의 총 비용을 저장할 변수

	for (int i = 1; i < 8; i++)
	{
		nearest[i] = 1;	//vi에서 가장 가까운 정점을 v1로 초기화
		distance[i] = W[0][i];	//vi와 v1을 잇는 가중치로 초기화
	}
	printf("%d 정점 포함\n", vnear + 1);	//v1정점은 Y에 포함되었으므로 출력한다.

	for (int j = 1; j < 8; j++) //나머지 n-1개의 정점을 Y에 추가한다
	{
		int min = INF;
		for (int i = 1; i < 8; i++)	//각 정점에 대해서
		{
			if (0 <= distance[i] && distance[i] <= min) //distance[i]를 검사하여 가장 가까이 있는 vnear를 찾는다.
			{
				min = distance[i];
				vnear = i;
			}
		}
		sum = sum + distance[vnear];	//가장 가까이에 있는 정점을 추가하면서 비용을 추가한다.
		printf("%d 정점 포함, 비용 : %d\n", vnear+1, distance[vnear]);	//추가된 정점과 간선의 가중치를 출력한다.
		distance[vnear] = -1;	//찾은 정점은 Y에 추가한다.
		for (int i = 1; i < 8; i++)
		{
			if (W[i][vnear] < distance[i])	//Y에 없는 각 정점에 대해서 distance[i]를 갱신한다.
			{
				distance[i] = W[i][vnear];
				nearest[i] = vnear;
			}
		}
	}
	printf("\n최소신장트리 비용: %d\n", sum);	//최소비용신장트리의 총 비용을 출력한다.
}