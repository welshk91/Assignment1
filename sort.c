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

int main(){

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
