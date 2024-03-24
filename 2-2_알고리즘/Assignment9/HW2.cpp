#include <stdio.h>
int p[6] = { 0,20,30,35,12,3 }; //�� �������� ��ġ�� ������ �迭
int w[6] = { 0,2,5,7,3,1 };	//�� �������� ���Ը� ������ �迭
int W = 9;  //�ִ밡���� ����
typedef struct Node {   //���°���Ʈ�������� ���
    int level;  //Ʈ�������� ����
    int profit; //��������� ����
    int weight; //��������� ����
}Node;
typedef struct queue {  //ť
    int head;   //���� ������� ������ �������� �ε���
    int tail;   //���� �ֱٿ� ����� �������� �ε���
    Node nodeArray[100];    //���迭
}queue;
float bound(Node u);    //bound(������ �����۵��� �����Ͽ� ���� �� �ִ� �ִ�����)�� ������ִ� �Լ�
int queue_empty(queue q);   //ť�� ������� Ȯ���ϴ� �Լ�
void enqueue(queue* q, Node data);  //ť�� data�� �����ϴ� �Լ�
Node dequeue(queue* q); //ť���� data�� �����ϴ� �Լ�
void knapsack2(int n, int p[], int w[], int W, int maxprofit);  //�ʺ�켱Ž������ knapsack������ �ذ��ϰ� ������ ����ϴ� �Լ�
void initialize(queue* q);  //ť�� �ʱ�ȭ�ϴ� �Լ�
int main()
{
    knapsack2(5,p,w,W,0);
    return 0;
}
int queue_empty(queue q)    
{
    if (q.head == q.tail)  //head���� tail���� ���ٸ� ť�� ����ִ� ���̹Ƿ� true�� ����
        return 1;
    return 0;
}
void enqueue(queue* q, Node data)   
{
    q->nodeArray[q->head++] = data; //ť�� �����͸� ����
    printf("ť�ȿ� ����\nlevel:%d / profit:%d / weight:%d\n\n",data.level,data.profit,data.weight);  //�����ϴ� ����� ������ ���
    q->head = q->head % 100;
}
Node dequeue(queue* q)
{
    Node data;
    data = q->nodeArray[q->tail++]; //ť���� ������ ��带 ����
    printf("ť���� ����\nlevel:%d / profit:%d / weight:%d\n\n", data.level, data.profit, data.weight);   //�����ϴ� ����� ������ ���
    q->tail = q->tail % 100;
    return data;
}
void knapsack2(int n, int p[], int w[], int W, int maxprofit)
{
    queue Q;    //ť ����
    Node u, v;  //�θ���v�� �ڽĳ��v�� ��Ÿ�������� ����
    initialize(&Q); //ť �ʱ�ȭ
    v.level = 0;    //��Ʈ��� ���� �����ϱ� ���� ��Ʈ����� ���� �����Ѵ�.
    v.profit = 0;
    v.weight = 0;
    maxprofit = 0;  //���� �� �ִ� �ִ������� �����ϱ����� ����

    enqueue(&Q, v); //ť�� ��Ʈ��带 �����Ѵ�.
    while (!queue_empty(Q)) {   //ť�� ������� �������
        v = dequeue(&Q);    //ť���� ��带 ����
        u.level = v.level + 1;  //���� �������� ������ v�� �ڽĳ���� ���� ����
        u.profit = v.profit + p[u.level];   
        u.weight = v.weight + w[u.level];
        if ((u.weight <= W) && (u.profit > maxprofit))  //u�� �ִ빫�Ը� �����ʰ� ������ maxprofit���� ū ���  
            maxprofit = u.profit;   //maxprofit�� ����
        if (bound(u) > maxprofit) enqueue(&Q,u);    //u�� bound���� maxprofit���� ũ�ٸ� ť�� ����
        u.weight = v.weight;    //���� �������� �������� �ʴ� v�� �ڽĳ���� ������ ����
        u.profit = v.profit;
        if (bound(u) > maxprofit) enqueue(&Q, u);   //u�� bound���� maxprofit���� ũ�ٸ� ť�� ����
    }
    printf("\nmaxprofit : %d\n", maxprofit);    //�ִ������� ���
}
void initialize(queue* q)
{
    q->head = 0;    //ť�� head���� 0���� �ʱ�ȭ�Ѵ�.
    q->tail = 0;    //ť�� tail���� 0���� �ʱ�ȭ�Ѵ�.
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