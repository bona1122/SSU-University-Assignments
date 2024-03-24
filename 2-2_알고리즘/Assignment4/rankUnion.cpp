#include<stdio.h>
typedef struct _TreeNode {	//집합에서 사용할 트리노드 구조체 선언
	int rank;
	char value;
	struct _TreeNode* parent;
}TreeNode;
void Union(TreeNode a[], TreeNode b[]);	//rank를 이용하여 집합을 합치는 함수
void SetArrayPlus(TreeNode a[], TreeNode b[]);	/*a배열을 b배열에 합치는 함수
												(집합을 합칠 경우, 집합데이터를 하나의 배열에 합쳐주는 함수)*/
void PrintResult(TreeNode set[]); //집합을 출력하는 함수
int main()
{
	//(a)의 첫 번째 집합 구성(집합의 원소데이터들은 하나의 배열에 저장해 둘 것이다.)
	TreeNode a1set[10] = { 0 }; //집합의 원소들을 저장할 배열을 선언하고 0으로 초기화한다.(루트노드는 0번인덱스에 저장될 것이다.)
	//a노드 생성
	a1set[0].parent = &(a1set[0]);	//a노드는 a노드 자신을 가리키게 한다.
	a1set[0].value = 'a';	//a노드는 데이터값으로 a를 갖는다.
	a1set[0].rank = 1;	//a노드의 랭크는 1이다.
	//b노드 생성
	a1set[1].parent = &(a1set[0]); //b노드는 a노드를 가리키게 한다.
	a1set[1].value = 'b';	//b노드는 데이터값으로 b를 갖는다.
	a1set[1].rank = 0;	//b노드의 랭크는 0이다.

	//(a)의 두 번째 집합 구성
	TreeNode a2set[10] = { 0 };	//집합의 원소들을 저장할 배열을 선언하고 0으로 초기화한다.
	//c노드 생성
	a2set[0].parent = &(a2set[0]);	//c노드는 c노드 자신을 가리키게 한다.
	a2set[0].value = 'c';	//c노드는 데이터값으로 c를 갖는다.
	a2set[0].rank = 0;	//c노드의 랭크는 0이다.


	Union(a1set, a2set);	//(a)의 두 개의 집합을 합친다.
	printf("<(a)의 두 집합을 합친 결과 출력 시작>\n");
	PrintResult(a1set);	//합친 결과 a1set에 합쳐졌으므로 a1set을 출력한다.
	printf("\n\n");


	//(b)의 첫 번째 집합 구성
	TreeNode b1set[10] = { 0 };	//집합의 원소들을 저장할 배열을 선언하고 0으로 초기화한다.
	//d노드 생성
	b1set[0].parent = &(b1set[0]);	//d노드는 d노드 자신을 가리키게 한다.
	b1set[0].value = 'd';	//d노드는 데이터 값으로 d를 갖는다.
	b1set[0].rank = 1;	//d노드의 랭크는 1이다.
	//e노드 생성
	b1set[1].parent = &(b1set[0]);	//e노드는 d노드를 가리키게 한다.
	b1set[1].value = 'e';	//e노드는 데이터 값으로 d를 갖는다.
	b1set[1].rank = 0;	//e노드의 랭크는 0이다.

	//(b)의 두 번째 트리 구성
	TreeNode b2set[10] = { 0 };	//집합의 원소들을 저장할 배열을 선언하고 0으로 초기화한다.
	//f노드 생성
	b2set[0].parent = &(b2set[0]);	//f노드는 f노드 자신을 가리키게 한다.
	b2set[0].value = 'f';	//f노드는 데이터 값으로 f를 갖는다.
	b2set[0].rank = 1;	//f노드의 랭크는 1이다.
	//g노드 생성
	b2set[1].parent = &(b2set[0]);	//g노드는 f노드를 가리킨다.
	b2set[1].value = 'g';	//g노드는 데이터 값으로 g를 갖는다.
	b2set[1].rank = 0;	//g노드의 랭크는 0이다.
	//h노드 생성
	b2set[2].parent = &(b2set[0]);	//h노드는 f노드를 가리킨다.
	b2set[2].value = 'h';	//h노드는 데이터 값으로 h를 갖는다.
	b2set[2].rank = 0;	//h노드의 랭크는 0이다.
	//i노드 생성
	b2set[3].parent = &(b2set[0]);	//i노드는 f노드를 가리킨다.
	b2set[3].value = 'i';	//i노드는 데이터 값으로 i를 갖는다.
	b2set[3].rank = 0;	//i노드의 랭크는 0이다.


	Union(b1set, b2set);	//(b)의 두 개의 집합을 합친다.
	printf("<(b)의 두 집합을 합친 결과 출력 시작>\n");
	PrintResult(b2set);	//합친 결과 b1set에 합쳐졌으므로 b1set을 출력한다.
	printf("\n\n");


	Union(a1set, b2set);//(a)에서 합친 집합과 (b)에서 합친 집합을 합친다.
	printf("<(a)에서 합친 집합과 (b)에서 합친집합을 합친 결과 출력 시작>\n");
	PrintResult(b2set);//합친 결과 b2set에 합쳐졌으므로 b2set을 출력한다.

	return 0;
}
void Union(TreeNode a[], TreeNode b[])	//rank를 이용하여 집합을 합치는 함수
{
	if (a[0].rank > b[0].rank)	//배열a에 저장된 집합의 랭크가 더 높은 경우
	{
		b[0].parent = &(a[0]);	//b배열에 있는 집합을 a배열에 있는 집합에 붙인다.
		SetArrayPlus(b, a);	//b배열에 있는 원소들을 a배열로 옮긴다.
	}
	else   //배열a에 저장된 집합의 랭크가 더 낮거나 같은 경우
	{
		a[0].parent = &(b[0]);	//a배열에 있는 집합을 b배열에 있는 집합에 붙인다.
		SetArrayPlus(a, b);	//a배열에 있는 원소들을 b배열로 옮긴다.
		if (a[0].rank == b[0].rank) //만약 랭크가 같다면
		{
			b[0].rank++;	//b배열에 있는 집합의 랭크를 1 증가시킨다.
		}
	}
}
void SetArrayPlus(TreeNode a[], TreeNode b[]) //a배열을 b배열에 합치는 함수
{
	int i = 0;	//b배열에 차례로 접근하기 위한 변수 선언
	int j = 0;	//a배열에 차례로 접근하기 위한 변수 선언
	while (true)
	{
		if (b[i].value == '\0')	//b배열에서 i번인덱스에 원소가 없다면 멈춘다.
		{
			break;
		}
		i++;	//b배열에서 i번인덱스에 원소가 있다면 다음 인덱스로 넘어간다.
	}
	while (true)
	{
		if (a[j].value == '\0')	//a배열에서 j번인덱스에 원소가 없다면 멈춘다.
		{
			break;
		}
		b[i] = a[j];	//a배열의 원소를 차례로 b배열끝에 삽입한다.
		j++;	//a배열의 다음 원소를 b배열에 저장하기 위해 넘어간다.
		i++;	//b배열의 끝에 원소를 추가하기 위해 넘어간다.
	}
}
void PrintResult(TreeNode set[]) //집합을 출력하는 함수(각 노드는 어떤 노드를 가리키는지, 랭크는 몇인지 출력한다.)
{
	int i = 0; //집합의 원소를 저장한 배열의 0번인덱스부터 차례로 접근할 것이다.
	while (true)
	{
		if (set[i].value == '\0')	//출력을 할 원소가 없다면 출력을 그만한다.
			break;
		else  //출력할 원소가 있다면 정보를 출력한다.
		{
			printf("%c는 %c노드를 가리킵니다.(rank: %d)\n", set[i].value, set[i].parent->value, set[i].rank);
		}
		i++; //다음 원소에 접근하기 위해 증가시킨다.
	}
}