/*
Contributers
-Jason Laveus
-Camilo Alvarez-Velez

Date
-2/1/2024
*/
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

int pas[ARRAY_SIZE];

// Function to calculate the base address of the activation record
int base(int BP, int L) { // Find base L levels down
  int arb = BP;           // arb = activation record base
  while (L > 0)           // find base L levels down
  {
    arb = pas[arb];
    L--;
  }
  return arb;
}

int main(int argc, char *argv[]) {
  int i = 0, instrCount = 0, calFlag = 0, bpTracker[100], j = 0;
  myStruct IR;

  // sets all values in pas to 0
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

  // (FOR TESTING)
  // for (int j = 0; j < instrCount; j++) {
  //   if (j % 3 == 0) {
  //     printf("\n");
  //   }
  //   printf("(%d) ", pas[j]);
  // }

  // Check if the file is empty
  // if (instrCount != 0) {
  //   instrCount /= 3;
  // } else {
  //   printf("Error: File is empty.\n");
  //   return 1;
  // }

  printf("\n");
  // beginning output
  printf("\t\t\t\t\tPC\tBP\tSP\tstack\n");
  printf("Initial values:\t\t0\t%d\t%d\n\n", BP, SP);

  int halt = 0;
  char opCode[10];

  while (halt == 0) // SYS M=3 (HALT).. doesnt cover all cases
  {

    // TODO: Implement the instruction cycle. Currently output is not correct
    // and skips over some instructions.
    // strcpy(opCode, "");

    // Fetch
    IR.OP = pas[PC];
    IR.L = pas[PC + 1];
    IR.M = pas[PC + 2];
    PC = PC + 3;

    // Execute
    // halt = execute(IR, &opCode);

    switch (IR.OP) {
    case 1: // LIT (literal)
      SP = SP - 1;
      pas[SP] = IR.M; // pushes value M onto the stack
      strcpy(opCode, "LIT");
      break;

    case 2: // OPR (return/operator)(will do later)
      if (IR.M == 0) {
        SP = BP + 1;
        BP = pas[SP - 2];
        PC = pas[SP - 3];
        strcpy(opCode, "RTN");
        break;
      } else if (IR.M == 1) { // ADD
        pas[SP + 1] = pas[SP + 1] + pas[SP];
        SP = SP + 1;
        strcpy(opCode, "ADD");
        break;
      } else if (IR.M == 2) { // SUB
        pas[SP + 1] = pas[SP + 1] - pas[SP];
        SP = SP + 1;
        strcpy(opCode, "SUB");
        break;
      } else if (IR.M == 3) { // MUL
        pas[SP + 1] = pas[SP + 1] * pas[SP];
        SP = SP + 1;
        strcpy(opCode, "MUL");
        break;
      } else if (IR.M == 4) { // DIV
        pas[SP + 1] = pas[SP + 1] / pas[SP];
        SP = SP + 1;
        strcpy(opCode, "DIV");
        break;
      } else if (IR.M == 5) { // EQL
        pas[SP + 1] = pas[SP + 1] == pas[SP];
        SP = SP + 1;
        strcpy(opCode, "EQL");
        break;
      } else if (IR.M == 6) { // NEQ
        pas[SP + 1] = pas[SP + 1] != pas[SP];
        SP = SP + 1;
        strcpy(opCode, "NEQ");
        break;
      } else if (IR.M == 7) { // LSS
        pas[SP + 1] = pas[SP + 1] < pas[SP];
        SP = SP + 1;
        strcpy(opCode, "LSS");
        break;
      } else if (IR.M == 8) { // LEQ
        pas[SP + 1] = pas[SP + 1] <= pas[SP];
        SP = SP + 1;
        strcpy(opCode, "LEQ");
        break;
      } else if (IR.M == 9) { // GTR
        pas[SP + 1] = pas[SP + 1] > pas[SP];
        SP = SP + 1;
        strcpy(opCode, "GTR");
        break;
      } else if (IR.M == 10) { // GEQ
        pas[SP + 1] = pas[SP + 1] >= pas[SP];
        SP = SP + 1;
        strcpy(opCode, "GEQ");
        break;
      } else if (IR.M == 11) { // ODD
        pas[SP + 1] = pas[SP + 1] % 2;
        SP = SP + 1;
        strcpy(opCode, "ODD");
        break;
      } else { // ERROR
        strcpy(opCode, "ERROR");
        break;
      }
    case 3: // LOD (load)
      SP = SP - 1;
      pas[SP] = pas[base(BP, IR.L) - IR.M]; // review
      strcpy(opCode, "LOD");
      break;

    case 4: // STO (store)
      pas[base(BP, IR.L) - IR.M] =
          pas[SP]; // loads the value in the stack location at offset M from the
      // stack pointer (SP) into the data memory
      SP = SP + 1; // increment stack pointer, one less AR
      strcpy(opCode, "STO");
      break;

    case 5:                         // CAL (Call procedure)
      pas[SP - 1] = base(BP, IR.L); // static link
      pas[SP - 2] = BP;             // dynamic link
      pas[SP - 3] = PC;             // return address
      BP = SP - 1;
      PC = IR.M;
      strcpy(opCode, "CAL");
      break;

    case 6: // INC (Allocates m slots on the stack)
      SP = SP - IR.M;
      strcpy(opCode, "INC");
      break;

    case 7: // JMP (Jumps to a new instruction)
      PC = IR.M;
      strcpy(opCode, "JMP");
      break;

    case 8: // JPC (Jumps conditionally); will automatically pop top of stack
      if (pas[SP] == 0) {
        PC = IR.M;
      }
      SP = SP + 1; 
      strcpy(opCode, "JPC");
      break;

    case 9: // SYS
      switch (IR.M) {
      case 1: // WRITE, SOU
        printf("Output Result is: %d\n", pas[SP]);
        SP = SP + 1;
        strcpy(opCode, "SOU");
        break;
      case 2: // READ, SIN
        printf("Enter an integer: ");
        SP = SP - 1;
        scanf("%d", &pas[SP]);
        strcpy(opCode, "SIN");
        break;
      case 3: // HALT, EOP (End of Program)
        strcpy(opCode, "EOP");
        halt = 1;
        break;
      }
      break;
    default:
      printf("Error: Invalid operation code.\n");
      halt = 1;
      break;
    }

    if (strcmp(opCode, "ERROR") == 0) {
      return 0;
    }
    // bpTracker[j] is an array that keeps track of BP values for each New AR
    else if (strcmp(opCode, "CAL") ==
             0) // separate each Activation Record with “|”.
    {
      bpTracker[j] = BP; // add AR record
      j++;
    } else if (strcmp(opCode, "RTN") == 0) // removes "|" once RTN is excecuted
    {
      j--;
      bpTracker[j] = 0; // deletes AR record
    }

    // Print
    printf("%-10s%-4d%-4d%-4d%-4d%-4d\t", opCode, IR.L, IR.M, PC, BP, SP);

    int k = 0;
    for (int i = ARRAY_SIZE; i >= SP; i--) { // Prints stack
      if (bpTracker[k] == i) {
        printf("| ");
        k++;
      }
      printf("%d ", pas[i]);
    }

    printf("\n");
  }
}
