#include <stdio.h>
#include <unistd.h>

int main()
{
	printf("STDOUT_FILENO:%d\n", STDOUT_FILENO);
	close(STDOUT_FILENO);
	printf("Hello, world!\n");
	
	return 0;
}
