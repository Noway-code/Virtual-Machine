#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_SIZE 499

int BP = ARRAY_SIZE;
int SP = ARRAY_SIZE + 1;
int PC = 0;

// Instruction
typedef struct myStruct {
  int OP;
  int L;
  int M;
} myStruct; // for IR

typedef struct stack {
  int stack[ARRAY_SIZE];
  int top;
} stack;

// Global variable declaration
int pas[ARRAY_SIZE]; // Text and the stack will be in the same ARRAY

int base(int BP, int L) { // Find base L levels down
  int arb = BP; // arb = activation record base
  while (L > 0) // find base L levels down
  {
    arb = pas[arb];
    L--;
  }
  return arb;
}


int main(int argc, char *argv[]) { // impliment command line arguments
  int i = 0, Operation = 0, top = -1, instrCount = 0;
  char string[256];
  myStruct IR;

  for (i = 0; i < ARRAY_SIZE; i++) {
    pas[i] = 0;
  }

  FILE *inFile;

  if (argc != 2) {
    printf("Usage: %s<input.txt>\n", argv[0]);
    return 1; // Exit the program if the input file is not specified
  }
  inFile = fopen(argv[1], "r");
  // At this point, argv[1] contains the name of the input file
  printf("Reading from file: %s\n", argv[1]);
  
  while (fscanf(inFile, "%d", &pas[instrCount]) == 1) {
    instrCount++;
  }

  // for testing outputs
  for (int j = 0; j < instrCount; j++) { 
    if (j % 3 == 0) {
      printf("\n");
    }
    printf("(%d) ", pas[j]);
  }
  
  if (instrCount != 0) {
    instrCount /= 3;
  } else {
    printf("Error: File is empty.\n");
    return 1;
  }
  printf("\n");
  printf("Initial values: \n");

  
  printf("				        PC	BP	  SP	  stack\n");
  printf("Initial values:	0	  499	  500\n");
  
  int flag = 0; // for exiting while loop
  while (flag == 0) // SYS M=3 (HALT).. doesnt cover all cases
  {
      IR.OP = pas[PC];
      IR.L = pas[PC + 1];
      IR.M  = pas[PC + 2];
      PC = PC + 3; 

    switch (Operation) {
    case 1:           // LIT (literal)
      SP = SP - 1;    // decrement stack pointer
      pas[SP] = IR.M; // pushes value M onto the stack
      break;
    case 2: // OPR (return/operator)(will do later)
      SP = BP + 1;
      BP = pas[SP - 2];
      PC = pas[SP - 3];
      break;
    case 3: // LOD (load)
      SP = SP - 1;
      pas[SP] = pas[base(BP, IR.L) - IR.M]; // review
      break;
    case 4: // STO (store)
      pas[base(BP, IR.L) - IR.M] =
          pas[SP]; // loads the value in the stack location at offset M from the
                   // stack pointer (SP) into the data memory
      SP = SP + 1; // increment stack pointer, one less AR
      break;
    case 5:                         // CAL (Call procedure)
      pas[SP - 1] = base(BP, IR.L); // static link
      pas[SP - 2] = BP;             // dynamic link
      pas[SP - 3] = PC;             // return address
      BP = SP - 1;
      PC = IR.M;
      break;
    case 6: // INC (Allocates m slots on the stack)
      SP = SP - IR.M;
      break;
    case 7: // JMP (Jumps to a new instruction)
      PC = IR.M;
      break;
    case 8: // JPC (Jumps conditionally); review
      if (SP == 0) {
        PC = IR.M;
        SP = SP + 1;
      }
      break;
    case 9: // SYS
      switch (IR.M) {
      case 1: // WRITE
        printf("Top of Stack Value: %d\n", pas[SP]);
        break;
      case 2: // READ
        printf("Enter an integer");
        scanf("%d", &pas[SP]);
        break;
      case 3: // HALT
        // output for stack 
        flag = 1;
        break;
      }
      break;
    default:
      printf("Error: Invalid operation code.\n");
      fclose(inFile);
      return 1;
    }
  }
} // Code looks good
