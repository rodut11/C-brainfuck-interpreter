#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    OP_END = 0,
    OP_RIGHT_DP, // move pointer right | (>)
    OP_LEFT_DP,  // move pointer left  | (<)
    OP_INC_VAL,  // increase value     | (+)
    OP_DEC_VAL,  // decrease value     | (-)
    OP_OUT,      // output value       | (.)
    OP_IN,       // input value        | (,)
    OP_JMP_FWD,  // jump forward       | ([)
    OP_JMP_REV,  // jump backwards     | (])

}Operator;

typedef enum {
    SUCCESS = 0,
    FAILURE = 1
} Status;

typedef struct {
    Operator operator;
    uint16_t operand;
} Instruction;

#define PROGRAM_SIZE 8192  // 8KiB
#define TAPE_SIZE 30000 // 30.000 TAPE_SIZE according to wikipedia
#define STACK_SIZE 1024
#define FULL_STACK (stack_pointer == STACK_SIZE)

static Instruction PROGRAM[PROGRAM_SIZE];
static unsigned int stack_pointer = 0;

uint8_t tape[TAPE_SIZE]; // use uint8_t to make max size of 255
size_t ip = 0; // ip = instruction pointer
size_t cursor = 0; // dp = data pointer

uint16_t stack[STACK_SIZE];

int compile(FILE *pFile) {
    uint16_t program_counter = 0, jmp_program_counter;
    int c;
    // read file
    while ((c = getc(pFile)) != EOF && program_counter < PROGRAM_SIZE) {
        switch (c) {
            case '>': PROGRAM[program_counter].operator = OP_RIGHT_DP; break;
            case '<': PROGRAM[program_counter].operator = OP_LEFT_DP; break;
            case '+': PROGRAM[program_counter].operator = OP_INC_VAL; break;
            case '-': PROGRAM[program_counter].operator = OP_DEC_VAL; break;
            case '.': PROGRAM[program_counter].operator = OP_OUT; break;
            case ',': PROGRAM[program_counter].operator = OP_IN; break;
            case '[':
                PROGRAM[program_counter].operator = OP_JMP_FWD;
                if (FULL_STACK) return FAILURE;
                stack[stack_pointer++] = program_counter;
                break;
            case ']':
                if (stack_pointer == 0) return FAILURE;
                jmp_program_counter = stack[--stack_pointer]; // pop off the stack
                PROGRAM[program_counter].operator = OP_JMP_REV;
                PROGRAM[program_counter].operand = jmp_program_counter;
                PROGRAM[jmp_program_counter].operand = program_counter;
                break;
            default: program_counter--; break;
        }
        program_counter++;
    }
    PROGRAM[program_counter].operator = OP_END;
    return SUCCESS;
}

int execute() {
    // while the operator ran in the program is no 0
    while (PROGRAM[ip].operator != OP_END) {
        switch (PROGRAM[ip].operator) {
            case OP_RIGHT_DP: cursor++; break; // move cursor on the tape to the right
            case OP_LEFT_DP: cursor--; break;  // move cursor on the tape to the left
            case OP_INC_VAL: tape[cursor]++; break; // at the position of the cursor, increase by one
            case OP_DEC_VAL: tape[cursor]--; break; // at the position of the cursor, decrease by one
            case OP_OUT: putchar(tape[cursor]); break; // print the value at the position of the cursor
            case OP_IN: tape[cursor] = getchar(); break; // input a value at the position of a cursor
            case OP_JMP_FWD: if (tape[cursor] == 0) ip = PROGRAM[ip].operand; break;
            case OP_JMP_REV: if (tape[cursor] != 0) ip = PROGRAM[ip].operand; break;
            default: return FAILURE;
        }
        ip++; // move to the next instruction on the tape
    }

    return 0;
}
int main(int argc, const char * argv[]) {

    if (argc != 2) { // check if the user provided exactly one argument
        fprintf(stderr, "Usage: %s [FILE]\n", argv[0]);
        return FAILURE;
    }

    ip = 0;
    cursor = 0;
    stack_pointer = 0;

    FILE *pFile = fopen(argv[1], "r");
    if (pFile == NULL) {
        fprintf(stderr, "Can't open file.\n");
        return FAILURE;
    }

    Status status = compile(pFile);
    fclose(pFile);

    switch (status) {
        case SUCCESS: status = execute(); break;
        case FAILURE: fprintf(stderr, "Encountered error!\n"); break;
    };

    return status;
}