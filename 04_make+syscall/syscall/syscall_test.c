#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

#define SYSCALL_NUM 548
#define STUDENT_NUM 202212345 // put your number

int main()
{
	int n = (int) syscall(SYSCALL_NUM, STUDENT_NUM);
	printf("syscall return %d\n", n);
	return 0;
}

