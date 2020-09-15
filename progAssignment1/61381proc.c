#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main( int argc, char* argv[]) {

	//grab the number of children to create
	
	int numofChildren = atoi(argv[1]);
	
	//create the pipe	
	int p[2];
	pipe(p);

	//create the array of values
	
	int inputNumsSize = argc - 2;	
	int *inputNums = (int*) malloc(sizeof(int) * inputNumsSize);

	for(int i = 2; i < argc; i++) {
		inputNums[i - 2] = atoi(argv[i]);
	}

	//print out starting message
	
	printf("I am parent with pid: %d sending the array: ", (int) getpid());

	for(int i = 0; i < inputNumsSize; i++) {
		printf("%d ", inputNums[i]);
	}
	
	printf("\n");

	//create fork of parent

	int rc = fork();

	if(rc < 0 ) {
		fprintf(stderr, "fork failed \n");
		exit(1);	
	} else if(rc == 0) { //Child Process
		
		//close the reading channed for the pipe

		close(p[0]);
		
		//print out the operation to the shell
		printf("I am child with pid: %d, adding the array: ", (int) getpid());

		//calculate the sum while printing out the array	
		int sum = 0;
	
		for(int i = 0; i < inputNumsSize; i++) {
			printf("%d ", inputNums[i]);
			sum+= inputNums[i];
		}

		printf("and sending partial sum %d\n", sum);

		//write the sume to the pipe 
		write(p[1],&sum, 4);
		close(p[1]);	
	} else { //parent process
		//close the write channel for the pipe
		close(p[1]);
	
		//wait for the process to finish
		wait(NULL);	
	
		//get the sum from the child process	
		int sum;
		read(p[0], &sum, 4);

		//write the sum back to the shell
		printf("I am the parent pid: %d, receiving partial sums %d and printing %d\n", (int) getpid(), sum, sum); 
	}
		
	exit(0);



















}
