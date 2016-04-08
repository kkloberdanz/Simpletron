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
         NOOP            00
         READ            10
         WRITE           11

         READ_ASCII      14
         WRITE_ASCII     15
         WRITE_NEWLINE   16

        Load/Store operations
         LOAD            20
         STORE           21

        Arithmetic Operations
         ADD             30
         SUBTRACT        31
         DIVIDE          32
         MULTIPLY        33

        Transfter of control Operations
         BRANCH          40     unconditional branch
         BRANCHNEG       41     branch if negative 
         BRANCHZERO      42     branch if zero
         HALT            43     display memory and registers

    A valid instruction to read an integer from the terminal, and store
    it in memory location 20 would be:
        
        READ 20

    Comments are denoted by ';'
    So a valid comment would be:

        READ 20     ; Reads an integer from terminal into mem location 20
