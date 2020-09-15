#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>

int main( int argc, char* argv[]) {

	//printf("test to see if this prints\n");

	if(argc < 3) {
		printf("not enough arguments for the function.\n");
		exit(1);
	}

	//grab the number of children to create
	int numofChildren = atoi(argv[1]);
	
	// use ceiling function
	// add values to end of array
	int partitionSize = (argc-2)/numofChildren;
	int childID; //used in child proc to pull correct values from array	
	int rc[numofChildren]; //array containing the pids of the children

	//create the pipe	
	int p[numofChildren][2];
	for(int i = 0; i < numofChildren; i++){
		pipe(p[i]);
	}

	//create the array of values
	int inputNumsSize = argc - 2;	
	int *inputNums = (int*) malloc(sizeof(int) * inputNumsSize);

	for(int i = 2; i < argc; i++) {
		inputNums[i - 2] = atoi(argv[i]);
	}
	//print out starting message
	printf("I am parent with pid: %d sending the array: ", (int) getpid());


	for(int j = 0; j < numofChildren; j++) {
		childID = j;		

		int index = childID *partitionSize;

		for(int i = 0; i < partitionSize; i++) {
			printf("%d ", inputNums[index + i]);
		}


		printf("to child with pid:");
		fflush(stdout);

		rc[j] = fork();

		//breaking from loop if process is a child
		if(rc[j] < 0) {
			fprintf(stderr, "fork failed \n");
			exit(1);
		} else if (rc[j] == 0) { //child process
			//printf(" %d (child) \n", getpid());
			break;
		} else {
			printf(" %d", (int) rc[j]);

			//coding for punctuation
			if(j+1 >= numofChildren) {
				printf(".");
			} else if (j + 2 >= numofChildren) {
				printf(", and ");
			} else {
				printf(", ");
			}
		}

		if(j +1 == numofChildren) {
			printf("\n");
		}		

	}

	if(rc[childID] == 0) { //Child Process
		
		//close the reading channed for the pipe
		close(p[childID][0]);
		
		//print out the operation to the shell
		printf("I am child with pid: %d, adding the array: ", (int) getpid());

		//calculate the sum while printing out the array	
		int sum = 0;

		int index = childID * partitionSize;
	
		for(int i = 0; i < partitionSize; i++) {
			printf("%d ", inputNums[index + i]);
			sum+= inputNums[index + i];
		}

		printf("and sending partial sum %d\n", sum);

		//write the sume to the pipe 
		write(p[childID][1],&sum, 4);
		close(p[childID][1]);	
	} else { //parent process
		//close the write channel for the pipe
		for(int i = 0; i < numofChildren; i++) {
			close(p[i][1]);
		}
	
		//wait for the process to finish
		for(int i = 0; i < numofChildren; i++) {
			waitpid(rc[i], NULL, 0);
		}

		printf("I am the parent pid: %d, receiving partial sums ", (int) getpid());
 
		//get the sum from the child process	
		int *sum = malloc(sizeof(int) * numofChildren);
		int totalSum = 0;	
	
		for(int i = 0; i < numofChildren; i++) {
			read(p[i][0], &sum[i], 4);

			printf("%d ", sum[i]);
			totalSum+= sum[i];
		}

		printf("for a total sum of %d\n", totalSum);


		//write the sum back to the shell
	}
		
	exit(0);



















}
