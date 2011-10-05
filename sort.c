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
FILE *myLog;
char readbuffer[80];

/*
To compile:	gcc sort.c -Wall -lm -g -lpthread
To run:		./a.out file1 file2 file3 file4 ... fileN
*/

int main(int argc, char* argv[]){
	
	/* Create a Log */
	myLog = fopen("log.txt","w+");

	/* Create the pipes.  */
	if (pipe(pipeRight)){
		printf ("Pipe Right failed to create \n");
		fprintf(myLog, "Pipe Right failed to create \n");
		exit(1);
	}

	/* Create the pipes.  */
	if (pipe (pipeLeft)){
		printf ("Pipe Left failed to create \n");
		fprintf(myLog, "Pipe Left failed to create \n");
		exit(1);
	}

	printf("\n-------------------------------------------------\n");
	fprintf(myLog, "\n--------------------------------------------------------\n");
	printf("Files that need to be sorted: ");
	fprintf(myLog, "Files that need to be sorted: ");
	
	fclose(myLog);

	int i;
	//Prints the files given in the arguments
	for(i = 1; i < argc; i++ )
	{
		printf( "%s ", argv[i] );
		files[i]=argv[i];
		myLog = fopen("log.txt", "a+");
		fprintf(myLog, "%s ", files[i]);
		fclose(myLog);
	}
	printf("\n");
	myLog = fopen("log.txt", "a+");
	fprintf(myLog, "\n\n	PROCESS TREE	\n");
	fclose(myLog);

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
		myLog = fopen("log.txt", "a+");
		fprintf(myLog, "*** ERROR: forking child process failed\n");
		fclose(myLog);
	        exit(1);
	}

	else if(pid>0) {                                  /* for the parent:      */
		printf("Parent: my pid = %d, parent pid = %d \n", getpid(), getppid());
		myLog = fopen("log.txt", "a+");
		fprintf(myLog, "Parent: my pid = %d, parent pid = %d \n", getpid(), getppid());
		fclose(myLog);

	/*Left Side*/
		if( (pid=fork()) ==0){
			printf("	Child: my pid = %d, parent pid = %d \n", getpid(), getppid());
			myLog = fopen("log.txt", "a+");
			fprintf(myLog, "	Child: my pid = %d, parent pid = %d \n", getpid(), getppid());
			fclose(myLog);

			//close(pipeLeft[1]);
			//read_pipe(pipeLeft[0],getpid());

			for(j=0; j<2; j++){	/*Grandchild process*/
				if( (pid=fork()) ==0){
					printf("		Grandchild: my pid = %d, parent pid = %d \n", getpid(), getppid());
					myLog = fopen("log.txt", "a+");
					fprintf(myLog, "		Grandchild: my pid = %d, parent pid = %d \n", getpid(), getppid());
					fclose(myLog);

					//close(pipeLeft[0]);
					sortFile(getpid(), files[j+1]);				
					//exit(0);
				}//end of if		
			}//end of for	

		}//end of if
	}

	/*Right Side*/
	else if (pid == 0) {          /* for the child process:         */
		printf("	Child: my pid = %d, parent pid = %d \n", getpid(), getppid());
		myLog = fopen("log.txt", "a+");
		fprintf(myLog, "	Child: my pid = %d, parent pid = %d \n", getpid(), getppid());
		fclose(myLog);
		//write_pipe(pipeLeft[1], getpid());				

		for(j=0; j<2; j++){	/*Grandchild process*/
			if( (pid=fork()) ==0){
				printf("		Grandchild: my pid = %d, parent pid = %d \n", getpid(), getppid());
				myLog = fopen("log.txt", "a+");
				fprintf(myLog, "		Grandchild: my pid = %d, parent pid = %d \n", getpid(), getppid());
				fclose(myLog);

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
	myLog = fopen("log.txt", "a+");
	fprintf(myLog, "*** %s being read by %d \n", f, p);
	fclose(myLog);

	pFile = fopen(f,"r");
	if (pFile == NULL) {
		printf("File %s doesn't exist\n",f);
		myLog = fopen("log.txt", "a+");
		fprintf(myLog, "File %s doesn't exist\n",f);
		fclose(myLog);
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

	printf("Unsorted %s : %s	",f, buffer);
	myLog = fopen("log.txt", "a+");
	fprintf(myLog, "Unsorted %s : %s", f, buffer);
	fclose(myLog);

	//qsort(buffer, sizeBuffer, sizeof(int), compare);
	//printf("Sorted buffer: %s",buffer);
	//int n;
	//for (n=0; n<sizeBuffer; n++)
     	//	printf ( "%d ",buffer[n]);	

	printf("\n");
	myLog = fopen("log.txt", "a+");
	fprintf(myLog, "\n");
	fclose(myLog);

	//write_pipe(pipeLeft[WRITE], getpid());	
	//read_pipe(pipeLeft[READ], getpid());

	exit(0);
}

/*Write a value to pipe*/
void write_pipe(int f, int p){
	printf("$$$ %d in write_pipe function $$$\n", p);
	myLog = fopen("log.txt", "a+");
	fprintf(myLog, "$$$ %d in write_pipe function $$$\n", p);
	fclose(myLog);

	close(pipeLeft[0]);
	char string[] = "hi everyone! \n";	

	write(f, string, (strlen(string)+1));

	printf("$$$ %d exit write_pipe function $$$\n", p);
	myLog = fopen("log.txt", "a+");
	fprintf(myLog, "$$$ %d exit write_pipe function $$$\n", p);
	fclose(myLog);
}

/*Read a value from pipe*/
void read_pipe(int f, int p){
	printf("$$$ %d in read_pipe function $$$\n", p);
	myLog = fopen("log.txt", "a+");
	fprintf(myLog, "$$$ %d in read_pipe function $$$\n", p);
	fclose(myLog);

	close(pipeLeft[1]);
	int nbytes;

	nbytes = read(pipeLeft[0], readbuffer, sizeof(readbuffer));
	printf("$$$ nbytes = %d \n", nbytes);
	myLog = fopen("log.txt", "a+");
	fprintf(myLog, "$$$ nbytes = %d \n", nbytes);
	fclose(myLog);

	if(nbytes == -1){
		perror ("$$$ Nbytes Error ");
		myLog = fopen("log.txt", "a+");
		fprintf(myLog, "$$$ Nbytes ERROR");
		fclose(myLog);
	}
	else{
        	printf("$$$ Received string: %s \n", readbuffer);
		myLog = fopen("log.txt", "a+");
		fprintf(myLog, "$$$ Received string: %s \n", readbuffer);
		fclose(myLog);		
	}

	printf("$$$ %d exit read_pipe function $$$\n", p);
	myLog = fopen("log.txt", "a+");
	fprintf(myLog, "$$$ %d exit read_pipe function $$$\n", p);
	fclose(myLog);
}


