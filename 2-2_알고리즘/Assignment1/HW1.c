#include <stdio.h>
#include <stdlib.h>
#pragma warning (disable:4996)
typedef struct _TreeNode {		//Ʈ����� ����ü ����
	char value;
	struct _TreeNode* rChild;
	struct _TreeNode* lChild;
}TreeNode;

TreeNode* searchNode(TreeNode*, char);	//Ư�� ���� �����͸� ���� �ִ� ��带 �˻��Ͽ� �ش� ��带 ����Ű�� �����͸� ��ȯ�ϴ� �Լ�
void treeInOrder(TreeNode*);	//������ȸ�� ����Ʈ���� ����ϴ� �Լ�
void freeNode(TreeNode*);	//Ʈ���� �����Ҵ�� ������ �޸𸮸� �����ϴ� �Լ�

int main()
{
	int n;	//����� �� ������ ������ ����
	TreeNode* root = NULL;	//��Ʈ��带 ����Ű�� �����ͺ���

	scanf("%d", &n);
	getchar();

	for (int i = 0; i < n; i++)
	{
		char parent, leftchild, rightchild;
		scanf("%c %c %c", &parent, &leftchild, &rightchild);
		getchar();

		if (i == 0) {	//�ݺ����� ù��°�� ��Ʈ��带 �����Ѵ�
			root = (TreeNode*)calloc(1, sizeof(TreeNode)); //��Ʈ ��� �����
			root->value = 'A'; //��Ʈ��� �� �ֱ�
		}

		TreeNode* searchedNode;	//�Է¹��� ������ Ʈ�� ���� ��带 �˻��ϰ� �˻��� ��带 ����Ű�� �����ͺ���
		searchedNode = NULL;
		searchedNode = searchNode(root, parent); //Ʈ�� ���� ��� �� parent�� �����ͷ� ���� �ִ� ��带 �˻��Ѵ�.

		if (leftchild != '.')	//���� �ڽ��� �����͸� �Է¹޾Ҵٸ�
		{
			searchedNode->lChild = (TreeNode*)calloc(1, sizeof(TreeNode));	//���� �ڽ� ��带 �����Ҵ�	
			searchedNode->lChild->value = leftchild;	//���� �ڽ� ��忡 ������ �Ҵ�
		}
		if (rightchild != '.')	//������ �ڽ��� �����͸� �Է¹޾Ҵٸ�
		{
			searchedNode->rChild = (TreeNode*)calloc(1, sizeof(TreeNode));	//������ �ڽ� ��带 �����Ҵ�
			searchedNode->rChild->value = rightchild;	//������ �ڽ� ��忡 ������ �Ҵ�
		}
	}
	treeInOrder(root);	//Ʈ���� ������ȸ�� ���

	freeNode(root); //���� �Ҵ�� Ʈ���� ��� ����
	root = NULL;

	return 0;
}
TreeNode* searchNode(TreeNode* currNode, char parent)
{
	TreeNode* searchedNode = NULL;	//�˻��� ��带 ����ų ������ ����

	if (currNode) {	//���� ����Ű�� ��尡 �ִ� ���
		if (currNode->value == parent) {  //���� ��尡 ã�� ����� ���
			searchedNode = currNode;
			return searchedNode;	//�˻��� ��带 ����Ű�� ������ ��ȯ
		}
		else //���� ��尡 ã�� ��尡 �ƴ� ���
		{
			if (currNode->lChild != NULL) { //�������� ���� �ڽ��� �ִ� ���
				searchedNode = searchNode(currNode->lChild, parent); //���� �ڽĿ��� ��� �˻�
				if (searchedNode != NULL) //���� �ڽĿ��� ��带 ã�� ���
					return searchedNode;	//�˻��� ��带 ����Ű�� ������ ��ȯ
			}

			//�����ڽĿ��� �� ã�� ���, ������� ��� �����´�
			if (currNode->rChild != NULL) { //�������� ������ �ڽ��� �ִ� ���
				searchedNode = searchNode(currNode->rChild, parent); //�������ڽĿ��� ��� �˻�
				if (searchedNode != NULL) //������ �ڽĿ��� ��带 ã�� ���
					return searchedNode;	//�˻��� ��带 ����Ű�� ������ ��ȯ
			}			
			return searchedNode;	//�������ڽİ� ���� �ڽĿ��� �� ã�� ���, ������� ��� �����´�. ���� ���� ������ ��ȯ
		}
	}
	else //���� ����Ű�� ��尡 ���� ���	
		return searchedNode;	//�� ���� ���� �����͸� ��ȯ
}
void treeInOrder(TreeNode* currNode)
{
	if (currNode)	//���� ����Ű�� ��尡 �����Ѵٸ�
	{
		treeInOrder(currNode->lChild);	//���� ����� ���� �ڽĳ���� ������ ���
		printf("%c", currNode->value);	//���� ����� ������ ���
		treeInOrder(currNode->rChild);	//���� ����� ������ �ڽĳ���� ������ ���
	}
}
void freeNode(TreeNode* currNode)
{
	if (currNode)	//���� ����Ű�� ��尡 �����Ѵٸ�
	{
		freeNode(currNode->lChild);	//���� ����� ���� �ڽĳ�� �޸� ����
		freeNode(currNode->rChild);	//���� ����� ������ �ڽĳ�� �޸� ����
		free(currNode); //���� ��� �޸� ����
	}
}