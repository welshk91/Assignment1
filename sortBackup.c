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

#define READ 0 
#define WRITE 1 

void spawn();
void sortFile(int p, char f[]);
char* files[] = {};
void read_pipe(int f, int p);
void write_pipe(int f, int p);
int pipeLeft[2] = {0};
int pipeRight[2] = {0};
//char    readbuffer[80];

/*
To compile:	gcc sort.c -Wall -lm -g -lpthread
To run:		./a.out file1 file2 file3 file4 ... fileN
*/

int main(int argc, char* argv[]){

/*
	// Create the pipes.  //
	if (pipe(pipeRight)){
		printf ("Pipe Right failed to create \n");
		exit(1);
	}
*/	
	/* Create the pipes.  */
	if (pipe (pipeLeft)){
		printf ("Pipe Left failed to create \n");
		exit(1);
	}

	printf("\n-------------------------------------------------\n");
	printf("Files that need to be sorted: ");

	int i;
	//Prints the files given in the arguments
	for(i = 1; i < argc; i++ )
	{
		printf( "%s ", argv[i] );
		files[i]=argv[i];
	}
	printf("\n");

	/*Create Process tree with forks*/
	spawn();
	
	/*Give time for print statements*/
	sleep(1);

	return 0;

} //end of main

/*Function used to build process tree*/
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

	/*Left Side*/
		if( (pid=fork()) ==0){
			printf("	Child: my pid = %d, parent pid = %d \n", getpid(), getppid());
			read_pipe(pipeLeft[0],getpid());

			for(j=0; j<2; j++){	/*Grandchild process*/
				if( (pid=fork()) ==0){
					printf("		Grandchild: my pid = %d, parent pid = %d \n", getpid(), getppid());
					sortFile(getpid(), files[j+1]);				
					//exit(0);
				}//end of if		
			}//end of for	

		}//end of if
	}

	/*Right Side*/
	else if (pid == 0) {          /* for the child process:         */
		printf("	Child: my pid = %d, parent pid = %d \n", getpid(), getppid());
		//write_pipe(pipeLeft[1], getpid());				

		for(j=0; j<2; j++){	/*Grandchild process*/
			if( (pid=fork()) ==0){
				printf("		Grandchild: my pid = %d, parent pid = %d \n", getpid(), getppid());
				sortFile(getpid(), files[j+3]);
				//exit(0);
			}//end of if		
		}//end of for

	}//end of child

}//end of spawn

/*Function used to compare in qsort()*/
int compare (const void * a, const void * b)
{

  if (*((char*)a)==*((char*)b))
    return 0;
  else
    if (*((char*)a)< *((char*)b))
        return -1;
     else
      return 1;
}

/*Function that opens a file, reads, and sorts it*/
void sortFile(int p, char f[]){

	FILE *pFile;
	char buffer[256];

	printf( "*** %s being read by %d \n", f, p);

	pFile = fopen(f,"r");
	if (pFile == NULL) {
		printf("File %s doesn't exist\n",f);
		exit(1);
	}

	while (!feof(pFile)){
        	fgets(buffer,256,pFile);
	}//end of while	

	int sizeBuffer = 0;
	while(buffer[sizeBuffer]!='\0'){	
//		buffer2[sizeBuffer] = atoi (&buffer[sizeBuffer]);		
		sizeBuffer++;
	}

	//printf("Size of buffer: %d \n", sizeBuffer);
	printf("Unsorted buffer: %s	", buffer);
	//qsort(buffer, sizeBuffer, sizeof(int), compare);
	//printf("Sorted buffer: %s",buffer);
	//int n;
	//for (n=0; n<sizeBuffer; n++)
     	//	printf ( "%d ",buffer[n]);	

	printf("\n");

	//write_pipe(pipeLeft[WRITE], getpid());	
	//read_pipe(pipeLeft[READ], getpid());

	exit(0);
}

/*Write a value to pipe*/
void write_pipe(int f, int p){
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
void read_pipe(int f, int p){
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


