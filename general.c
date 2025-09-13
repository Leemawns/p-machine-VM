/*
    Assignment :
    vm.c - Implement a P-machine virtual machine

    Authors : < Leonardo & Eric>

    Language : C (only)

    To Compile :
    gcc -O2 -Wall -std=c11 -o vm vm.c

    To Execute (on Eustis) :
    ./vm input.txt

    where:
    input.txt is the name of the file containing PM/0 instructions;
    each line has three integers (OP L M)

    Notes:
    - Implements the PM/0 virtual machine described in the homework instructions.
    - No dynamic memory allocation or pointer arithmetic.
    - Does not implement any VM instruction using a separate function.
    - Runs on Eustis.

    Class: COP 3402 - Systems Software - Fall 2025
    Instructor: Dr. Jie Lin
    Due Date: Friday, September 12th, 2025
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Top of the text segment in PAS.
   Per the spec, code is stored at the top of PAS (address 499 downward).
   For JMP/CAL/JPC, M is an offset from this top; targets are computed as PC = CODETOP - M. */
#define CODETOP 499

/* Instruction register: holds the OP/L/M of the instruction just fetched. */
typedef struct {
    int op; // Opcode
    int L;  // Lexicographical level (static depth)
    int M;  // Modifier/operand: meaning depends on OP (literal, offset, sub-opcode, or code addr offset)
} InstructionRegister;

/* Single process address space (PAS), size 500, initialized to 0.
   This single array backs BOTH segments:
   - Text segment (code) at the top, growing downward
   - Stack segment (data) below code, growing downward
   No duplicate “stack” array is used anywhere. */
int PAS[500] = {0};

/* CPU registers (globals permitted by clarification):
   PC: next instruction address in text segment
   BP: base of current activation record
   SP: top-of-stack pointer (stack grows downward) */
int PC = 0;
int BP = 0;
int SP = 0;

/* Global instruction register instance (allowed). */
InstructionRegister IR;

/* Prototypes */
void printState(int op, int L, int M, int lastMIndex);
int base(int BP, int L);

