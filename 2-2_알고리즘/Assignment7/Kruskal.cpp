#include <stdio.h>
int INF = 9999;		//정점이 이어져있지 않은경우 최대값으로 표기하기 위한 변수
typedef struct HeapNode {	//힙에 간선을 넣을 것이므로 간선의 데이터를 노드로 저장하기 위한 구조체 선언
	int weight;
	int v1;
	int v2;
} HNode;
typedef struct {	//힙 구조체 선언. 힙은 최소힙을 유지한다.
	HNode heap[14];	//총 14개의 간선을 저장할 것이므로 간선노드 배열을 선언한다.
	int heap_size;	//배열에 저장된 간선노드의 개수를 나타낼 변수이다.
}HeapType;
void union_set(int set1, int set2);	//두 개의 집합을 합치는 함수
int find_set(int id);	//id가 속하는 집합의 부모를 찾는 함수
void kruskal(int W[][8]);	//크루스칼 알고리즘을 통해 최소비용신장트리를 구하는 함수
void insert_heap(HeapType* h, HNode e);	//최소힙 구조인 h힙에 e노드를 삽입하는 함수
HNode delete_heap(HeapType* h);	//최소힙 구조인 h힙에서 루트노드를 삭제하는 함수
void init_set(int nSets);	//각 정점마다 자기자신만을 원소로하는 집합을 만들기 위한 함수
int setCnt;	//집합의 개수
int parent[8];	//각 정점이 속하는 집합의 부모노드
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

	kruskal(W);	//최소비용신장트리 구하기
	return 0;
}
void kruskal(int W[][8])
{
	int sum = 0;	//최소비용신장트리의 총 비용을 저장할 변수
	HNode e;	//간선을 나타낼 변수
	HeapType mimHeap;	//최소 힙을 생성한다.
	mimHeap.heap_size = 0;	//저장된 간선의 수를 나타내는 변수를 초기화한다.
	int set1, set2;	//집합을 나타낼 변수 선언
	int acceptedEdge = 0;	//선택된 간선의 개수를 나타내는 변수

	init_set(8);	//총 8개의 정점을 모두 집합으로 표현한다.

	for (int i = 0; i < 8; i++)
	{
		for (int j = i + 1; j < 8; j++)
		{
			if (W[i][j] < INF)		// 모든 간선을 힙에 저장한다.
			{
				e.weight = W[i][j];
				e.v1 = i;
				e.v2 = j;
				insert_heap(&mimHeap,e);
			}
		}
	}
	while (acceptedEdge < 7) //선택된 간선의 수가 n-1=7보다 작다면
	{
		e = delete_heap(&mimHeap);	//최소힙에서 가중치가 최소인 간선을 가져온다.

		set1 = find_set(e.v1);	//간선에 이어져 있는 정점1이 속하는 집합을 저장한다.
		set2 = find_set(e.v2);	//간선에 이어져 있는 정점2가 속하는 집합을 저장한다.
		if (set1 != set2)	//두 개의 집합이 같지 않다면, 간선을 추가한다.
		{
			sum = sum + e.weight;	//새로 추가하는 간선의 비용을 추가한다.
			printf("간선 추가 : %d - %d (비용: %d)\n",e.v1+1,e.v2+1,e.weight);	//추가되는 간선의 정보를 출력한다.
			union_set(set1, set2);	//두 개의 집합을 하나로 합친다.
			acceptedEdge++;	//선택된 간선의 개수를 1 늘린다.
		}
	}
	printf("\n최소신장트리 비용 :%d\n", sum);	//총 비용을 출력한다.
}
int find_set(int id)
{
	while (parent[id] != id)	//부모가 자신이 아니면 부모로 올라간다.
	{
		id = parent[id];
	}
	return id;	//집합의 루트를 반환한다.
}
void union_set(int set1,int set2)
{
	parent[set1] = set2;	//set1을 set2에 합친다.
}
void insert_heap(HeapType *h, HNode e)
{
	int i = (h->heap_size)++;	

	while (i != 0 && e.weight < h->heap[(i-1) / 2].weight) {	//i가 루트노드가 아니고 부모보다 작은 경우
		h->heap[i] = h->heap[(i - 1) / 2];	//i번째 노드와 부모노드를 교환
		i = (i - 1) / 2;	//한 단계 위로 올라간다.
	}
	h->heap[i] = e;	//새로운 노드 삽입
}
HNode delete_heap(HeapType* h)
{
	int parent, child;
	HNode item, temp;

	item = h->heap[0]; //루트 노드 값을 반환하기 위해 item에 할당
	temp = h->heap[(h->heap_size)--];	//마지막에 저장된 노드를 temp에 저장하고 저장된 노드 수를 줄인다.
	parent = 0;
	child = 1;
	
	while (child <= h->heap_size) {	//temp가 저장될 곳을 찾는다.
		if (child < h->heap_size && h->heap[child].weight > h->heap[child + 1].weight)	//현재 노드의 자식 노드 중 더 작은 자식 노드를 찾는다.
		{
			child++;
		}
		if (temp.weight <= h->heap[child].weight)
		{
			break;
		}
		h->heap[parent] = h->heap[child];	//저장될 곳을 찾지 못한 경우 한단계 내려간다.
		parent = child;
		child = (child + 1) * 2 - 1;
	}
	h->heap[parent] = temp;	
	return item;	//처음에 저장해둔 루트노드를 반환한다.
}
void init_set(int nSets)
{
	for (int i = 0; i < nSets; i++)
	{
		parent[i] = i;	//모든 정점을 자기자신을 부모로 하는 원소 1개인 집합으로 만든다.
	}
}