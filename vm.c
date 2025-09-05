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

const PAS[500] = 0;

int PC = 499;
int SP = 500;

int main(int argc, char * argv[]) {

    // See if file is present
    if (argc < 2) {
        printf("File not found");
        return 1;
    }


    FILE * filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("File not read");
        return 1;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), filePtr) != EOF) {
        // Maybe use scanf instead, but i feel we should be used != EOF 
    
    }

    

    fclose(filePtr);
    return 0;
    

}

// void print();

/* Find base L levels down from the current activation record */
// int base ( int BP , int L ) {
    // int arb = BP ; // activation record base
    // while ( L > 0) {
    // arb = pas [ arb ]; // follow static link
    // L - -;
  //  }
//return arb ;
// }