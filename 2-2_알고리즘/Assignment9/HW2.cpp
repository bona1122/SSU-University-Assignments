#include <stdio.h>
int p[6] = { 0,20,30,35,12,3 }; //각 아이템의 가치를 저장한 배열
int w[6] = { 0,2,5,7,3,1 };	//각 아이템의 무게를 저장한 배열
int W = 9;  //최대가능한 무게
typedef struct Node {   //상태공간트리에서의 노드
    int level;  //트리에서의 레벨
    int profit; //현재까지의 이익
    int weight; //현재까지의 무게
}Node;
typedef struct queue {  //큐
    int head;   //가장 저장된지 오래된 데이터의 인덱스
    int tail;   //가장 최근에 저장된 데이터의 인덱스
    Node nodeArray[100];    //노드배열
}queue;
float bound(Node u);    //bound(이후의 아이템들을 포함하여 얻을 수 있는 최대이익)를 계산해주는 함수
int queue_empty(queue q);   //큐가 비었는지 확인하는 함수
void enqueue(queue* q, Node data);  //큐에 data를 삽입하는 함수
Node dequeue(queue* q); //큐에서 data를 삭제하는 함수
void knapsack2(int n, int p[], int w[], int W, int maxprofit);  //너비우선탐색으로 knapsack문제를 해결하고 과정을 출력하는 함수
void initialize(queue* q);  //큐를 초기화하는 함수
int main()
{
    knapsack2(5,p,w,W,0);
    return 0;
}
int queue_empty(queue q)    
{
    if (q.head == q.tail)  //head값과 tail값이 같다면 큐가 비어있는 것이므로 true를 리턴
        return 1;
    return 0;
}
void enqueue(queue* q, Node data)   
{
    q->nodeArray[q->head++] = data; //큐에 데이터를 삽입
    printf("큐안에 삽입\nlevel:%d / profit:%d / weight:%d\n\n",data.level,data.profit,data.weight);  //삽입하는 노드의 정보를 출력
    q->head = q->head % 100;
}
Node dequeue(queue* q)
{
    Node data;
    data = q->nodeArray[q->tail++]; //큐에서 삭제할 노드를 저장
    printf("큐에서 삭제\nlevel:%d / profit:%d / weight:%d\n\n", data.level, data.profit, data.weight);   //삭제하는 노드의 정보를 출력
    q->tail = q->tail % 100;
    return data;
}
void knapsack2(int n, int p[], int w[], int W, int maxprofit)
{
    queue Q;    //큐 선언
    Node u, v;  //부모노드v와 자식노드v를 나타내기위한 변수
    initialize(&Q); //큐 초기화
    v.level = 0;    //루트노드 부터 시작하기 위해 루트노드의 값을 저장한다.
    v.profit = 0;
    v.weight = 0;
    maxprofit = 0;  //얻을 수 있는 최대이익을 저장하기위한 변수

    enqueue(&Q, v); //큐에 루트노드를 삽입한다.
    while (!queue_empty(Q)) {   //큐가 비어있지 않은경우
        v = dequeue(&Q);    //큐에서 노드를 삭제
        u.level = v.level + 1;  //다음 아이템을 포함한 v의 자식노드의 정보 저장
        u.profit = v.profit + p[u.level];   
        u.weight = v.weight + w[u.level];
        if ((u.weight <= W) && (u.profit > maxprofit))  //u가 최대무게를 넘지않고 이익이 maxprofit보다 큰 경우  
            maxprofit = u.profit;   //maxprofit을 갱신
        if (bound(u) > maxprofit) enqueue(&Q,u);    //u의 bound값이 maxprofit보다 크다면 큐에 삽입
        u.weight = v.weight;    //다음 아이템을 포함하지 않는 v의 자식노드의 정보를 저장
        u.profit = v.profit;
        if (bound(u) > maxprofit) enqueue(&Q, u);   //u의 bound값이 maxprofit보다 크다면 큐에 삽입
    }
    printf("\nmaxprofit : %d\n", maxprofit);    //최대이익을 출력
}
void initialize(queue* q)
{
    q->head = 0;    //큐의 head값을 0으로 초기화한다.
    q->tail = 0;    //큐의 tail값을 0으로 초기화한다.
}
float bound(Node u)
{
    float result;   //bound 값을 저장할 변수
    int totweight;  //현재까지의 무게에 W를 초과하지 않게끔 다음아이템들을 저장한 무게
    int j;
    int k;
    if (u.weight >= W)  //현재까지의 무게가 W를 초과하는 경우 false를 리턴
        return 0;
    else {
        result = u.profit;  //현재까지의 이익을 저장
        j = u.level + 1;    //다음에 넣을 아이템을 저장
        totweight = u.weight;   //현재까지의 무게를 저장

        while ((j <= 5) && (totweight + w[j] <= W)) //totweight이 W를 초과하기 전까지 다음아이템들을 취한다.
        {
            totweight = totweight + w[j];
            result = result + p[j];
            j++;
        }
        k = j;  //totweight이 W를 초과하게 되는 아이템을 저장
        if (k <= 5)
        {
            result = result + (W - totweight) * p[k] / w[k];    //남은 공간이 허용하는무게만큼 마지막 아이템의 일부분을 취하고 bound에 더한다.
        }
        return result; //bound값을 리턴
    }
}