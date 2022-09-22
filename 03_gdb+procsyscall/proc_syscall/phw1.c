#include <stdio.h>
#include <stdlib.h> // exit
#include <unistd.h> // fork, getpid, sleep
#include <sys/wait.h>

int data = 20;

int main(int argc, char *argv[])
{
    int var = 40;
    int ret_fork = fork();

    // fork failed; exit
    if (ret_fork < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);

    // child process
    } else if (ret_fork == 0) {
		data++;
		var++;
		printf("[child] data:%d, var:%d\n", data, var);

    // parent process
    } else {
		data++;
		var++;
		printf("[parent] data:%d, var:%d\n", data, var);
    }

    printf("[pid-%d] data:%d, var:%d\n", getpid(), data, var);
    return 0;
}
