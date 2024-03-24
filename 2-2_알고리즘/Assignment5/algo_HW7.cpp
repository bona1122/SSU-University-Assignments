#include <stdio.h>
int tsp(int mask, int current, int W[][5]); //current정점부터 시작하여 다른 모든 정점을 지나 돌아오는 최소비용계산 함수
void printPath(int start, int mask); //start정점부터 시작하여 다른 모든 정점을 지나 돌아오는 경로를 출력하는 함수
int visit_all = (1 << 5) - 1; // 다 방문했을 때를 나타내는 변수 선언 및 초기화(00011111 --> 31 . 방문한 정점을 1로 표현)
int path[5][32]; //경로를 저장하기위한 배열
int main()
{
	int W[5][5] = { {0,8,13,18,20},{3,0,7,8,10},{4,11,0,10,7},{6,6,7,0,11},{10,6,2,1,0} }; //가중치 행렬 초기화
	int cost;
	for (int i = 0; i < 5; i++) //path배열 -1로 초기화
	{
		for (int j = 0; j < 32; j++)
		{
			path[i][j] = -1;
		}
	}
	cost = tsp(1, 0, W); //가중치 행렬을 이용하여 정점0부터 모든정점을 거치고 돌아오는 최소비용을 구한다.
	printf("최소 비용 : %d\n", cost); //최소비용을 출력한다.
	printPath(0,1);	//정점0부터 모든 정점을 거치고 돌아오는 경로를 출력한다.

	return 0;
}
int tsp(int mask, int current, int W[][5]) {
	if (mask == visit_all) { //모두 방문한 상태라면 
		return W[current][0]; //pos부터 0번 정점까지를 리턴한다.
	}
	int cost = 999; //최소비용을 구하기 위한 변수 선언 및 초기화
	for (int city = 1; city < 5; city++) { //현재 위치 다음으로 1~4정점으로 가는 것을 비교하기 위해 반복한다.

		if ((mask & (1 << city)) == 0) { //city번정점을 방문하지 않은 경우
			//city번정점을 방문하고 city번 정점부터의 최소비용을 구한다.
			int newCost = W[current][city] + tsp(mask | (1 << city), city, W); 
			if (newCost < cost) //새로운 비용이 기존의 비용과 비교해서 더 작다면 비용을 더 작은 값으로 갱신한다.
			{
				cost = newCost;
				path[current][mask] = city; //최소비용이 나오는 city정점을 저장한다.
			}
		}
	}
	return cost;
}
void printPath(int start, int mask)
{
	if (start == -1) //시작점이 -1 이라면 모든 정점을 방문한 상태일것이다. 따라서 0을 출력하고 끝낸다.
	{
		printf("%d\n", 0);
		return;
	}
	printf("%d --> ", start); //시작점을 출력한다.
	int next = path[start][mask]; //그 다음 정점을 next에 저장한다.
	printPath(next, mask | (1<< next)); //그 다음 정점을 방문한상태로 다음 경로를 출력한다.
}