#include <stdio.h>
int INF = 9999;		//������ �̾������� ������� �ִ밪���� ǥ���ϱ� ���� ����
typedef struct HeapNode {	//���� ������ ���� ���̹Ƿ� ������ �����͸� ���� �����ϱ� ���� ����ü ����
	int weight;
	int v1;
	int v2;
} HNode;
typedef struct {	//�� ����ü ����. ���� �ּ����� �����Ѵ�.
	HNode heap[14];	//�� 14���� ������ ������ ���̹Ƿ� ������� �迭�� �����Ѵ�.
	int heap_size;	//�迭�� ����� ��������� ������ ��Ÿ�� �����̴�.
}HeapType;
void union_set(int set1, int set2);	//�� ���� ������ ��ġ�� �Լ�
int find_set(int id);	//id�� ���ϴ� ������ �θ� ã�� �Լ�
void kruskal(int W[][8]);	//ũ�罺Į �˰����� ���� �ּҺ�����Ʈ���� ���ϴ� �Լ�
void insert_heap(HeapType* h, HNode e);	//�ּ��� ������ h���� e��带 �����ϴ� �Լ�
HNode delete_heap(HeapType* h);	//�ּ��� ������ h������ ��Ʈ��带 �����ϴ� �Լ�
void init_set(int nSets);	//�� �������� �ڱ��ڽŸ��� ���ҷ��ϴ� ������ ����� ���� �Լ�
int setCnt;	//������ ����
int parent[8];	//�� ������ ���ϴ� ������ �θ���
int main()
{
	int W[8][8] = {					//����ġ �迭 �ʱ�ȭ
		{0,11,9,8,INF,INF,INF,INF},
		{11,0,3,INF,8,8,INF,INF},
		{9,3,0,15,INF,12,1,INF},
		{8,INF,15,0,INF,INF,10,INF},
		{INF,8,INF,INF,0,7,INF,4},
		{INF,8,12,INF,7,0,INF,5},
		{INF,INF,1,10,INF,INF,0,2},
		{INF,INF,INF,INF,4,5,2,0} };

	kruskal(W);	//�ּҺ�����Ʈ�� ���ϱ�
	return 0;
}
void kruskal(int W[][8])
{
	int sum = 0;	//�ּҺ�����Ʈ���� �� ����� ������ ����
	HNode e;	//������ ��Ÿ�� ����
	HeapType mimHeap;	//�ּ� ���� �����Ѵ�.
	mimHeap.heap_size = 0;	//����� ������ ���� ��Ÿ���� ������ �ʱ�ȭ�Ѵ�.
	int set1, set2;	//������ ��Ÿ�� ���� ����
	int acceptedEdge = 0;	//���õ� ������ ������ ��Ÿ���� ����

	init_set(8);	//�� 8���� ������ ��� �������� ǥ���Ѵ�.

	for (int i = 0; i < 8; i++)
	{
		for (int j = i + 1; j < 8; j++)
		{
			if (W[i][j] < INF)		// ��� ������ ���� �����Ѵ�.
			{
				e.weight = W[i][j];
				e.v1 = i;
				e.v2 = j;
				insert_heap(&mimHeap,e);
			}
		}
	}
	while (acceptedEdge < 7) //���õ� ������ ���� n-1=7���� �۴ٸ�
	{
		e = delete_heap(&mimHeap);	//�ּ������� ����ġ�� �ּ��� ������ �����´�.

		set1 = find_set(e.v1);	//������ �̾��� �ִ� ����1�� ���ϴ� ������ �����Ѵ�.
		set2 = find_set(e.v2);	//������ �̾��� �ִ� ����2�� ���ϴ� ������ �����Ѵ�.
		if (set1 != set2)	//�� ���� ������ ���� �ʴٸ�, ������ �߰��Ѵ�.
		{
			sum = sum + e.weight;	//���� �߰��ϴ� ������ ����� �߰��Ѵ�.
			printf("���� �߰� : %d - %d (���: %d)\n",e.v1+1,e.v2+1,e.weight);	//�߰��Ǵ� ������ ������ ����Ѵ�.
			union_set(set1, set2);	//�� ���� ������ �ϳ��� ��ģ��.
			acceptedEdge++;	//���õ� ������ ������ 1 �ø���.
		}
	}
	printf("\n�ּҽ���Ʈ�� ��� :%d\n", sum);	//�� ����� ����Ѵ�.
}
int find_set(int id)
{
	while (parent[id] != id)	//�θ� �ڽ��� �ƴϸ� �θ�� �ö󰣴�.
	{
		id = parent[id];
	}
	return id;	//������ ��Ʈ�� ��ȯ�Ѵ�.
}
void union_set(int set1,int set2)
{
	parent[set1] = set2;	//set1�� set2�� ��ģ��.
}
void insert_heap(HeapType *h, HNode e)
{
	int i = (h->heap_size)++;	

	while (i != 0 && e.weight < h->heap[(i-1) / 2].weight) {	//i�� ��Ʈ��尡 �ƴϰ� �θ𺸴� ���� ���
		h->heap[i] = h->heap[(i - 1) / 2];	//i��° ���� �θ��带 ��ȯ
		i = (i - 1) / 2;	//�� �ܰ� ���� �ö󰣴�.
	}
	h->heap[i] = e;	//���ο� ��� ����
}
HNode delete_heap(HeapType* h)
{
	int parent, child;
	HNode item, temp;

	item = h->heap[0]; //��Ʈ ��� ���� ��ȯ�ϱ� ���� item�� �Ҵ�
	temp = h->heap[(h->heap_size)--];	//�������� ����� ��带 temp�� �����ϰ� ����� ��� ���� ���δ�.
	parent = 0;
	child = 1;
	
	while (child <= h->heap_size) {	//temp�� ����� ���� ã�´�.
		if (child < h->heap_size && h->heap[child].weight > h->heap[child + 1].weight)	//���� ����� �ڽ� ��� �� �� ���� �ڽ� ��带 ã�´�.
		{
			child++;
		}
		if (temp.weight <= h->heap[child].weight)
		{
			break;
		}
		h->heap[parent] = h->heap[child];	//����� ���� ã�� ���� ��� �Ѵܰ� ��������.
		parent = child;
		child = (child + 1) * 2 - 1;
	}
	h->heap[parent] = temp;	
	return item;	//ó���� �����ص� ��Ʈ��带 ��ȯ�Ѵ�.
}
void init_set(int nSets)
{
	for (int i = 0; i < nSets; i++)
	{
		parent[i] = i;	//��� ������ �ڱ��ڽ��� �θ�� �ϴ� ���� 1���� �������� �����.
	}
}