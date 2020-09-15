#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {

    printf("this is a line made in the parent ");
    fflush(stdout);

    int rc = fork();

    if(rc ==0) {//child process
    
        printf("this is a line made in the child\n");
    } else {
        printf("this is another line made in the parent\n");
    }

    exit(0);
}