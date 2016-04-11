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
                   int math_opperand_reg,
                   int op_code, 
                   int operand,
                   int mem_arr[],
                   int size_of_memory) {

    /* dump the registers */
    printf("%s\n", "REGISTERS:");
    printf("accumulator             %+05d\n", acc);
    printf("instruction_counter     %02d\n" , ins_count);
    printf("instruction_register    %+05d\n", ins_reg);
    printf("math_opperand_register  %+05d\n", math_opperand_reg);
    printf("op_code                 %02d\n" , op_code);
    printf("operand                 %02d\n" , operand);

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

int is_valid_input(int input) {
    if ((input >=  -9999) &&
        (input <=   9999) ){
        return TRUE;
    } else {
        return FALSE;
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

        if (is_valid_input(user_input)) {
            memory[i] = user_input;
            ++i;
        } else if (user_input == -99999) {
            ;
        } else { 
            puts("*** ERROR: acceptable range is from -9999 to 9999 ***");
        }
    }

    return i;
}

int load_file(char* filename, int mem_arr[]) {
    int j;
    char extension[4];

    /* get extension type */
    for (j = 0; filename[j] != '\0'; ++j) {
        if (filename[j] == '.') {
            if ( (filename[j+1] == 's') &&
                 (filename[j+2] == 'm') &&
                 (filename[j+3] == 'l') ) {
                  
                strcpy(extension, "sml");
            }  else if ( (filename[j+1] == 's') &&
                         (filename[j+2] == 'a') &&
                         (filename[j+3] == 'l') ) {
                strcpy(extension, "sal");
            } else {
                puts("*** invalid file type ***");
                puts("*** valid extensions are .sml and .sal ***");
                exit(EXIT_FAILURE);
            }
        }
    }

    /* check if file type is valid */
    if ( strcmp(extension, "sal") == 0 ) {
        puts("Reading an assembly file has not been built yet");
        exit(EXIT_FAILURE);
    }

    FILE* input_file;

    if ( !(input_file = fopen(filename, "r")) ) {
        puts("ERROR: no input file specified");
        exit(EXIT_FAILURE);
    }

    int line_number = 0;
    char tmp[255];
    int i = 0;
    int is_comment = FALSE;
    int next_is_comment = FALSE;

    /*
     * TODO: line number not working for error handling.
     *       Maybe read char by char instead of by string?
     *
     */
    while( fscanf(input_file, "%s", &tmp) != EOF ){ 

        /* remove comments and spaces */
        for (j = 0; tmp[j] != '\0'; ++j) { 

            if (tmp[j] == '\n') {
                line_number++;
            }

            if (tmp[j] == ' ') {
                tmp[j] = '\0';
                break;
            } 

            if (tmp[j] == '#') {
                tmp[j] = '\0';
                next_is_comment = !next_is_comment;
                break;
            } 
        } 

        /* check if valid input, check if comment, if not put in memory */
        if ((tmp[0] != 0) && ((tmp != '\0') && (!is_comment))) { 
            if (is_valid_input(atoi(tmp))) {
                mem_arr[i] = atoi(tmp);
                i++;
            } else {
                printf("error in file \'%s\' on line %d\n"
                                    , filename, line_number);
                exit(EXIT_FAILURE);
            }
        } 
        
        is_comment = next_is_comment;
    } /* end while */
    fclose(input_file);

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


    /* decide if reading from file, or shell */
    if (will_launch_shell) {
        i = launch_shell(memory);
    } else if (read_from_file) {
        i = load_file(input_filename, memory);
    } else {
        puts("An error occured. Shutting down");
        exit(EXIT_FAILURE);
    }

    if (i > MEMORY_SIZE) {
        puts("Out of memory! Shutting down");
        exit(EXIT_FAILURE);
    } else {
        puts("*** Program loading completed ***");
        puts("*** Program execution begins  ***");
    } 

    /* initialize special registers */
    int accumulator            = 0;
    int instruction_counter    = 0;
    int instruction_register   = 0;
    int math_opperand_register = 0;

    int op_code                = 0;
    int operand                = 0;

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

            /* Write newline to the terminal */
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
                math_opperand_register = memory[operand];
                accumulator += math_opperand_register;
                break;

            case SUBTRACT:
                math_opperand_register = memory[operand];
                accumulator -= math_opperand_register;
                break;

            case DIVIDE:
                math_opperand_register = memory[operand];
                accumulator /= math_opperand_register;
                break;

            case MULTIPLY:
                math_opperand_register = memory[operand];
                accumulator *= math_opperand_register;
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
                              math_opperand_register,
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
