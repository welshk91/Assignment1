#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>

int i, n;
double logs;
pid_t pid;

FILE *file_ptr;
char buffer[256];

void readFile(char a[]);

int main(int argc, char *argv[])
{
    n = 1;

    logs = (int)(log2(argc));           /* get the number of forks needed to acccomodate the log2(files) */

    printf("LOGS: %lf\n", logs);

    for (i=0; i < logs; i++)
        if (fork()==0)                  /* this is where a child process runs */
        {

            printf("CHILD pid %d PARENT ppid %d \n", getpid(),getppid());
            if(i == (logs - 1))         /* this is when there the leaves are created */
            {
                //printf("%d LEAF!\n", getpid());
                readFile(argv[n]);
                n++;
            }
        }
        else if( pid > 0)
        {
            printf("CHILD pid %d PARENT ppid %d \n", getpid(),getppid());
        }
        else{

            if(i == (logs - 1))         /* Another spot where leaves are created */
            {
                //printf("%d LEAF!\n", getpid());
                readFile(argv[n]);
                n++;
            }
        }


    return 0;
}

void readFile(char a[])
{

    file_ptr = fopen(a, "r");
    while (!feof(file_ptr))
    {
        fgets(buffer,256,file_ptr);
        printf("%s",buffer);
    }
    printf("\n\n\n");

}

