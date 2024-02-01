#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_SIZE 499

int BP = ARRAY_SIZE;
int SP = ARRAY_SIZE + 1;
int PC = 0;

typedef struct myStruct {
  int OP; // Operation code
  int L;  // Lexicographical level
  int M;  // Modifier
} myStruct;

typedef struct stack {
  int *stack;
  int top;
} stack;

int pas[ARRAY_SIZE];

// Function to calculate the base address of the activation record
int base(int BP, int L) { // Find base L levels down
  int arb = BP; // arb = activation record base
  while (L > 0) // find base L levels down
  {
    arb = pas[arb];
    L--;
  }
  return arb;
}

int main(int argc, char *argv[]) {
  int i = 0, instrCount = 0;
  myStruct IR;

  for (i = 0; i < ARRAY_SIZE; i++) {
    pas[i] = 0;
  }
  FILE *inFile;

  // Check if the input file is specified
  if (argc != 2) {
    printf("Usage: %s<input.txt>\n", argv[0]);
    return 1; // Exit the program if the input file is not specified
  }
  inFile = fopen(argv[1], "r");
  // At this point, argv[1] contains the name of the input file
  printf("Reading from file: %s\n", argv[1]);

  // Read instructions from the file & write to pas array
  while (fscanf(inFile, "%d", &pas[instrCount]) == 1) {
    instrCount++;
  }

  for (int j = 0; j < instrCount; j++) {
    if (j % 3 == 0) {
      printf("\n");
    }
    printf("(%d) ", pas[j]);
  }

  // Check if the file is empty
  if (instrCount != 0) {
    instrCount /= 3;
  } else {
    printf("Error: File is empty.\n");
    return 1;
  }

  printf("\n");

  printf("\t\t\tPC\tBP\tSP\tstack\n");
  printf("Initial values:\t\t0\t499\t500\n\n");

  int flag = 0;
  char *opCode;
  while (flag == 0) // SYS M=3 (HALT).. doesnt cover all cases
  {
      IR.OP = pas[PC];
      IR.L = pas[PC + 1];
      IR.M  = pas[PC + 2];
      PC = PC + 3;
	  opCode = "WIP";

    switch (IR.OP) {
    case 1:           // LIT (literal)
      SP = SP - 1;
      pas[SP] = IR.M; // pushes value M onto the stack
	  opCode = "LIT";
      break;
    case 2: // OPR (return/operator)(will do later)
      SP = BP + 1;
      BP = pas[SP - 2];
      PC = pas[SP - 3];
      opCode = "OPR";
      break;
    case 3: // LOD (load)
      SP = SP - 1;
      pas[SP] = pas[base(BP, IR.L) - IR.M]; // review
	  opCode = "LOD";
      break;
    case 4: // STO (store)
      pas[base(BP, IR.L) - IR.M] =
          pas[SP]; // loads the value in the stack location at offset M from the
                   // stack pointer (SP) into the data memory
      SP = SP + 1; // increment stack pointer, one less AR
	  opCode = "STO";
      break;
    case 5:                         // CAL (Call procedure)
      pas[SP - 1] = base(BP, IR.L); // static link
      pas[SP - 2] = BP;             // dynamic link
      pas[SP - 3] = PC;             // return address
      BP = SP - 1;
      PC = IR.M;
      opCode = "CAL";
      break;
    case 6: // INC (Allocates m slots on the stack)
      SP = SP - IR.M;
      opCode = "INC";
      break;
    case 7: // JMP (Jumps to a new instruction)
      PC = IR.M;
      opCode = "JMP";
      break;
    case 8: // JPC (Jumps conditionally); review
      if (SP == 0) {
        PC = IR.M;
        SP = SP + 1;
      }
      opCode = "JPC";
      break;
    case 9: // SYS
      switch (IR.M) {
      case 1: // WRITE
		SP = SP + 1;
        break;
      case 2: // READ
        printf("Enter an integer: ");
        scanf("%d", &pas[SP]);
        break;
      case 3: // HALT
        flag = 1;
        break;
      }
      break;
    default:
      printf("Error: Invalid operation code.\n");
      fclose(inFile);
      return 1;
    }

    printf("%s\t%d\t%d\t%d\t%d\t%d\t", opCode, IR.L, IR.M, PC, BP, SP);

	// Print stack elements from SP to BP
	for (int i = SP; i < BP; i++) {
	    printf("%d ", pas[i]);
	}

	// Print separator if BP is not equal to SP
	if (BP != SP) {
	    printf("| ");
	}

	// Print stack elements from BP to BP - 3
	for (int i = BP; i > BP - 3; i--) {
	    printf("%d ", pas[i]);
	}

    printf("\n");
  }
}
