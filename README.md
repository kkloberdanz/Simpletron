# Simpletron

A basic simpletron emulator coded in C

Simpletron Machine Language code can be executed from a .sml file

(this feature is coming soon)
Assembly code can be executed from a .sal file
SAL or Simpletron Assembly Language is as follows:
    
    Each instruction is of the following format
        OP_CODE OPPERAND
    
    OP_CODE's and OPPERAND's are each 2 digits in width, making
    each instruction 4 digits wide (as this is mearly a simulator, 
    digits are considered rather than bits)

    The current valid OP_CODE's with corresponding machine code 
    and description are:

        Input/Output operations
         NOOP            00     Do nothing

         READ            10     Read word from terminal, 
                                store at memory[OPPERAND]

         WRITE           11     Write to terminal word at the memory
                                location of opperand

         READ_ASCII      14     Same as read, but for ascii text

         WRITE_ASCII     15     Same as write, but for ascii text

         WRITE_NEWLINE   16     Writes a new line to terminal

        Load/Store operations
         LOAD            20     Load a word from memory[OPPERAND] 
                                into the accumulator

         STORE           21     Store a word from the accumulator
                                into memory[OPPERAND]

        Arithmetic Operations:  Results stored in accumulator
         ADD             30     add opperand to accumulator

         SUBTRACT        31     subract opperand from accumulator

         DIVIDE          32     divide accumulator by opperand

         MULTIPLY        33     multiply accumulator by opperand

        Transfter of control Operations
         BRANCH          40     unconditional branch, goto opperand

         BRANCHNEG       41     branch if negative, goto opperand

         BRANCHZERO      42     branch if zero, goto opperand

         HALT            43     display memory and registers, goto opperand

    A valid instruction to read an integer from the terminal, and store
    it in memory location 20 would be:
        
        READ 20

    Comments are denoted by ';'
    So a valid comment would be:

        READ 20     ; Reads an integer from terminal into mem location 20

    The Simpletron Machine has 3 special registers:
        accumulator             Used for arithmetic and to store values 
                                for later use.

        instruction_counter     Points to the memory location to fetch
                                instructions from.

        instruction_register    Holds the op_code and the opperand
                                Example: 
                                    if instruction_register = 1120 
                                    OP_CODE  = 11
                                    OPPERAND = 20

                                    This instruction writes the integer
                                    11 to memory location 20
