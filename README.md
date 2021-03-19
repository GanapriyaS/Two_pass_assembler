# Two_pass_assembler

AIM

To write a program to implement two pass assembler(assembly code to machine code)

ALGORITHM

Step 1: Start a program

Step 2: Include header symbol_table.h and data_storage.h to get data from file and to implement symbol table.

Step 3: PASS 1
From the data of registers, assembly code, pseudo opcode and machine opcode(mnemonics code),branchcode create an intermediate code,symbol table ,literal table,and location counter(LC) is processed.

Step 4: PASS 2
From data of symbol table,intermediate code,opcode table machine code is generated, to resolve forward references back-patching symbol and literal table used

STEP 5: Functions pass1() and pass2() are called from main function and content of the files are displayed.

STEP 6: Stop the program
