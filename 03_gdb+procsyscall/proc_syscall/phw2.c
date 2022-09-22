#include <stdio.h>
#include <stdlib.h>     // exit
#include <unistd.h>     // fork, close
#include <string.h>     // strdup
#include <fcntl.h>      // open(flags, modes)
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int ret_fork = fork();
    if (ret_fork < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (ret_fork == 0) {
        // redirect the output of "ls -al" to the file "ls-output.txt"
        close(STDOUT_FILENO);
		printf("closed stdout\n");
        open("FILE", O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR);
        char *myargs[3];
        myargs[0] = strdup("app");	// program
        myargs[1] = strdup("arg1");	// argument 1
        myargs[2] = NULL;			// marks end of array
        execvp(myargs[0], myargs);	// runs word count
    } else {
        wait(NULL);
    }
	printf("[pid-%d] Hello, world!\n", getpid());
    return 0;
}
