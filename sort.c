#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>
#include <pthread.h> 
#include <errno.h> 

void spawn();

/*
To compile:	gcc sort.c -Wall -lm -g -lpthread
To run:		./a.out file1 file2 file3 file4 ... fileN
*/

int main(int argc, const char* argv[]){

	printf("\n-------------------------------------------------\n");
	printf("Files that need to be sorted: ");

	int i;
	//Prints the files given in the arguments
	for(i = 1; i < argc; i++ )
	{
		printf( "%s ", argv[i] );
	}
	printf("\n");

	/*Create Process tree with forks*/
	spawn();
	
	/*Give time for print statements*/
	sleep(1);

	return 0;

} //end of main

void  spawn()
{
	pid_t  pid;
	//int    status;
	int j;

	pid = fork();

	if (pid < 0) {		/*Error*/
	        printf("*** ERROR: forking child process failed\n");
	        exit(1);
	}

	else if(pid>0) {                                  /* for the parent:      */
		printf("Parent: my pid = %d, parent pid = %d \n", getpid(), getppid());

		if( (pid=fork()) ==0){
			printf("	Child: child pid = %d, my pid = %d, parent pid = %d \n", pid, getpid(), getppid());
			for(j=0; j<2; j++){
				if( (pid=fork()) ==0){
					printf("		Grandchild: child pid = %d, my pid = %d, parent pid = %d \n", pid, getpid(), getppid());
					exit(0);
				}//end of if		
			}//end of for	
		
		}//end of if

		//printf("***Parent Code Here:    \n");
		//while (wait(&status) != pid);       /* wait for completion  */
		//printf("***Parent code ending\n");
	}

	else if (pid == 0) {          /* for the child process:         */
		printf("	Child: child pid = %d, my pid = %d, parent pid = %d \n", pid, getpid(), getppid());

		for(j=0; j<2; j++){
			if( (pid=fork()) ==0){
				printf("		Grandchild: child pid = %d, my pid = %d, parent pid = %d \n", pid, getpid(), getppid());
				exit(0);
			}//end of if		
		}//end of for

	}//end of child


}//end of spawn
