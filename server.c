#include <pthread.h> 
#include <stdio.h> 
#include <errno.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <signal.h>
#include <string.h>

void* dis_code(void* arg);		//prototype for function handling dispatcher
void* worker_code(void* arg);	//prototype for function handling worker
int input;						//input value entered for file time
int request;					//total number of requests made
int totalTime;					//total time for request

int main(){

	pthread_t dispatcher;	// Dispatcher 
	int status;         		// captures any error code

	/*Signal Handler*/
	void sigHandler (int);
	signal (SIGINT, sigHandler);
	
	// create and start a dispatcher thread executing the "dis_code()" function 
	if ((status = pthread_create (&dispatcher, NULL,  dis_code, NULL)) != 0) { 
	        fprintf (stderr, "thread create error %d: %s\n", status, strerror(status)); 
	        exit (1); 
	}

	//wait for dispatcher to finish
	if ((status = pthread_join (dispatcher, NULL)) != 0) { 
	        fprintf (stderr, "thread join error %d: %s\n", status, strerror(status)); 
	        exit (1); 
	} 

	return 0;
}


/*Dispatcher code goes here*/
void* dis_code(void* arg){

	pthread_t worker;		// Worker 
	int status;         		// captures any error code

	printf("***Dispoatcher starting\n");

	while(1){
		printf("Please input an integer value: \n");
		scanf("%d",&input);
		printf("Number entered: %d \n", input);

		// create and start a child thread executing the "worker_code()" function 
		if ((status = pthread_create (&worker, NULL,  worker_code, &input)) != 0) { 
	        	fprintf (stderr, "thread create error %d: %s\n", status, strerror(status)); 
	        	exit (1); 
		}
/*
		//wait for worker to finish
		if ((status = pthread_join (worker, NULL)) != 0) { 
		        fprintf (stderr, "thread join error %d: %s\n", status, strerror(status)); 
		        exit (1); 
		}
*/	 
	}

	return 0;
}

/*Worker code goes here*/
void* worker_code(void* arg){
	printf("\n		***Worker starting\n");
	int* sTime = (int *)arg; 	//Time to simulate file access
	totalTime = totalTime + *sTime;	
	request = request + 1;
	printf("		File Access (ETA %d seconds) \n", *sTime);
	sleep(*sTime);
	printf("		Done \n");
				
	return 0;
}

/*Signal Handler*/
void sigHandler (int sigNum)
{
	if(sigNum==SIGINT){
		printf("    Received SIGINT signal\n");
		printf("\nTotal Number of file requests: %d \n", request);
		printf("Total file access time: %d \n", totalTime);
		printf("Average file access time: %d \n", (totalTime/request));
		kill(getpid(), SIGTERM);
	}
	else{
		printf("\n***You shouldn't be here!***\n");
	}
}
