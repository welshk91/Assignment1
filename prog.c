#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>

   /*
                   GOALS
   write a parent program that:
           installs signal handler(s) for each of the user-defined signals
           spawns off a child process
           reports when a user-defined signal is raised*
           terminates gracefully upon receiving a Control-C
   the child process should repeatedly:
           wait a random amount of time
           randomly send one of the user-defined signals to its parent
   */

void  spawn()
{
   pid_t  pid;
   int    status;

   pid = fork();

   if (pid < 0) {
            printf("*** ERROR: forking child process failed\n");
            exit(1);
   }
  else if (pid == 0) {          /* for the child process:         */
          printf("***Child code here:   \n");
   
       while(1){
           /*Random waiting time*/
           srand (time(NULL) );
           int t = rand() % 10 + 1;        
           printf("\nSleeping for %d seconds \n", t);        
           sleep(t);
           printf("Woke up after %d seconds \n", t);

           /*Send Random signal*/
           int randomSig = rand() % 2 + 1;
           if(randomSig==1){
               printf("Sending SIGUSR%d \n", randomSig);
               kill (getppid (), SIGUSR1);
           }
           else if(randomSig==2){
               printf("Sending SIGUSR%d \n", randomSig);
               kill (getppid (), SIGUSR2);
           }
           else{
               printf("    ERROR    \n");
               printf("Crap. Random Signal is %d \n", randomSig);
               exit(0);
           }
       }
       printf("***Child code ending\n");
   }
       else {                                  /* for the parent:      */
       printf("***Parent Code Here:    \n");
           while (wait(&status) != pid);       /* wait for completion  */
       printf("***Parent code ending\n");
       }
}
int main(){
   /*Signal Handler*/
   void sigHandler (int);
   signal (SIGUSR1, sigHandler);
   signal (SIGUSR2, sigHandler);
   signal (SIGINT, sigHandler);

   /*Fork*/
   spawn();

   return 0;
}
void sigHandler (int sigNum)
{
   if(sigNum==SIGINT){
       printf("    Received SIGINT signal from %d\n", pid);

//signal(SIGINT, SIG_DFL);
kill(getpid(), SIGTERM);
   }

   else if(sigNum==SIGUSR1){
       printf("    Received SIGUSR1 signal from %d\n", pid);
   }

   else if(sigNum==SIGUSR2){
       printf("    Received SIGUSR2 signal from %d\n", pid);
   }
}
