#include<stdio.h>
void path(int q, int r);	//최단거리 경로를 출력하는 함수
void floyd2(int n, int W[][7]);	//최단경로를 구하고, 최단거리 경로를 저장하는 함수
int P[7][7] = { 0 };	//최단거리 경로를 저장하는 배열
int D[7][7] = { 0 };	//최단경로를 저장하는 배열
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

	floyd2(7, W);	//최단경로와 최단거리 경로를 저장한다.

	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			printf("v%d부터 v%d까지의 최단경로: %d\n", i + 1, j + 1, D[i][j]);	//정점i부터 정점j까지의 최단경로를 출력한다.
		}
		printf("\n");
	}

	//정점3부터 정점6까지의 최단거리 경로를 출력한다.
	printf("path(3,6): v3-->");	
	path(2, 5);
	printf("v6\n");

	return 0;
}
void floyd2(int n, int W[][7])
{
	int i, j, k;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			P[i][j] = -1;	//P배열을 -1로 초기화한다.
			D[i][j] = W[i][j];	//어떠한 정점도 통하지 않고 i부터 j까지 가는 경로 저장	
		}
	}
	for (k = 0; k < n; k++)	//거칠 수 있는 정점의 수를 1개부터 7개까지 늘려간다.
	{
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < n; j++)
			{
				if (D[i][k] != INF && D[k][j] != INF)	//i부터 k까지의 최단경로, k부터 j까지의 최단경로가 존재하는 경우
				{
					if (D[i][k] + D[k][j]< D[i][j])	//기존의 i부터 j까지의 최단경로와 비교하여 k를거치는 최단경로가 더 작은 경우
					{
						P[i][j] = k;	//i에서 j까지 갈 때, i다음으로 k로 가는 것으로 저장한다.
						D[i][j] = D[i][k] + D[k][j];	//i부터 j까지의 최단경로를 갱신한다.
					}
				}
			}
		}
	}
}
void path(int q, int r)
{
	if (P[q][r] != -1)	//q에서 r까지 갈 때, 거치는 정점이 있는경우. 거치는 정점을 a(P[q][r])라하자.
	{
		path(q, P[q][r]);	//q에서 a까지 갈 때의 경로를 출력
		printf("v%d-->", P[q][r] + 1);	//a를 출력
		path(P[q][r], r);	//a에서 r까지 갈 때의 경로를 출력
	}
}