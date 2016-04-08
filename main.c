/*
 * Programmer: Kyle Kloberdanz
 * Description: 
 *      Basic Simpletron emulator.
 *      --------------------------
 *      Instruction format is first 2 digits are op-code, 
 *      second 2 digits is the opperand
 */

#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE     100

/* Input/Output operations */
#define READ            10
#define WRITE           11

/* Load/Store operations*/
#define LOAD            20
#define STORE           21

/* Arithmetic Operations */
#define ADD             30
#define SUBTRACT        31
#define DIVIDE          32
#define MULTIPLY        33

/* Transfter of control Operations */
#define BRANCH          40
#define BRANCHNEG       41
#define BRANCHZERO      42
#define HALT            43

void printGreeting() {
    puts("*** Welcome to Simpletron! ***");
    puts("*** Please enter your program one instruction ***");
    puts("*** (or data word) at a time. ***");
    puts("*** Enter -99999 to stop entering your program ***");
}

void halt() {
    puts("*** Simpletron execution terminated ***");
    /*exit(EXIT_SUCCESS);*/
}

void computer_dump(int acc, 
                   int ins_count, 
                   int ins_reg, 
                   int op_code, 
                   int operand,
                   int mem_arr[],
                   int size_of_memory) {

    /* dump the registers */
    printf("%s\n", "REGISTERS:");
    printf("accumulator             %+05d\n", acc);
    printf("instruction_counter     %02d\n" , ins_count);
    printf("instruction_register    %+05d\n", ins_reg);
    printf("op_code                 %02d\n", op_code);
    printf("operand                 %02d\n", operand);

    /* TODO: dump the memory */
    /*
    printf("MEMORY:\n");
    for (int i = 0; i < size_of_memory; ++i) {

    }
    */

}

int main(void) {
    printGreeting();

    int memory[MEMORY_SIZE];

    /* Read input, store in memory */
    int user_input = 0;
    int i = 0;
    while ((user_input != -99999) && (i < MEMORY_SIZE)) {
        printf("%02d ? ", i);
        scanf("%d", &user_input);

        if (user_input != -99999) {
            memory[i] = user_input;
            ++i;
        }
    }

    if (i > MEMORY_SIZE) {
        puts("Out of memory! Shutting down");
        exit(EXIT_FAILURE);
    } else {
        puts("*** Program loading completed ***");
        puts("*** Program execution begins  ***");
    }

    /* initialize special registers */
    int accumulator           = 0;
    int instruction_counter   = 0;
    int instruction_register  = 0;
    int op_code               = 0;
    int operand               = 0;

    for (instruction_counter = 0; 
         instruction_counter < i; ++instruction_counter) {
        instruction_register = memory[instruction_counter];

        op_code = instruction_register / 100;
        operand = instruction_register % 100; 

        /* TODO: implement read, store, and branching
         */
        switch (op_code) {


            case READ:
                scanf("%d", &memory[operand]);
                break; 

            case LOAD:
                accumulator = memory[operand];
                break;

            case ADD:
                accumulator += memory[operand];
                break;

            case SUBTRACT:
                accumulator -= memory[operand];
                break;

            case DIVIDE:
                accumulator /= memory[operand];
                break;

            case MULTIPLY:
                accumulator *= memory[operand];
                break;

            case HALT:
                halt();
                break;
        }

        computer_dump(accumulator, 
                      instruction_counter, 
                      instruction_register, 
                      op_code, 
                      operand,
                      memory,
                      MEMORY_SIZE);
    }

    return 0;
}
