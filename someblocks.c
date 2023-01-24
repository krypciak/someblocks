#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#ifdef __OpenBSD__
#define SIGPLUS			SIGUSR1+1
#define SIGMINUS		SIGUSR1-1
#else
#define SIGPLUS			SIGRTMIN
#define SIGMINUS		SIGRTMIN
#endif
#define LENGTH(X)               (sizeof(X) / sizeof (X[0]))
#define CMDLENGTH		50
#define MIN( a, b ) ( ( a < b) ? a : b )
#define STATUSLENGTH (LENGTH(blocks) * CMDLENGTH + 1)

typedef struct {
	char* icon;
	char* command;
	float interval;
	unsigned int signal;
} Block;

typedef struct {
    Block *block;
    unsigned int i;
} ThreadData;

#ifndef __OpenBSD__
void dummysighandler(int num);
#endif
void sighandler(int num);
void getsigcmds(unsigned int signal);
void setupsignals();
void sighandler(int signum);
int getstatus(char *str, char *last);
void statusloop();
void termhandler();
void pstdout();
void psomebar();
static void (*writestatus) () = psomebar;

void *threadblock(void *data);

#include "blocks.h"

static char statusbar[LENGTH(blocks)][CMDLENGTH] = {0};
static char statusstr[2][STATUSLENGTH];
static int statusContinue = 1;
static int returnStatus = 0;
static char somebarPath[128];
static int somebarFd = -1;
// block threads
static pthread_t threads[LENGTH(blocks)];
static pthread_mutex_t lock;

//opens process *cmd and stores output in *output
void getcmd(const Block *block, char *output)
{
    char *output1 = malloc(sizeof(char) * CMDLENGTH);

	strcpy(output1, block->icon);
	FILE *cmdf = popen(block->command, "r");
	if (!cmdf)
		return;
	int i = strlen(block->icon);
	fgets(output1+i, CMDLENGTH-i-delimLen, cmdf);
	i = strlen(output1);
	if (i == 0) {
		//return if block and command output are both empty
		pclose(cmdf);
		return;
	}
	if (delim[0] != '\0') {
		//only chop off newline if one is present at the end
		i = output1[i-1] == '\n' ? i-1 : i;
		strncpy(output1+i, delim, delimLen); 
	}
	else
		output1[i++] = '\0';



	pclose(cmdf);

    // lock
    pthread_mutex_lock(&lock);

    strcpy(output, output1);

    // unlock
    pthread_mutex_unlock(&lock);

    free(output1);
}

void startthreads(void)
{
    ThreadData *data = malloc(LENGTH(blocks)*sizeof(ThreadData));
    const Block* current;
	for (unsigned int i = 0; i < LENGTH(blocks); i++) {
		current = blocks + i;
        data[i] = (ThreadData){.block = current, .i = i};
        // start thread
        pthread_create(&threads[i], NULL, threadblock, (void *)&data[i]);
    }
}


// sleep tms miliseconds
int msleep(long tms)
{
    struct timespec ts;
    int ret;

    if (tms < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = tms / 1000;
    ts.tv_nsec = (tms % 1000) * 1000000;

    do {
        ret = nanosleep(&ts, &ts);
    } while (ret && errno == EINTR);

    return ret;
}

void *threadblock(void *data)
{
    ThreadData *data1 = (ThreadData*) data;
    const Block *block = data1->block;
    while(1) {
        getcmd(block, statusbar[data1->i]);

        // lock
        pthread_mutex_lock(&lock);

        writestatus();

        // unlock
        pthread_mutex_unlock(&lock);

        // sleep block->interval in sections of 0.5s
        // in case someblocks gets killed
        unsigned int sleep = block->interval * 1000;
        for(int i = 0; i < block->interval * 2; i++) {
            if(statusContinue == 0) break;
            msleep(500);
        }
        if(statusContinue == 0) break;
    }
}

void getsigcmds(unsigned int signal)
{
	const Block *current;
	for (unsigned int i = 0; i < LENGTH(blocks); i++) {
		current = blocks + i;
		if (current->signal == signal)
			getcmd(current,statusbar[i]);
	}
}

void setupsignals()
{
	struct sigaction sa = {0};
#ifndef __OpenBSD__
	/* initialize all real time signals with dummy handler */
	sa.sa_handler = dummysighandler;
	for (int i = SIGRTMIN; i <= SIGRTMAX; i++)
		sigaction(i, &sa, NULL);
#endif

	sa.sa_handler = sighandler;
	for (unsigned int i = 0; i < LENGTH(blocks); i++) {
		if (blocks[i].signal > 0)
			sigaction(SIGMINUS+blocks[i].signal, &sa, NULL);
	}

}

int getstatus(char *str, char *last)
{
	strcpy(last, str);
	str[0] = '\0';

	for (unsigned int i = 0; i < LENGTH(blocks); i++)
		strcat(str, statusbar[i]);

	str[strlen(str)-strlen(delim)] = '\0';
	return strcmp(str, last);//0 if they are the same
}

void pstdout()
{
	if (!getstatus(statusstr[0], statusstr[1]))//Only write out if text has changed.
		return;
	printf("%s\n",statusstr[0]);

	fflush(stdout);
}


void psomebar()
{
	if (!getstatus(statusstr[0], statusstr[1]))//Only write out if text has changed.
		return;

	if (somebarFd < 0) {
		somebarFd = open(somebarPath, O_WRONLY|O_CLOEXEC);
		if (somebarFd < 0 && errno == ENOENT) {
			// assume somebar is not ready yet
			sleep(1);
			somebarFd = open(somebarPath, O_WRONLY|O_CLOEXEC);
		}
		if (somebarFd < 0) {
			perror("open");
			return;
		}

	}
    

	dprintf(somebarFd, "status %s\n", statusstr[0]);
}


void statusloop()
{
	setupsignals();
    
    startthreads();  

    for(int i = 0; i < LENGTH(blocks); i++) {
        pthread_join(threads[i], NULL);
    }
}

#ifndef __OpenBSD__
/* this signal handler should do nothing */
void dummysighandler(int signum)
{
    return;
}
#endif

void sighandler(int signum)
{
	getsigcmds(signum-SIGPLUS);
	writestatus();
}

void termhandler()
{
	statusContinue = 0;
}

void sigpipehandler()
{
	close(somebarFd);
	somebarFd = -1;
}

int main(int argc, char** argv)
{
	for (int i = 0; i < argc; i++) {//Handle command line arguments
		if (!strcmp("-d",argv[i]))
			strncpy(delim, argv[++i], delimLen);
		else if (!strcmp("-p",argv[i]))
			writestatus = pstdout;
		else if (!strcmp("-s",argv[i]))
			strcpy(somebarPath, argv[++i]);
	}

	if (!strlen(somebarPath)) {
		strcpy(somebarPath, getenv("XDG_RUNTIME_DIR"));
		strcat(somebarPath, "/somebar-0");
	}

	delimLen = MIN(delimLen, strlen(delim));
	delim[delimLen++] = '\0';
	signal(SIGTERM, termhandler);
	signal(SIGINT, termhandler);
	signal(SIGPIPE, sigpipehandler);
	statusloop();
	return 0;
}
