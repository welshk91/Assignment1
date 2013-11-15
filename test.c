#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define READ 0 
#define WRITE 1 

int pipeLeft[2];
/* Read characters from the pipe and echo them to stdout.   */

/*Write a value to pipe*/
void write_to_pipe(int f, int p){
	close(pipeLeft[0]);
	printf("$$$ %d in write_pipe function $$$\n", p);

	FILE *streamWrite = {0};	
	streamWrite = fdopen (pipeLeft[WRITE], "w");
	if(streamWrite==NULL){
		perror ("$$$ Stream Error ");
	}

	else{
		fprintf (streamWrite, "hello, world from %d! \n", p);
		fclose (streamWrite);
	}

	printf("$$$ %d exit write_pipe function $$$\n", p);
}

/*Read a value from pipe*/
void read_from_pipe(int f, int p){
	close(pipeLeft[1]);
	printf("$$$ %d in read_pipe function $$$\n", p);

	FILE *streamRead = {0};
	int c;
	streamRead = fdopen (f, "r");
	if(streamRead==NULL){
		perror ("$$$ Stream Error ");
	}
	else{
		while ((c = fgetc (streamRead)) != EOF){
			putchar (c);
		}
		fclose (streamRead);
	}

	printf("$$$ %d exit read_pipe function $$$\n", p);
}


int
main (void)
{
  pid_t pid;


  /* Create the pipe.  */
  if (pipe (pipeLeft))
    {
      fprintf (stderr, "Pipe failed.\n");
      return EXIT_FAILURE;
    }

  /* Create the child process.  */
  pid = fork ();
  if (pid == (pid_t) 0)
    {
      /* This is the child process.  */
      read_from_pipe (pipeLeft[0], getpid());
      return EXIT_SUCCESS;
    }
  else if (pid < (pid_t) 0)
    {
      /* The fork failed.  */
      fprintf (stderr, "Fork failed.\n");
      return EXIT_FAILURE;
    }
  else
    {
      /* This is the parent process.  */
      write_to_pipe (pipeLeft[1], getpid());
      return EXIT_SUCCESS;
    }
}