int main(int argc, char * argv[]) {

    /* Exactly one argument (the input file) is required. */
    if (argc != 2) {
        printf("File not found\n");
        return 1;
    }

    /* Open the input program (three integers per line: OP L M). */
    FILE * filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("File not read\n");
        return 1;
    }

    /* Load code triples into PAS starting at 499 downward:
       (OP at 499, L at 498, M at 497), then next at 496/495/494, etc.
       Track the lowest M index loaded so we can place SP and BP correctly. */
    int op, l, m;
    int topIndex = 499;    // next slot in PAS for OP of the next instruction
    int lastMIndex = 499;  // lowest address used by the text segment (last written M)

    while (fscanf(filePtr, "%d %d %d", &op, &l, &m) == 3) {
        PAS[topIndex]     = op; // OP
        PAS[topIndex - 1] = l;  // L
        PAS[topIndex - 2] = m;  // M
        topIndex -= 3;
        if (topIndex + 1 < lastMIndex) {
            lastMIndex = topIndex + 1; // remember the lowest M address written
        }
    }
    fclose(filePtr);

    /* Register init per spec:
       - PC = 499 (top of text segment) before first fetch
       - SP = lastMIndex (lowest text address used)
       - BP = SP - 1 (base is just above stack top before any push) */
    PC = 499;
    SP = lastMIndex;
    BP = SP - 1;
    IR.op = IR.L = IR.M = 0;

    /* Header + initial register line. Spacing/tabbing is preserved. */
    printf("\tL \tM \tPC \tBP \tSP \tstack\n");
    printf("Initial values: \t%d \t%d \t%d\n", PC, BP, SP);

    int halt = 0;

    /* Fetch–Execute loop: fetch (copy OP/L/M and decrement PC by 3), then execute. */
    while (!halt) {
        /* FETCH (code is stored downward, so PC moves down by 3 each fetch) */
        IR.op = PAS[PC];
        IR.L  = PAS[PC - 1];
        IR.M  = PAS[PC - 2];
        PC -= 3;

        /* EXECUTE */
        switch (IR.op) {
            case 1: // LIT: push literal M
                SP--;
                PAS[SP] = IR.M;
                break;

            case 2: // OPR: arithmetic/logic/return based on sub-opcode M
                switch (IR.M) {
                    case 0: // RTN: restore caller’s AR and return
                        SP = BP + 1;
                        BP = PAS[SP - 2]; // DL
                        PC = PAS[SP - 3]; // RA
                        break;
                    case 1: // ADD: (x,y) -> x+y
                        PAS[SP+1] = PAS[SP+1] + PAS[SP];
                        SP++;
                        break;
                    case 2: // SUB: (x,y) -> x-y
                        PAS[SP+1] = PAS[SP+1] - PAS[SP];
                        SP++;
                        break;
                    case 3: // MUL: (x,y) -> x*y
                        PAS[SP+1] = PAS[SP+1] * PAS[SP];
                        SP++;
                        break;
                    case 4: // DIV: integer division (x,y) -> x/y
                        PAS[SP+1] = PAS[SP+1] / PAS[SP];
                        SP++;
                        break;
                    case 5: // EQL: (x,y) -> (x==y)
                        PAS[SP+1] = PAS[SP+1] == PAS[SP];
                        SP++;
                        break;
                    case 6: // NEQ: (x,y) -> (x!=y)
                        PAS[SP+1] = PAS[SP+1] != PAS[SP];
                        SP++;
                        break;
                    case 7: // LSS: (x,y) -> (x<y)
                        PAS[SP+1] = PAS[SP+1] < PAS[SP];
                        SP++;
                        break;
                    case 8: // LEQ: (x,y) -> (x<=y)
                        PAS[SP+1] = PAS[SP+1] <= PAS[SP];
                        SP++;
                        break;
                    case 9: // GTR: (x,y) -> (x>y)
                        PAS[SP+1] = PAS[SP+1] > PAS[SP];
                        SP++;
                        break;
                    case 10: // GEQ: (x,y) -> (x>=y)
                        PAS[SP+1] = PAS[SP+1] >= PAS[SP];
                        SP++;
                        break;
                    default:
                        halt = 1; // invalid sub-op: stop
                        break;
                }
                break;

            case 3: // LOD: push value at offset M in AR L levels down
                SP--;
                PAS[SP] = PAS[base(BP, IR.L) - IR.M];
                break;

            case 4: // STO: store top of stack to offset M in AR L levels down; pop
                PAS[base(BP, IR.L) - IR.M] = PAS[SP];
                SP++;
                break;

            case 5: // CAL: call procedure at code offset M; build AR = [SL, DL, RA]
                PAS[SP - 1] = base(BP, IR.L); // SL (static link)
                PAS[SP - 2] = BP;            // DL (dynamic link)
                PAS[SP - 3] = PC;            // RA (return address)
                BP = SP - 1;                 // new base
                PC = CODETOP - IR.M;         // jump to target code address
                break;

            case 6: // INC: allocate M locals (stack grows downward)
                SP -= IR.M;
                break;

            case 7: // JMP: unconditional jump to code offset M
                PC = CODETOP - IR.M;
                break;

            case 8: // JPC: if top of stack == 0 then jump to code offset M; pop
                if (PAS[SP] == 0) {
                    PC = CODETOP - IR.M;
                }
                SP++;
                break;

            case 9: // SYS: 1=write top; 2=read int; 3=halt
                if (IR.M == 1) {
                    /* Print and pop (exact prompt/output strings preserved) */
                    printf("Output result is: %d\n", PAS[SP]);
                    SP++;
                } else if (IR.M == 2) {
                    int input;
                    printf("Please Enter an Integer: ");
                    scanf("%d", &input);
                    SP--;
                    PAS[SP] = input;
                } else if (IR.M == 3) {
                    halt = 1;
                }
                break;

            default:
                halt = 1; // invalid opcode: stop
                break;
        }

        /* Print machine state after each instruction, matching the assignment’s trace format. */
        printState(IR.op, IR.L, IR.M, lastMIndex);
    }

    return 0;
}

