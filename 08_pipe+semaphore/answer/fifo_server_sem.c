#include <fcntl.h> // open
#include <stdio.h> // perror
#include <stdlib.h> // exit
#include <string.h> // memset
#include <sys/stat.h> // mkfifo
#include <unistd.h> // close, read
#include <pthread.h>
#include <semaphore.h>

#define BUF_SIZE 64
#define RECV_PATH "./fifo1"
#define SEND_PATH "./fifo2"

int recv_fd, send_fd;
char rbuf[BUF_SIZE];
char wbuf[BUF_SIZE];
pthread_t t1, t2;

int any_exit = 0; // thread exit state variable. 1 means going to exit.
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;

/*TODO*/
// You need to make another source code for client.
// Refer this file.
/*TODO*/


void *recv(void *arg)
{
	int rc, cnt = 0;
	/*=======TODO=======*/

	/*=======TODO=======*/
	return NULL;
}

void *send(void *arg)
{
	int rc;
	/*=======TODO=======*/
	mkfifo(

	/*=======TODO=======*/
	return NULL;
	}

int main()
{
	pthread_create(&t1, NULL, recv, NULL);
	pthread_create(&t2, NULL, send, NULL);
	pthread_mutex_lock(&m);
	while (!any_exit)
		pthread_cond_wait(&c, &m);
	pthread_mutex_unlock(&m);
	pthread_cancel(t1);
	pthread_cancel(t2);
	close(recv_fd);
	remove(RECV_PATH);
	close(send_fd);
	remove(SEND_PATH);
	return 0;
}
