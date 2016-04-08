/*
 * Programmer   : Kyle Kloberdanz
 * Date Created : 7 Apr 2016
 * Description  : 
 *      Basic Simpletron emulator
 *      -------------------------
 *      Instruction format is first 2 digits are op-code, 
 *      second 2 digits is the opperand
 *
 *      calling 'simpletron' launches the interactive shell
 *
 *      calling 'simpletron --file FILENAME' reads input from
 *      the specified file (not yet built)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE    100
#define FILE_NAME_SIZE 255

#define TRUE             1
#define FALSE            0

/* Input/Output operations */
#define NOOP             0
#define READ            10
#define WRITE           11

#define READ_ASCII      14
#define WRITE_ASCII     15
#define WRITE_NEWLINE   16

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
    puts("***           Welcome to Simpletron!           ***");
    puts("*** Please enter your program one instruction  ***");
    puts("***          (or data word) at a time.         ***");
    puts("*** Enter -99999 to stop entering your program ***");
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

    const int col_len = 5;
    int i, j;

    /* dump the memory */
    printf("MEMORY:\n");
    printf("          %d", 0);
    for (i = 1; i < col_len; ++i) {
        printf("        %d", i);
    }
    puts("");
    for (i = 0; i < size_of_memory; i += col_len) { 
        printf("%02d", i);
        for (j = 0; j < col_len; ++j) {
            printf("    %+05d", mem_arr[i+j]);
        }
        printf("%c", '\n');
    }

}

/* returns the number of memory locations user entered a value to */
int launch_shell(int memory[]) {

    printGreeting();

    /* Read input, store in memory */
    int user_input = 0;
    int i = 0;
    while ((user_input != -99999) && (i < MEMORY_SIZE)) {
        printf("%02d ? ", i);
        scanf("%d", &user_input);

        if ((user_input != -99999) && 
            (user_input >=  -9999) &&
            (user_input <=   9999) ){

            memory[i] = user_input;
            ++i;
        } else if ( ((user_input <   -9999)  ||
                     (user_input >    9999)) &&
                     (user_input != -99999)) {
            puts("*** ERROR: acceptable range is from -9999 to 9999 ***");
        }
    }

    if (i > MEMORY_SIZE) {
        puts("Out of memory! Shutting down");
        exit(EXIT_FAILURE);
    } else {
        puts("*** Program loading completed ***");
        puts("*** Program execution begins  ***");
    } 

    return i;
}

int load_file(char* filename, int mem_arr[]) {
    FILE* input_file;

    if ( !(input_file = fopen(filename, "r")) ) {
        puts("ERROR: no input file specified");
        exit(EXIT_FAILURE);
    }

    char tmp[255];
    int i = 0;
    while( fscanf(input_file, "%s", &tmp) != EOF ){
        mem_arr[i] = atoi(tmp);
        i++;
    }
    return i;
}

int main(int argc, char* argv[]) {

    char input_filename[FILE_NAME_SIZE] = "";

    int will_launch_shell = TRUE;
    int read_from_file    = FALSE;

    int i = 0;
    for (i = 0; i < argc; ++i) {
        if ( strncmp(argv[i], "-f", sizeof("-f")) == 0 ) {
            will_launch_shell = FALSE;
            read_from_file = TRUE;
            strncat(input_filename, argv[i+1], FILE_NAME_SIZE);
        } 
    } 


    /* initialize memory to all zero */
    int memory[MEMORY_SIZE];
    for (i = 0; i < MEMORY_SIZE; ++i) {
        memory[i] = 0;
    }


    if (will_launch_shell) {
        i = launch_shell(memory);
    } else if (read_from_file) {
        i = load_file(input_filename, memory);
    } else {
        puts("An error occured. Shutting down");
        exit(EXIT_FAILURE);
    }

    /* initialize special registers */
    int accumulator           = 0;
    int instruction_counter   = 0;
    int instruction_register  = 0;
    int op_code               = 0;
    int operand               = 0;

    /* execute instructions */
    for (instruction_counter  = 0; 
         instruction_counter  < i; 
         ++instruction_counter) {

        instruction_register = memory[instruction_counter];

        if ((instruction_register > 9999) || 
            (instruction_register < -9999)) {
            puts("*** ERROR: instruction too large for memory ***");
            puts("***              Shutting down              ***");
            exit(EXIT_FAILURE);
        }

        op_code = instruction_register / 100;
        operand = instruction_register % 100; 

        switch (op_code) { 

            /* Read a word from the terminal into memory */
            case READ:
                printf("? ");
                scanf("%d", &memory[operand]);
                break; 

            /* Read a word from the terminal as ascii into memory */
            case READ_ASCII:
                printf("? ");
                scanf("%s", &memory[operand]);
                break;

            /* Write a word from memory to the terminal */
            case WRITE:
                printf("%d", memory[operand]);
                break;

            /* Write a word from memory in its ascii representation */
            case WRITE_ASCII:
                printf("%c", memory[operand]);
                break;

            case WRITE_NEWLINE:
                printf("\n");
                break;

            /* Load a word from memory to the accumulator */
            case LOAD:
                accumulator = memory[operand];
                break;

            /* Store a word from the accumulator into memory */
            case STORE:
                memory[operand] = accumulator;
                break;

            /* Arithmetic opperations */
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

            /* unconditional branch */
            case BRANCH:
                instruction_counter = operand;
                break;

            /* branch if zero */
            case BRANCHZERO:
                if (accumulator == 0) {
                    instruction_counter = operand;
                }
                break;

            /* branch if negative */
            case BRANCHNEG:
                if (accumulator < 0) {
                    instruction_counter = operand;
                }
                break;

            /* display memory and registers */
            case HALT:
                puts("*** Simpletron execution terminated ***");
                computer_dump(accumulator, 
                              instruction_counter, 
                              instruction_register, 
                              op_code, 
                              operand,
                              memory,
                              MEMORY_SIZE); 
                break;

            case NOOP:
                break;

            default:
                printf("*** %3d: not a valid op_code ***\n", op_code); 
                break;
        }
    } // end for loop



    return 0;
}
