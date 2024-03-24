#include<stdio.h>
int D[7][7] = { 0 }; //모든 쌍 최단경로를 저장하기 위한 행렬
void simpleShortestPath(int W[][7]); //가중치행렬을 이용하여 모든 쌍 최단경로를 구하는 함수
int INF = 9999;	//정점이 이어져있지 않은경우 최대값으로 표기하기 위한 변수
int main()
{
	int W[7][7] = {			//가중치 행렬 초기화
		{0,4,INF,INF,INF,10,INF},
		{3,0,INF,18,INF,INF,INF},
		{INF,6,0,INF,INF,INF,INF},
		{INF,5,15,0,2,19,5},
		{INF,INF,12,1,0,INF,INF},
		{INF,INF,INF,INF,INF,0,10},
		{INF,INF,INF,8,INF,INF,0} };

	simpleShortestPath(W); //모든 쌍 최단경로 구하기

	for (int i = 0; i < 7; i++) //모든 쌍 최단경로를 출력
	{
		for (int j = 0; j < 7; j++)
		{
			printf("v%d부터 v%d까지의 최단경로: %d\n", i + 1, j + 1, D[i][j]); 
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
			D[i][j] = W[i][j]; //1개의 간선을 사용하여 정점i부터 정점j에 이르는 최단거리를 저장
		}
	}
	for (int m = 1; m < 6; m++) //최대 2개~6개의 간선을 사용하여 정점i부터 정점j에 이르는 최단거리를 저장
	{
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				int min = INF;	//최단경로를 비교할 때 사용할 변수
				for (int k = 0; k < 7; k++)	//정점 k를 거쳐서 가는 최단경로들을 비교한다. 
				{					
					if (D[i][k] != INF && W[k][j] != INF) //i부터k까지의 최단경로가 존재하고 k부터 j까지 연결되어있다면 
					{
						int value = D[i][k] + W[k][j]; //i부터 k까지의 최단경로에 k부터j까지의 경로를 합한다.
						if (value < min)	//새로운 값이 더 작을 경우 min을 갱신한다.
							min = value;
					}	
				}
				D[i][j] = min;	//i부터 j까지의 최단경로를 min값으로 갱신한다.
			}
		}
	}
}