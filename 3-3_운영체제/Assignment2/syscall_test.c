#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<ctype.h>
int main(int argc, char* argv[]) {
	char str[50];
	int num[2] = { 0 };
	int result;
	char op;
	printf("input : ");
	scanf("%s",str);
	int length = strlen(str);
	for (int i = 0; i < length; i++) {
		if (!isdigit(str[i])) {
			op = str[i];
			break;
		}
	}
	char* ptr = strtok(str, &op);
	int idx = 0;
	while (ptr != NULL) {
		num[idx++] = atoi(ptr);
		ptr = strtok(NULL, &op);
	}
	switch(op){
		case '+':
			result = syscall(443,num[0],num[1]);
			break;

		case '-':
			result = syscall(444,num[0],num[1]);
			break;

		case '*':
			result = syscall(445,num[0],num[1]);
			break;
		case '%':
			result = syscall(446,num[0],num[1]);
			break;
	}
	printf("%d\n",result);
	return 0;
}
