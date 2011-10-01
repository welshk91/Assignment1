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

	/*Forking*/
	spawn();
	return 0;
}

void  spawn()
{
   pid_t  pid;
   int    status;

   pid = fork();

   if (pid < 0) {		/*Error*/
            printf("*** ERROR: forking child process failed\n");
            exit(1);
   }

  else if (pid == 0) {          /* for the child process:         */
       printf("***Child code here:   \n");
	//assign files
	//sort files
	//pipe first value to parent
       printf("***Child code ending\n");
   }

  else {                                  /* for the parent:      */
       printf("***Parent Code Here:    \n");
       while (wait(&status) != pid);       /* wait for completion  */
       printf("***Parent code ending\n");
       }
}