/* Find base L levels down from the current activation record.
   Follows the static-link chain L times (SL is stored at PAS[BP]). */
int base (int BP, int L) {
    int arb = BP; // activation record base
    while (L > 0) {
        arb = PAS[arb]; // follow static link (SL)
        L--;
    }
    return arb;
}

/* Print the machine state in required format.
   Conventions used here (matches sample trace):
   - Print registers (L M PC BP SP), then a tab, then the stack.
   - The stack is shown from high address down to SP.
   - If inside a callee frame with locals allocated (SP <= BP-3), print:
       [caller region above BP+1]  |  SL DL RA  [locals/temps from BP-3 down to SP]
     Otherwise (e.g., right after CAL), no bar is printed yet. */
void printState(int op, int L, int M, int lastMIndex) {

    /* Print mnemonic.
       For OPR, print the sub-op name (RTN, ADD, SUB, MUL, DIV, EQL, NEQ, LSS, LEQ, GTR, GEQ). */
    if (op == 2){
        switch(M) {
            case 0:
                printf("RTN ");
                break;
            case 1:
                printf("ADD ");
                break;
            case 2:
                printf("SUB ");
                break;
            case 3:
                printf("MUL ");
                break;
            case 4:
                printf("DIV ");
                break;
            case 5:
                printf("EQL ");
                break;
            case 6:
                printf("NEQ ");
                break;
            case 7:
                printf("LSS ");
                break;
            case 8:
                printf("LEQ ");
                break;
            case 9:
                printf("GTR ");
                break;
            case 10:
                printf("GEQ ");
                break;
            default:
                printf("OPR ");
        }
    } else {
        switch (op) {
            case 1:
                printf("LIT ");
                break;
            case 3:
                printf("LOD ");
                break;
            case 4:
                printf("STO ");
                break;
            case 5:
                printf("CAL ");
                break;
            case 6:
                printf("INC ");
                break;
            case 7:
                printf("JMP ");
                break;
            case 8:
                printf("JPC ");
                break;
            case 9:
                printf("SYS ");
                break;
            default:
                printf("??? ");
                break;
        }
    }

    /* Registers (tab before the stack area; keep spacing as you have it). */
    printf("\t%d \t%d \t%d \t%d \t%d\t", L, M, PC, BP, SP);

    int mainBP = lastMIndex - 1;
    if(BP == mainBP){
        /* In main frame: print entire stack region high->low, no bar. */
        for (int i = lastMIndex - 1; i >= SP; i--)
            printf("%-2d ", PAS[i]);
        printf("\n");
        return;
    }

    if(SP <= BP - 3){
        /* Callee frame with locals/temps present: print caller region left of bar. */
        for(int i  = lastMIndex - 1; i >= BP + 1; i--)
            printf("%-2d ", PAS[i]);

        /* Bar + control fields: SL, DL, RA (no newline here). */
        printf("| %d %d %d ", PAS[BP], PAS[BP - 1], PAS[BP - 2]);
        /* PAS[BP]     = SL (static link)
           PAS[BP - 1] = DL (dynamic link)
           PAS[BP - 2] = RA (return address)
           PAS[BP - 3] = first callee slot below RA (locals/temps begin here) */

        /* Then callee data right of the bar: from BP-3 down to SP. */
        for(int i = BP - 3; i >= SP; i--)
            printf("%-2d ", PAS[i]);
        printf("\n");
    } else {
        /* Just after CAL (no locals allocated yet): no bar. */
        for(int i = lastMIndex - 1; i >= SP; i--)
            printf("%-2d ", PAS[i]);
        printf("\n");
    }
}
