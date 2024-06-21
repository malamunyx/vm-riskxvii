#ifndef VM_RISKXVII_H
#define VM_RISKXVII_H

/* Responsible with execution flows */

#include "isa.h"

#define INST_SIZE (4)

uint32_t fetch(struct core *c);

int32_t exec(struct core *c, uint32_t inst); 
int32_t exec_R(struct core *c, uint32_t inst, uint8_t f3, uint8_t f7);
int32_t exec_IAR(struct core *c, uint32_t inst, uint8_t f3);
int32_t exec_IMA(struct core *c, uint32_t inst, uint8_t f3);
int32_t exec_IPF(struct core *c, uint32_t inst, uint8_t f3);
int32_t exec_S(struct core *c, uint32_t inst, uint8_t f3);
int32_t exec_SB(struct core *c, uint32_t inst, uint8_t f3);

/* Errors */
void err_inst_not_implemented(struct core *c, uint32_t inst);
void err_uknown_retval(struct core *c, uint32_t inst);

#endif