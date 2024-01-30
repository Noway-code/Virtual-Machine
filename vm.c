#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ARRAY_SIZE = 500;

int BP = 499; 
int SP = 500; 
int PC = 0; 

// Instruction
struct mystruct 
{
  int Op;
  int L;
  int M;
}

  


// int base( int BP, int L)
// {
//   int pas[100]; // erase later
//   int Sp[ARRAY_SIZE];
//   int arb = BP;	// arb = activation record base
//   while ( L > 0)     //find base L levels down
//   {
//     arb = pas[arb];
//     L--;
//   }
//   return arb;
// }


int main(int argc, char*argv[]) { // impliment command line arguments
  int pas[ARRAY_SIZE], i=0; 
  int 
  int top = -1;
  int instrCount = 0;
  
  FILE *inFile, *outFile;
  
  if (argc != 2) {
    printf("Usage: %s<input.txt>\n", argv[0]);
    return 1; // Exit the program if the input file is not specified
  }

  // At this point, argv[1] contains the name of the input file
  printf("Reading from file: %s\n", argv[1]);
  // Here, you would add your file reading and processing logic
  inFile = fopen(argv[1], "r");
  while (fscanf(inFile, "%d", &pas[instrCount]) == 1) {
    instrCount++;
  }

  for(int j = 0; j < instrCount; j++)
    {
      if(j % 3 == 0)
      {
       printf("\n");
      }
      printf("(%d) ", pas[j]);
    }
  if(instrCount != 0)
  {
    instrCount /= 3;
  }
  else
  {
    printf("Error: File is empty.\n");
    return 1;
  }
  printf("\n");
  printf( "Initial values: \n"); 
  
}