#include <stdio.h>
#include <stdint.h>

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

typedef struct {
    Operator operator;
    uint16_t operand;
} Instruction;

//TODO: use malloc() and NOT ,you hear me, NOT use a fixed size like 8KiB
#define PROGRAM_SIZE 8192  // 8KiB
#define TAPE_SIZE 30000 // 30.000 TAPE_SIZE according to wikipedia

static Instruction PROGRAM[PROGRAM_SIZE];

uint8_t tape[TAPE_SIZE]; // use uint8_t to make max size of 255
size_t ip = 0; // ip = instruction pointer
size_t dp = 0; // dp = data pointer

int execute() {

    while (PROGRAM[ip].operator != OP_END) {
        switch (PROGRAM[ip].operator) {
            case OP_RIGHT_DP: dp++; break;
            case OP_LEFT_DP: dp--; break;
            case OP_INC_VAL: tape[dp]++; break;
            case OP_DEC_VAL: tape[dp]--; break;
            case OP_OUT: putchar(tape[dp]); break;
            case OP_IN: tape[dp] = getchar(); break;
            case OP_JMP_FWD: if (tape[dp] == 0) ip = PROGRAM[ip].operand; break;
            case OP_JMP_REV: if (tape[dp] != 0) ip = PROGRAM[ip].operand; break;
        }
        ip++;
    }

    return 0;
}
int main() {
    PROGRAM[0] = (Instruction){ OP_INC_VAL, 0 };
    PROGRAM[1] = (Instruction){ OP_INC_VAL, 0 };
    PROGRAM[2] = (Instruction){ OP_OUT, 0 };
    PROGRAM[3] = (Instruction){ OP_END, 0 };

    execute();

}
