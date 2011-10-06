/*
Kevin Welsh
Operating Systems
October 6, 2011
Programing Assignment One - Process Tree Sort
*/

/*
To compile:	gcc sort.c -Wall -lm -g -lpthread
To run:		./a.out file1 file2 file3 file4
*/

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

/*Function Prototypes*/
void spawn();
void sortFile(int p, char f[]);
void read_pipe(int f, int p);
void write_pipe(int f, int p, char b[]);

/*Global Variables*/
char* files[] = {};
int pipeLeft[2] = {0};
int pipeRight[2] = {0};
int pipeUp[2] = {0};
FILE *myLog;
char readbuffer[80];

/*Main function*/
int main(int argc, char* argv[]){
	
	/* Create a Log */
	myLog = fopen("log.txt","w+");

	/* Create the right pipe.  */
	if (pipe(pipeRight)){
		printf ("Pipe Right failed to create \n");
		fprintf(myLog, "Pipe Right failed to create \n");
		exit(1);
	}

	/* Create the left pipe.  */
	if (pipe (pipeLeft)){
		printf ("Pipe Left failed to create \n");
		fprintf(myLog, "Pipe Left failed to create \n");
		exit(1);
	}

	/* Create the up pipe.  */
	if (pipe(pipeUp)){
		printf ("Pipe Up failed to create \n");
		fprintf(myLog, "Pipe Right failed to create \n");
		exit(1);
	}

	printf("\n-------------------------------------------------\n");
	fprintf(myLog, "\n--------------------------------------------------------\n");
	printf("Files that need to be sorted: ");
	fprintf(myLog, "Files that need to be sorted: ");
	
	fclose(myLog);

	
	/*Prints the files given in the arguments*/
	int i;
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
	int    status;
	int j;

	pid = fork();

	/*Error*/
	if (pid < 0) {		
	        printf("*** ERROR: forking child process failed\n");
		myLog = fopen("log.txt", "a+");
		fprintf(myLog, "*** ERROR: forking child process failed\n");
		fclose(myLog);
	        exit(1);
	}

	/* Parent*/
	else if(pid>0) {                                  
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

			/*Grandchild process*/
			for(j=0; j<2; j++){	
				if( (pid=fork()) ==0){
					printf("		Grandchild: my pid = %d, parent pid = %d \n", getpid(), getppid());
					myLog = fopen("log.txt", "a+");
					fprintf(myLog, "		Grandchild: my pid = %d, parent pid = %d \n", getpid(), getppid());
					fclose(myLog);

					/*Pipes for Left Grandchildren*/
					//dup2(pipeUp[WRITE], 1);	//redirect standard output to pipeUp[WRITE]
					
					/*First Left Grandkid*/					
					if(j==0){					
						dup2(pipeLeft[WRITE], pipeUp[WRITE]);					
					}
					/*Second Left Grandkid*/
					else{
						dup2(pipeRight[WRITE], pipeUp[WRITE]);
					}

					close(pipeUp[READ]);
					close(pipeLeft[READ]);
					close(pipeLeft[WRITE]);
					close(pipeRight[READ]);
					close(pipeRight[WRITE]);

					sortFile(getpid(), files[j+1]);				
					
				}//end of if
		
			}//end of for - Grandchildren	

		/*Children Wait for Left GrandChildren*/
		
		/*Pipes for left child*/
		dup2(pipeLeft[WRITE], pipeUp[WRITE]);		

		close(pipeUp[READ]);
		close(pipeLeft[WRITE]);
		close(pipeRight[WRITE]);
		waitpid(0,&status,0);
		waitpid(0,&status,0);
		
		read_pipe(pipeLeft[READ], getpid());
		read_pipe(pipeRight[READ], getpid());
		}//end of Left Child
		
		/*Parent Waits for Children*/

		/*Pipes for Parent*/
		//dup2(pipeLeft[READ], pipeUp[READ]);	//connect left pipe to up read
		//dup2(pipeRight[READ], pipeUp[READ]); 	//connect right pipe to up read

		close(pipeUp[WRITE]);
		close(pipeUp[READ]);
		close(pipeLeft[WRITE]);
		close(pipeRight[WRITE]);
		waitpid(0,&status,0);
		waitpid(0,&status,0);
		
		read_pipe(pipeLeft[READ], getpid());
		read_pipe(pipeRight[READ], getpid());	

	}//end of Parent

	/*Right Side*/
	else if (pid == 0) {          /* for the child process:         */
		printf("	Child: my pid = %d, parent pid = %d \n", getpid(), getppid());
		myLog = fopen("log.txt", "a+");
		fprintf(myLog, "	Child: my pid = %d, parent pid = %d \n", getpid(), getppid());
		fclose(myLog);				

		/*Grandchild process*/
		for(j=0; j<2; j++){	
			if( (pid=fork()) ==0){
				printf("		Grandchild: my pid = %d, parent pid = %d \n", getpid(), getppid());
				myLog = fopen("log.txt", "a+");
				fprintf(myLog, "		Grandchild: my pid = %d, parent pid = %d \n", getpid(), getppid());
				fclose(myLog);

				/*Pipes for Right Grandchildren*/
				//dup2(pipeUp[WRITE], 1);	//redirect standard output to pipeUp[WRITE]
				
				/*First Right Grandkid*/					
				if(j==0){					
					dup2(pipeLeft[WRITE], pipeUp[WRITE]);					
				}
				/*Second Right Grandkid*/
				else{
					dup2(pipeRight[WRITE], pipeUp[WRITE]);
				}

				close(pipeUp[READ]);
				close(pipeLeft[READ]);
				close(pipeLeft[WRITE]);
				close(pipeRight[READ]);
				close(pipeRight[WRITE]);

				sortFile(getpid(), files[j+3]);

			}//end of if	
	
		}//end of for Grandchildren

		/*Children Wait for Right GrandChildren*/
		
		/*Pipes for Right Child*/
		dup2(pipeRight[WRITE], pipeUp[WRITE]);

		close(pipeUp[READ]);
		close(pipeLeft[WRITE]);
		close(pipeRight[WRITE]);

		waitpid(0,&status,0);
		waitpid(0,&status,0);

		read_pipe(pipeLeft[READ], getpid());
		read_pipe(pipeRight[READ], getpid());
		
	}//end of Right Child

}//end of spawn

