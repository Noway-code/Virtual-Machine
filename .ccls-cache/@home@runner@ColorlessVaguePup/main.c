#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ARRAY_SIZE = 500;

int BP = 499; 
int SP = 500; 
int PC = 0; 


int base( int BP, int L)
{
  int pas[100];
  int Sp[ARRAY_SIZE];
  int arb = BP;	// arb = activation record base
  while ( L > 0)     //find base L levels down
  {
    arb = pas[arb];
    L--;
  }
  return arb;
}


int main(int argc, char*argv[]) { // impliment command line arguments
  if(argc > 1)
  {
    printf("Loading file: %s\n", argv[1]);
    fopen("input.txt", "r");
    
  }
  else{
    printf("No file given\n");
  }
  
  printf("Hello World\n");
  return 0;
}