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

        printf("Please Enter an Integer :")

        if (IR.op == 0)
            return -1;
        else if (IR.op == 1) {
            SP = SP - 1;
            PAS[SP] = 5 // 5 will be input 
        } else if {IR.op == 2} {
            switch(IR.op) {
                case 1:
                    Return from subroutine and restore caller’s AR.
                    sp = bp +1
                    bp ← pas[sp−2]
                    pc ← pas[sp−3]
                case 2: 
                    Addition.
                    pas[sp+1] ← pas[sp+1] + pas[sp]
                    sp ← sp +1
                case 3: 
                    Subtraction.
                    pas[sp+1] ← pas[sp+1] − pas[sp]
                    sp ← sp +1
                case 4: 
                    Multiplication.
                    pas[sp+1] ← pas[sp+1] ∗ pas[sp]
                    sp ← sp +1
                case 5: 
                    Integer division.
                    pas[sp+1] ← pas[sp+1] / pas[sp]
                    sp ← sp +1
                case 6:
                    Equality comparison (result 0/1).
                    pas[sp+1] ← (pas[sp+1] == pas[sp])
                    sp ← sp +1
                case 7:
                    Inequality comparison (result 0/1).
                    pas[sp+1] ← (pas[sp+1] ̸ = pas[sp])
                    sp ← sp +1
                case 8:
                    Less-than comparison (result 0/1).
                    pas[sp+1] ← (pas[sp+1] < pas[sp])
                    sp ← sp +1
                case 9:
                    Less-or-equal comparison (result 0/1).
                    pas[sp+1] ← (pas[sp+1] ≤ pas[sp])
                    sp ← sp +1
                case 10:
                    Greater-than comparison (result 0/1).
                    pas[sp+1] ← (pas[sp+1] > pas[sp])
                    sp ← sp +1
                case 11: 
                    Greater-or-equal comparison (result 0/1).
                    pas[sp+1] ← (pas[sp+1] ≥ pas[sp])
                    sp ← sp +1
                default:
                    return -1;
            
        else if (IR.op == 3) {
            Load value to top of stack from offset a in the AR n static levels down.
            sp ← sp −1
            pas[sp] ← pas[base(bp,n) −a]

        }   else if (IR.op == 4) {
            Store top of stack into offset o in the AR n static levels down.
            pas[base(bp,n) −o] ← pas[sp]
            sp ← sp +1
            
        }   else if (IR.op == 5) {
            Call procedure at code address a; create activation record.
            pas[sp−1] ← base(bp,n)
            pas[sp−2] ← bp
            pas[sp−3] ← pc
            bp ← sp−1
            pc ← a
            
        }   else if (IR.op == 6) {

            Allocate n locals on the stack.
            sp ← sp −n
            
        }   else if (IR.op == 7) {
            Unconditional jump to address a.
            pc ← a
            
        }   else if (IR.op == 8) {

            Conditional jump: if value at top of stack is 0, jump
            to a; pop the stack.
            if pas[sp] = 0 then pc ← a
            sp ← sp +1
            
        }   else if (IR.op == 9) {
                switch(IR.op) {
                    case 1: 
                        Output integer value at top of stack; then pop.
                        print(pas[sp])
                        sp ← sp +1
                    case 2:
                        Read an integer from stdin and push it.
                        sp ← sp −1
                        pas[sp] ← readInt()
                    case 3: 
                        Halt the program.
                        halt
                    default:
                        return -1;
                }
            
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
    // arb = pas [ arb ]; // follow static link
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