/*Function used to compare in qsort()*/
int compare (const void * a, const void * b)
{
/*
  if ((strcmp(a, b))==0)
    return 0;
  else
    if (*((char*)a)< *((char*)b))
        return -1;
     else
      return 1;
*/
	return (strcmp(a, b));
}

/*Function that opens a file, reads, and sorts it*/
void sortFile(int p, char f[]){

	FILE *pFile;
	char buffer[256];

	//printf( "*** %s being read by %d \n", f, p);
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

	write_pipe(pipeUp[WRITE], getpid(), buffer);	

	exit(0);
}

/*Write a value to pipe*/
void write_pipe(int f, int p, char b []){
	//printf("$$$ %d in write_pipe function $$$\n", p);
	myLog = fopen("log.txt", "a+");
	fprintf(myLog, "$$$ %d in write_pipe function $$$\n", p);
	fclose(myLog);	

	write(f, b, (strlen(b)));
	myLog = fopen("log.txt", "a+");
	fprintf(myLog, "$$$ %d wrote %s to pipe $$$\n", p, b);
	fclose(myLog);

	//printf("$$$ %d exit write_pipe function $$$\n", p);
	myLog = fopen("log.txt", "a+");
	fprintf(myLog, "$$$ %d exit write_pipe function $$$\n", p);
	fclose(myLog);
}

/*Read a value from pipe*/
void read_pipe(int f, int p){
	//printf("$$$ %d in read_pipe function $$$\n", p);
	myLog = fopen("log.txt", "a+");
	fprintf(myLog, "$$$ %d in read_pipe function $$$\n", p);
	fclose(myLog);

	close(pipeLeft[1]);
	int nbytes;

	nbytes = read(f, readbuffer, sizeof(readbuffer));
	myLog = fopen("log.txt", "a+");
	fprintf(myLog, "$$$ %d read %s from pipe $$$\n", p, readbuffer);
	fclose(myLog);

	//printf("$$$ nbytes = %d \n", read(pipeLeft[0], readbuffer, sizeof(readbuffer)));
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

	//printf("$$$ %d exit read_pipe function $$$\n", p);
	myLog = fopen("log.txt", "a+");
	fprintf(myLog, "$$$ %d exit read_pipe function $$$\n", p);
	fclose(myLog);
}


