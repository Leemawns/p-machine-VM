# P-Machine Virtual Machine (C)

This project is an implementation of a **stack-based virtual machine** in C for the PL/0 programming language.  
It simulates execution of P-code instructions such as arithmetic, memory management, procedure calls, and control flow.

The VM manages a runtime stack with **activation records** (static and dynamic links) and supports recursion.  
This project was part of the *Systems Software (COP 3402)* course at the University of Central Florida.

---

## Features
- Instruction set implementation for arithmetic, logic, memory, jumps, and procedure calls.
- Runtime stack management with static and dynamic links.
- Support for recursive procedure calls.
- Error handling for invalid instructions and stack underflow/overflow.
- Debugging output for stack trace and instruction execution (optional).

---

## Build Instructions
Compile with `gcc` on Linux/macOS or Windows (via MSYS2/MinGW):
```bash
gcc -O2 -std=c11 -o vm vm.c
```


## Usage
Run the virtual machine with an input file containing P-code instructions:
```./vm input.txt```

## Example
```input.txt
-----------
9 0 5     // LIT 0,5
4 0 3     // STO 0,3
9 0 3     // LIT 0,3
2 0 3     // OPR 0,3 (ADD)
0 0 0     // SIO 0,0 (HALT)
```

## Sample Inputs in the Inputs folder:

- Inputs/input.txt
- Inputs/input2.txt
- Inputs/input3.txt

These files demonstrate test programs for the VM.

## Repository Layout (Relevant Files):
```
vm.c                  # Source code for the VM
README.md             # Project documentation
Inputs/               # Example input programs
general.c             # Source code with more in-depth comments
```
## How It Works (Conceptual Overview)
1. Instruction Fetch – Reads the next instruction (OP, L, M).

2. Instruction Decode/Execute – Performs the operation (arithmetic, jump, call, etc.).

3. Stack Management – Activation records maintain control flow and variable access.

4. Halting Condition – Execution stops when SIO/HALT instruction is reached.

## Author
- Eric Lemons

University of Central Florida — Systems Software (Fall 2025)

## Notes
- This repo contains my personal branch (main, previously Lemons) where the bulk of development occurred.
- Original group repo had minimal progress; this fork reflects my implementation and contributions.