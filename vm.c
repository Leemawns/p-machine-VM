/*
    Assignment :
    vm . c - Implement a P - machine virtual machine

    Authors : < Leonardo & Eric>

    Language : C ( only )

    To Compile :
    gcc - O2 - Wall - std = c11 -o vm vm . c

    To Execute ( on Eustis ) :
    ./ vm input . txt

    where :
    input . txt is the name of the file containing PM /0 instructions ;
    each line has three integers ( OP L M )

    Notes :
    - Implements the PM /0 virtual machine described in the homework
    instructions .
    - No dynamic memory allocation or pointer arithmetic .
    - Does not implement any VM instruction using a separate function .
    - Runs on Eustis .

    Class : COP 3402 - Systems Software - Fall 2025

    Instructor : Dr . Jie Lin

    Due Date : Friday , September 12 th , 2025
    */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int op; // Operand
    int L; // Call depth 
    int M; // Index of var in an AR
} InstructionRegister;

int main(int argc, char * argv[]) {

    InstructionRegister IR;
    int PAS[500] = 0;
    int stack[256] = 0;
    int PC = 499;
    int SP = 499;
    int BP = SP-1;
    int userInput = 0;


    // See if file is present
    if (argc < 2) {
        printf("File not found");
        return 1;
    }

    // Opening file if it exists
    FILE * filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("File not read");
        return 1;
    }

    printf("\tL\t M\tPC\tBP\tSP\tstack");
    printf("\nInitial Values: \t%d\t%d\t%d", PC, BP, SP);

    while(fscanf(filePtr, "%d %d %d", &IR.op, &IR.L, &IR.M) == 3 && getchar() != "\n") {
        // printf("%d %d %d", IR.op, IR.L, IR.M);

        while (IR.op != "SYS") { // figure out correct condition
            PAS[PC] = IR.op;
            PAS[PC-1] = IR.L;
            PAS[PC-2] = IR.M; // 45 . PC = 499
            PC = PC - 3;
        }

        // Theres an initial JUMP to know where instruction are to begin
        PC = PC - IR.M;
        // Then an initial INC to place the 5 initial values on stack
        for (int i = 0; i < 5; i++) {
            stack[i] = 0;
            printf("%d", stack[i]);
        }
        SP = SP - 5;

        scanf("Please Enter an Integer: ", &userInput);

        // If invalid op
        if (IR.op == 0)
            return -1;
        // If "LIT"
        else if (IR.op == 1) {
            SP = SP - 1;
            PAS[SP] = 5; // 5 will be input 
            // IF "OPR"
        } else if (IR.op == 2) {
            switch(IR.M) {
                case 0: //Return from subroutine and restore caller’s AR.
                    SP = BP + 1;
                    BP = PAS[SP - 2];
                    PC = PAS[SP - 3];
                    break;
                case 1: // Addition.
                    PAS[SP + 1] = PAS[SP + 1] + PAS[SP];
                    SP = SP + 1;
                    break;
                case 2:  // //Subtraction.
                    PAS[SP + 1] = PAS[SP + 1] - PAS[SP];
                    SP = SP + 1;
                    break;
                case 3:  // Multiplication.
                    PAS[SP + 1] = PAS[SP + 1] * PAS[SP];
                    SP = SP + 1;
                    break; 
                case 4:  // Integer division.
                    PAS[SP + 1] = PAS[SP + 1] / PAS[SP];
                    SP = SP + 1;
                    break;
                case 5: // Equality comparison (result 0/1).
                    PAS[SP + 1] = PAS[SP + 1] == PAS[SP];
                    SP = SP + 1;
                    break;
                case 6: // Inequality comparison (result 0/1).
                    PAS[SP + 1] = PAS[SP + 1] != PAS[SP];
                    SP = SP + 1;
                    break;
                case 7: // Less-than comparison (result 0/1).
                    PAS[SP + 1] = PAS[SP + 1] < PAS[SP];
                    SP = SP + 1;
                    break;
                case 8: // Less-or-equal comparison (result 0/1).
                    PAS[SP + 1] = PAS[SP + 1] <= PAS[SP];
                    SP = SP + 1;
                    break;
                case 9: // Greater-than comparison (result 0/1).
                    PAS[SP + 1] = PAS[SP + 1] > PAS[SP];
                    SP = SP + 1;
                    break;
                case 10:  // Greater-or-equal comparison (result 0/1).
                    PAS[SP + 1] = PAS[SP + 1] >= PAS[SP];
                    SP = SP +1;
                    break;
                default:
                    return -1;
            }
        } else if (IR.op == 3) { // Load value to top of stack from offset a in the AR n static levels down.
            SP = SP = 1;
            PAS[SP] = PAS[base(BP, IR.L) - IR.M];

        }   else if (IR.op == 4) { // Store top of stack into offset o in the AR n static levels down.
            PAS[base(BP,IR.L) - IR.M] = PAS[SP];
            SP = SP + 1;

        }   else if (IR.op == 5) { // Call procedure at code address a; create activation record.
            PAS[SP - 1] = base(BP, IR.L);
            PAS[SP - 2] = BP;
            PAS[SP - 3] = PC;

            BP = SP - 1;
            PC = IR.M;

        }   else if (IR.op == 6) {  // Allocate n locals on the stack.
            SP = SP = IR.M;
        }   else if (IR.op == 7) { // Unconditional jump to address a.
            PC = IR.M;
        }   else if (IR.op == 8) { // Conditional jump: if value at top of stack is 0, jump to a; pop the stack.
                if (PAS[SP] = 0) {
                    PC = IR.M;
                    SP = SP + 1;
            }   else if (IR.op == 9) {
                    switch(IR.M) {
                        case 1:  // Output integer value at top of stack; then pop.
                            print(PAS[SP]);
                            SP = SP +1;
                        case 2: // Read an integer from stdin and push it.
                            SP = SP - 1;
                            PAS[SP] = readInt();
                        case 3:  // Halt the program.
                            return;
                        default:
                            return -1;
                    }    
                }
            }
        }   
    fclose(filePtr);
    return 0;
}

// void print();

/* Find base L levels down from the current activation record */
// int base ( int BP , int L ) {
    // int arb = BP ; // activation record base -> arb = 439
    // while ( L > 0) {
    // arb = PAS [ arb ]; // follow static link
    // L - -;
  //  }
//return arb ;
// }

 // char buffer[256];
    // while (fgets(buffer, sizeof(buffer), filePtr) != EOF) {

    // buffer -> \n or null
        // Maybe use scanf instead, but i feel we should be used != EOF 
    
    // }
//  && getchar() != "\n"