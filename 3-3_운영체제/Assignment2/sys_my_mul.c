#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/syscalls.h>
asmlinkage int sys_my_mul(int num1, int num2)
{
        printk("HI_SYSTEMCALL_num1*num2 = %d\n",num1*num2);
        return num1*num2;
}
 
SYSCALL_DEFINE2(my_mul, int, num1, int, num2)
{       
        return sys_my_mul(num1,num2);
}
