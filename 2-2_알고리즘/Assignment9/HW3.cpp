#include <stdio.h>
int p[6] = { 0,20,30,35,12,3 }; //�� �������� ��ġ�� ������ �迭
int w[6] = { 0,2,5,7,3,1 };	//�� �������� ���Ը� ������ �迭
int W = 9;  //�ִ밡���� ����
typedef struct Node{    // ���°���Ʈ�������� ���
	int level;  //Ʈ�������� ����
	int profit; //��������� ����
	int weight; //��������� ����
	float bound; //������ bound
}Node;
typedef struct priority_queue { //�켱���� ť ����
    Node heap[100]; //�ִ��� ������ ��� �迭
    int size;   //�迭�� ����� ��� ����
}PQ;
void insert(PQ* pq, Node data); //�켱���� ť�� ��带 �����ϴ� �Լ�
Node remove(PQ* pq);    //�켱���� ť���� ��带 �����ϴ� �Լ�
float bound(Node u);    //bound�� ����ϴ� �Լ�
void knapsack3(int n, int p[], int w[], int W, int maxprofit);  //best-first������� knapsack������ �ذ��ϰ� ������ ����ϴ� �Լ�
int empty(PQ pq);   //�켱���� ť�� ����ִ��� Ȯ���ϴ� �Լ�
void initialize(PQ* pq);    //�켱���� ť�� �ʱ�ȭ�ϴ� �Լ�
int main(void)
{
    knapsack3(5,p,w,W,0);
    return 0;
}
void knapsack3(int n, int p[], int w[], int W,int maxprofit) {
    PQ pq;  //�켱���� ť ����
    Node u, v;  //�θ���� �ڽĳ�带 ����ų ���� ����

    initialize(&pq);    //ť �ʱ�ȭ
    v.level = 0; v.profit = 0; v.weight = 0; v.bound = bound(v);    //��Ʈ��带 v�� ����
    maxprofit = 0; //���� �� �ִ� �ִ������� �����ϱ����� ����
    insert(&pq,v);  //ť�� ��Ʈ��带 ����
    while (!empty(pq)) {    //ť�� ������� ���� ���
        v = remove(&pq);    //ť���� ��带 �����Ͽ� ������ ��带 v�� ����
        if (v.bound > maxprofit) {  //v�� bound���� maxprofit���� ū ��� 
            u.level = v.level + 1;   //���� �������� ������ v�� �ڽĳ���� ���� ����
            u.profit = v.profit + p[u.level];
            u.weight = v.weight + w[u.level];
            if ((u.weight <= W) && (u.profit > maxprofit)) {    //u�� �ִ빫�Ը� �����ʰ� ������ maxprofit���� ū ���
                maxprofit = u.profit;   //maxprofit�� ����
            }

            u.bound = bound(u); //u�� bound���� ����
            if (bound(u) > maxprofit) { //u�� bound���� maxprofit���� ũ�ٸ� ť�� ����
                insert(&pq,u);
            }
            u.weight = v.weight;    //���� �������� �������� ���� v�� �ڽĳ���� ���� ����
            u.profit = v.profit;
            u.bound = bound(u);
            if (u.bound > maxprofit) insert(&pq,u); //u�� �ִ빫�Ը� �����ʰ� ������ maxprofit���� ū ���
        }
    }
    printf("\nmaxprofit : %d\n", maxprofit);    //�ִ������� ���
}
void insert(PQ* pq,Node data) {
    pq->heap[pq->size] = data;  //ť�� ��带 ����
    printf("ť�ȿ� ����\nlevel:%d / profit:%d / weight:%d / bound:%f\n\n", data.level, data.profit, data.weight,data.bound); //���Ե� ����� ������ ���

    int current = pq->size; //���Ե� ����� �ڸ��� ���� ���̹Ƿ� ���Ե� ������ ����
    int parent = (pq->size - 1) / 2;    //���Ե� ����� �θ������ ����

    while (current > 0 && pq->heap[current].bound > pq->heap[parent].bound) { //����� bound���� ���� �ִ��������� �°� ���Ե� ����� �ڸ��� ã�´�.
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
    Node data = pq->heap[0];    //ť���� ��带 ����
    pq->size--;
    printf("ť���� ����\nlevel:%d / profit:%d / weight:%d / bound:%f\n\n", data.level, data.profit, data.weight,data.bound); //������ ����� ������ ���

    pq->heap[0] = pq->heap[pq->size];   //�迭�� ��������带 ��Ʈ���� �ű��.
    int current = 0;
    int leftC = current * 2 + 1;
    int rightC = current * 2 + 2;
    int maxNode = current;

    while (leftC < pq->size) {  //��Ʈ��带 �ִ��� ������ �°� �ڸ��� ���Ѵ�.      
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
            //heap[current]�� heap[maxNode]�� ���� �ٲ۴� 
            Node temp;
            temp = pq->heap[current];
            pq->heap[current] = pq->heap[maxNode];
            pq->heap[maxNode] = temp;

            current = maxNode;
            leftC = current * 2 + 1;
            rightC = current * 2 + 2;
        }
    }
    return data;    //������ ���(�� ��Ʈ���) ����
}
int empty(PQ pq) { 
    if (pq.size == 0) { //ť�� ����� �������� ������ 0���̸� true����
        return 1;
    }
    else return 0;
}
void initialize(PQ* pq) 
{
    pq->size = 0;   //ť�� size���� 0���� �ʱ�ȭ
}
float bound(Node u)
{
    float result;   //bound ���� ������ ����
    int totweight;  //��������� ���Կ� W�� �ʰ����� �ʰԲ� ���������۵��� ������ ����
    int j;
    int k;
    if (u.weight >= W)  //��������� ���԰� W�� �ʰ��ϴ� ��� false�� ����
        return 0;
    else {
        result = u.profit;  //��������� ������ ����
        j = u.level + 1;    //������ ���� �������� ����
        totweight = u.weight;   //��������� ���Ը� ����

        while ((j <= 5) && (totweight + w[j] <= W)) //totweight�� W�� �ʰ��ϱ� ������ ���������۵��� ���Ѵ�.
        {
            totweight = totweight + w[j];
            result = result + p[j];
            j++;
        }
        k = j;  //totweight�� W�� �ʰ��ϰ� �Ǵ� �������� ����
        if (k <= 5)
        {
            result = result + (W - totweight) * p[k] / w[k];    //���� ������ ����ϴ¹��Ը�ŭ ������ �������� �Ϻκ��� ���ϰ� bound�� ���Ѵ�.
        }
        return result; //bound���� ����
    }
}