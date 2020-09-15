#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	
	printf("Welcome! This file is meant to practice my use of args\n\n");
	
	printf("There are %d args given to this function\n", argc);
	printf("They are: \n");
	for(int i = 0; i < argc; i++){
		printf("%s\n", argv[i]);
	}
	exit(0);	

}
