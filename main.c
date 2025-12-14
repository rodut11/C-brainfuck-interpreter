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



