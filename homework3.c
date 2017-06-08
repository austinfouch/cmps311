#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
**  Author:   Austin Fouch
**  Date:     02/13/2017
**  Assgnmt:  Homework #3
**
**  Creates and fills a file with fibo numbers (amount based on user input)
**  Computation and printing are done in seperate child processes
*/
#define READ_END 0
#define WRITE_END 1
int pfd[2];         // pipe arr
int pid_comp = -1;  // computation
int pid_print = -1; // printing

int main() {

  int input = 0;          // # of fibo numbers to print
  int counter = 1;        // current element of fibo seq
  int output;
  FILE *outFile;          // file output
  char filename[50];
  int first = 0, second = 1, next, i; // for fibo computation

  // create pipe
  pipe(pfd);

  //
  while(pid_print != 0 && pid_comp != 0) {

    printf("\nEnter an integer between 1 and 50: ");
    scanf("%d", &input);
    wait(NULL);

    if(input == -1) {
      printf("\nExiting...\n");
      return(0);
    } else if(input > 50) {

      printf("\nInvalid input\n");
      break;
    }
    // create child processes
    if(pid_comp != 0 && pid_print != 0) {
      pid_comp = fork();  //child1
    }
    if(pid_comp != 0 && pid_print != 0) {
      pid_print = fork(); //child2
    } else {
      printf("\nThe first %d fibonacci numbers were written to fib-%d.txt", input, input);
    }
    // change file name and open
    sprintf(filename, "fib-%d.txt", input);
    outFile = fopen(filename,"w+");
    wait(NULL);
  }

  if(pid_comp == 0) {  // child1
    // do fibo computation
    for(i = 1; i < input + 1; i++) {

      if(i <= 1) {
        next = 1;
      } else {
        next = first + second;
        first = second;
        second = next;
      }
      // send to pipe
      write(pfd[WRITE_END], &next, sizeof(int));
    }
  } else if(pid_print == 0) {  // child2
    // set file name
    fprintf(outFile, "%s\n", filename);
    for(i = 0; i < input; i++) {
      // get from pipe
      read(pfd[READ_END], &next, sizeof(int));
      fprintf(outFile, "%d: %d\n", counter, next);
      counter += 1;
      wait(NULL);
    }
  }
  fclose(outFile);
  close(pfd);
  return 0;
}
