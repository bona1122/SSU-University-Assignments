#include <stdio.h>
int p[6] = { 0,20,30,35,12,3 }; //각 아이템의 가치를 저장한 배열
int w[6] = { 0,2,5,7,3,1 };	//각 아이템의 무게를 저장한 배열
int W = 9;  //최대가능한 무게
typedef struct Node{    // 상태공간트리에서의 노드
	int level;  //트리에서의 레벨
	int profit; //현재까지의 이익
	int weight; //현재까지의 무게
	float bound; //현재의 bound
}Node;
typedef struct priority_queue { //우선순위 큐 정의
    Node heap[100]; //최대힙 구조의 노드 배열
    int size;   //배열에 저장된 노드 개수
}PQ;
void insert(PQ* pq, Node data); //우선순위 큐에 노드를 삽입하는 함수
Node remove(PQ* pq);    //우선순위 큐에서 노드를 삭제하는 함수
float bound(Node u);    //bound를 계산하는 함수
void knapsack3(int n, int p[], int w[], int W, int maxprofit);  //best-first방법으로 knapsack문제를 해결하고 과정을 출력하는 함수
int empty(PQ pq);   //우선순위 큐가 비어있는지 확인하는 함수
void initialize(PQ* pq);    //우선순위 큐를 초기화하는 함수
int main(void)
{
    knapsack3(5,p,w,W,0);
    return 0;
}
void knapsack3(int n, int p[], int w[], int W,int maxprofit) {
    PQ pq;  //우선순위 큐 선언
    Node u, v;  //부모노드와 자식노드를 가리킬 변수 선언

    initialize(&pq);    //큐 초기화
    v.level = 0; v.profit = 0; v.weight = 0; v.bound = bound(v);    //루트노드를 v에 저장
    maxprofit = 0; //얻을 수 있는 최대이익을 저장하기위한 변수
    insert(&pq,v);  //큐에 루트노드를 삽입
    while (!empty(pq)) {    //큐가 비어있지 않은 경우
        v = remove(&pq);    //큐에서 노드를 삭제하여 삭제된 노드를 v에 저장
        if (v.bound > maxprofit) {  //v의 bound값이 maxprofit보다 큰 경우 
            u.level = v.level + 1;   //다음 아이템을 포함한 v의 자식노드의 정보 저장
            u.profit = v.profit + p[u.level];
            u.weight = v.weight + w[u.level];
            if ((u.weight <= W) && (u.profit > maxprofit)) {    //u가 최대무게를 넘지않고 이익이 maxprofit보다 큰 경우
                maxprofit = u.profit;   //maxprofit을 갱신
            }

            u.bound = bound(u); //u의 bound값을 저장
            if (bound(u) > maxprofit) { //u의 bound값이 maxprofit보다 크다면 큐에 삽입
                insert(&pq,u);
            }
            u.weight = v.weight;    //다음 아이템을 포함하지 않은 v의 자식노드의 정보 저장
            u.profit = v.profit;
            u.bound = bound(u);
            if (u.bound > maxprofit) insert(&pq,u); //u가 최대무게를 넘지않고 이익이 maxprofit보다 큰 경우
        }
    }
    printf("\nmaxprofit : %d\n", maxprofit);    //최대이익을 출력
}
void insert(PQ* pq,Node data) {
    pq->heap[pq->size] = data;  //큐에 노드를 삽입
    printf("큐안에 삽입\nlevel:%d / profit:%d / weight:%d / bound:%f\n\n", data.level, data.profit, data.weight,data.bound); //삽입된 노드의 정보를 출력

    int current = pq->size; //삽입된 노드의 자리를 정할 것이므로 삽입된 곳부터 시작
    int parent = (pq->size - 1) / 2;    //삽입된 노드의 부모노드부터 시작

    while (current > 0 && pq->heap[current].bound > pq->heap[parent].bound) { //노드의 bound값에 따라 최대힙구조에 맞게 삽입된 노드의 자리를 찾는다.
        Node temp;
        temp = pq->heap[current];
        pq->heap[current] = pq->heap[parent];
        pq->heap[parent] = temp;

        current = parent;
        parent = (parent - 1) / 2;
    }
    pq->size++;
    return;
}
Node remove(PQ* pq) {
    Node data = pq->heap[0];    //큐에서 노드를 삭제
    pq->size--;
    printf("큐에서 삭제\nlevel:%d / profit:%d / weight:%d / bound:%f\n\n", data.level, data.profit, data.weight,data.bound); //삭제한 노드의 정보를 출력

    pq->heap[0] = pq->heap[pq->size];   //배열의 마지막노드를 루트노드로 옮긴다.
    int current = 0;
    int leftC = current * 2 + 1;
    int rightC = current * 2 + 2;
    int maxNode = current;

    while (leftC < pq->size) {  //루트노드를 최대힙 구조에 맞게 자리를 정한다.      
        if (pq->heap[maxNode].bound < pq->heap[leftC].bound) {
            maxNode = leftC;
        }
        if (rightC < pq->size && pq->heap[maxNode].bound < pq->heap[rightC].bound) {
            maxNode = rightC;
        }
        if (maxNode == current) {
            break;
        }
        else {
            //heap[current]와 heap[maxNode]의 값을 바꾼다 
            Node temp;
            temp = pq->heap[current];
            pq->heap[current] = pq->heap[maxNode];
            pq->heap[maxNode] = temp;

            current = maxNode;
            leftC = current * 2 + 1;
            rightC = current * 2 + 2;
        }
    }
    return data;    //삭제된 노드(전 루트노드) 리턴
}
int empty(PQ pq) { 
    if (pq.size == 0) { //큐에 저장된 데이터의 개수가 0개이면 true리턴
        return 1;
    }
    else return 0;
}
void initialize(PQ* pq) 
{
    pq->size = 0;   //큐의 size값을 0으로 초기화
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