#ifndef ISA_H
#define ISA_H

#include <stdint.h>
#include "heap.h"

#define INST_FAIL (0)
#define INST_SUCCESS (1)
#define INST_JUMP (2)

#define REG_SIZE (32)
#define MEM_SIZE (2048)

#define INST_MEM_START (0)
#define INST_MEM_END (1023)
#define DATA_MEM_START (1024)
#define DATA_MEM_END (2047)

#define VIRT_RTN_START (0x800)
#define VIRT_RTN_END (0x8ff)

/* OPCODES */
#define OP_R (0x33)
#define OP_I_AR (0x13)
#define OP_I_MA (0x3)
#define OP_I_PF (0x67)
#define OP_S (0x23)
#define OP_SB (0x63)
#define OP_U (0x37)
#define OP_UJ (0x6f)

/* bitvals */
#define INT_BITS (32)

/* Core Struct (memory, registers and PC) */
struct core {
    int32_t pc;
    int32_t reg[REG_SIZE];
    uint8_t mem[MEM_SIZE];

    struct node *start;
    uint8_t heap[HEAP_CAPACITY];
};

/* Decoding Functions */
uint8_t opcode(uint32_t inst);
uint8_t func3(uint32_t inst);
uint8_t func7(uint32_t inst);
uint8_t rd(uint32_t inst);
uint8_t rs1(uint32_t inst);
uint8_t rs2(uint32_t inst);
int32_t imm_I(uint32_t inst);
int32_t imm_S(uint32_t inst);
int32_t imm_SB(uint32_t inst);
int32_t imm_U(uint32_t inst);
int32_t imm_UJ(uint32_t inst); 
int32_t sext(int32_t val, int8_t bitsize);

/* Assembly instructions - Arithmetic */
int32_t exec_add(struct core *c, uint32_t inst);
int32_t exec_addi(struct core *c, uint32_t inst); 
int32_t exec_sub(struct core *c, uint32_t inst); 
int32_t exec_lui(struct core *c, uint32_t inst); 
int32_t exec_xor(struct core *c, uint32_t inst); 
int32_t exec_xori(struct core *c, uint32_t inst); 
int32_t exec_or(struct core *c, uint32_t inst); 
int32_t exec_ori(struct core *c, uint32_t inst); 
int32_t exec_and(struct core *c, uint32_t inst); 
int32_t exec_andi(struct core *c, uint32_t inst);
int32_t exec_sll(struct core *c, uint32_t inst); 
int32_t exec_srl(struct core *c, uint32_t inst); 
int32_t exec_sra(struct core *c, uint32_t inst); 

/* Assembly instructions - Memory Access */
int32_t exec_lb(struct core *c, uint32_t inst); 
int32_t exec_lh(struct core *c, uint32_t inst); 
int32_t exec_lw(struct core *c, uint32_t inst); 
int32_t exec_lbu(struct core *c, uint32_t inst); 
int32_t exec_lhu(struct core *c, uint32_t inst); 
int32_t exec_sb(struct core *c, uint32_t inst); 
int32_t exec_sh(struct core *c, uint32_t inst);
int32_t exec_sw(struct core *c, uint32_t inst); 

/* Assembly instructions - Program Flow */
int32_t exec_slt(struct core *c, uint32_t inst); 
int32_t exec_slti(struct core *c, uint32_t inst); 
int32_t exec_sltu(struct core *c, uint32_t inst); 
int32_t exec_sltiu(struct core *c, uint32_t inst); 
int32_t exec_beq(struct core *c, uint32_t inst); 
int32_t exec_bne(struct core *c, uint32_t inst);
int32_t exec_blt(struct core *c, uint32_t inst); 
int32_t exec_bltu(struct core *c, uint32_t inst); 
int32_t exec_bge(struct core *c, uint32_t inst); 
int32_t exec_bgeu(struct core *c, uint32_t inst); 
int32_t exec_jal(struct core *c, uint32_t inst); 
int32_t exec_jalr(struct core *c, uint32_t inst);

/* Virtual Register Functions */
int32_t get_int(struct core *c);
void reg_dump(struct core *c);
void pc_dump(struct core *c);
int32_t virt_load(struct core *c, int32_t addr);
void virt_store(struct core *c, int32_t addr, int32_t val);

/* Errors */
void illegal_access_errmsg(struct core *c);

#endif