#include <stdio.h>
#include <stdlib.h>
#pragma warning (disable:4996)
typedef struct _TreeNode {		//트리노드 구조체 선언
	char value;
	struct _TreeNode* rChild;
	struct _TreeNode* lChild;
}TreeNode;

TreeNode* searchNode(TreeNode*, char);	//특정 문자 데이터를 갖고 있는 노드를 검색하여 해당 노드를 가리키는 포인터를 반환하는 함수
void treeInOrder(TreeNode*);	//중위순회로 이진트리를 출력하는 함수
void freeNode(TreeNode*);	//트리에 동적할당된 노드들의 메모리를 해제하는 함수

int main()
{
	int n;	//노드의 총 개수를 저장할 변수
	TreeNode* root = NULL;	//루트노드를 가리키는 포인터변수

	scanf("%d", &n);
	getchar();

	for (int i = 0; i < n; i++)
	{
		char parent, leftchild, rightchild;
		scanf("%c %c %c", &parent, &leftchild, &rightchild);
		getchar();

		if (i == 0) {	//반복문의 첫번째로 루트노드를 생성한다
			root = (TreeNode*)calloc(1, sizeof(TreeNode)); //루트 노드 만들기
			root->value = 'A'; //루트노드 값 넣기
		}

		TreeNode* searchedNode;	//입력받은 정보로 트리 내의 노드를 검색하고 검색된 노드를 가리키는 포인터변수
		searchedNode = NULL;
		searchedNode = searchNode(root, parent); //트리 내의 노드 중 parent를 데이터로 갖고 있는 노드를 검색한다.

		if (leftchild != '.')	//왼쪽 자식의 데이터를 입력받았다면
		{
			searchedNode->lChild = (TreeNode*)calloc(1, sizeof(TreeNode));	//왼쪽 자식 노드를 동적할당	
			searchedNode->lChild->value = leftchild;	//왼쪽 자식 노드에 데이터 할당
		}
		if (rightchild != '.')	//오른쪽 자식의 데이터를 입력받았다면
		{
			searchedNode->rChild = (TreeNode*)calloc(1, sizeof(TreeNode));	//오른쪽 자식 노드를 동적할당
			searchedNode->rChild->value = rightchild;	//오른쪽 자식 노드에 데이터 할당
		}
	}
	treeInOrder(root);	//트리를 중위순회로 출력

	freeNode(root); //힙에 할당된 트리의 노드 해제
	root = NULL;

	return 0;
}
TreeNode* searchNode(TreeNode* currNode, char parent)
{
	TreeNode* searchedNode = NULL;	//검색된 노드를 가리킬 포인터 변수

	if (currNode) {	//현재 가리키는 노드가 있는 경우
		if (currNode->value == parent) {  //현재 노드가 찾는 노드인 경우
			searchedNode = currNode;
			return searchedNode;	//검색된 노드를 가리키는 포인터 반환
		}
		else //현재 노드가 찾는 노드가 아닌 경우
		{
			if (currNode->lChild != NULL) { //현재노드의 왼쪽 자식이 있는 경우
				searchedNode = searchNode(currNode->lChild, parent); //왼쪽 자식에서 노드 검색
				if (searchedNode != NULL) //왼쪽 자식에서 노드를 찾은 경우
					return searchedNode;	//검색된 노드를 가리키는 포인터 반환
			}

			//왼쪽자식에서 못 찾은 경우, 여기까지 제어가 내려온다
			if (currNode->rChild != NULL) { //현재노드의 오른쪽 자식이 있는 경우
				searchedNode = searchNode(currNode->rChild, parent); //오른쪽자식에서 노드 검색
				if (searchedNode != NULL) //오른쪽 자식에서 노드를 찾은 경우
					return searchedNode;	//검색된 노드를 가리키는 포인터 반환
			}			
			return searchedNode;	//오른쪽자식과 왼쪽 자식에서 못 찾은 경우, 여기까지 제어가 내려온다. 널을 가진 포인터 반환
		}
	}
	else //현재 가리키는 노드가 없는 경우	
		return searchedNode;	//널 값을 갖는 포인터를 반환
}
void treeInOrder(TreeNode* currNode)
{
	if (currNode)	//현재 가리키는 노드가 존재한다면
	{
		treeInOrder(currNode->lChild);	//현재 노드의 왼쪽 자식노드의 데이터 출력
		printf("%c", currNode->value);	//현재 노드의 데이터 출력
		treeInOrder(currNode->rChild);	//현재 노드의 오른쪽 자식노드의 데이터 출력
	}
}
void freeNode(TreeNode* currNode)
{
	if (currNode)	//현재 가리키는 노드가 존재한다면
	{
		freeNode(currNode->lChild);	//현재 노드의 왼쪽 자식노드 메모리 해제
		freeNode(currNode->rChild);	//현재 노드의 오른쪽 자식노드 메모리 해제
		free(currNode); //현재 노드 메모리 해제
	}
}