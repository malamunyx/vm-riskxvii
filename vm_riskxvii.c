#include <stdio.h>
#include <stdlib.h>
#include "isa.h"
#include "vm_riskxvii.h"


int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Error: only singular filename argument required\n");
        exit(1);
    }

    /* Ensure it is valid and readable */
    FILE *f = fopen(argv[1], "rb");
    if (f == NULL) {
        fprintf(stderr, "Error: %s cannot be read\n", argv[1]);
        exit(1);
    }

    struct core c = {
        .pc = 0,
        .reg= {0}, 
        .mem = {0},
        .heap = {0},
        .start = hist_init()
    };

    /* All .mi files are guaranteed 2048 bytes of memory */
    if (fread(c.mem, sizeof(int8_t), MEM_SIZE, f) != MEM_SIZE) {
        fprintf(stderr, "Error loading memory image\n");
        exit(1);
    }
    
    /* Close FILE buffer after succesfully loading */
    fclose(f);

    /* Initiate fetch, decode and execute cycle */
    uint32_t inst = 0;
    while ((inst = fetch(&c)) != 0) {

        int retval = exec(&c, inst);
        
        if (retval == INST_FAIL) {
            break;
        } else if (retval == INST_JUMP) {
            continue;
        } else {
            // INST_SUCCESS
            c.pc += INST_SIZE;
        }
    }

    // Free our heap every time we exit
    list_free(c.start);

    return 0;
}

/* Fetches 4-bit binary (uint32) for exec to parse */
uint32_t fetch(struct core *c) {
    return c->mem[c->pc] 
        | c->mem[c->pc + 1] << 8
        | c->mem[c->pc + 2] << 16
        | c->mem[c->pc + 3] << 24;
}

/* Errors */
void err_inst_not_implemented(struct core *c, uint32_t inst) {
    printf("Instruction Not Implemented: 0x%08x\n", inst);
    pc_dump(c);
    reg_dump(c);
}

/* Decode and Execute */
int32_t exec(struct core *c, uint32_t inst) {
    uint8_t op = opcode(inst);
    uint8_t f3 = func3(inst);
    uint8_t f7 = func7(inst);

    switch (op) {
        case OP_R: // Type R : 0x33
            switch (f3) {
                case 0x0: 
                    switch (f7) {
                        case 0x0:
                            return exec_add(c, inst);
                        case 0x20:
                            //printf("sub:\n");
                            return exec_sub(c, inst);
                        default: 
                            err_inst_not_implemented(c, inst);
                            return INST_FAIL;
                    }
                case 0x1:
                    return exec_sll(c, inst);
                case 0x2:
                    return exec_slt(c, inst);
                case 0x3:
                    return exec_sltu(c, inst);
                case 0x4:
                    return exec_xor(c, inst);
                case 0x5:
                    switch (f7) {
                        case 0x0:
                            return exec_srl(c, inst);
                        case 0x20:
                            return exec_sra(c, inst);
                        default: 
                            err_inst_not_implemented(c, inst);
                            return INST_FAIL;
                    }
                case 0x6:
                    return exec_or(c, inst);
                case 0x7:
                    return exec_and(c, inst);
                default:
                    err_inst_not_implemented(c, inst);
                    return INST_FAIL;
            }
        case OP_I_AR: // Type I_Arithmetic : 0x13
            switch (f3) {
                case 0x0:
                    return exec_addi(c, inst);
                case 0x2:
                    return exec_slti(c, inst);
                case 0x3:
                    return exec_sltiu(c, inst);
                case 0x4:
                    return exec_xori(c, inst);
                case 0x6:
                    return exec_ori(c, inst);
                case 0x7:
                    return exec_andi(c, inst);
                default:
                    err_inst_not_implemented(c, inst);
                    return INST_FAIL;
            }
        case OP_I_MA: // Type I_MemAccess : 0x3
            switch (f3) {
                case 0x0:
                    return exec_lb(c, inst);
                case 0x1:
                    return exec_lh(c, inst);
                case 0x2:
                    return exec_lw(c, inst);
                case 0x4:
                    return exec_lbu(c, inst);
                case 0x5:
                    return exec_lhu(c, inst);
                default:
                    err_inst_not_implemented(c, inst);
                    return INST_FAIL;
            }
        case OP_I_PF: // Type I_ProgramFlow : 0x67
            switch (f3) {
                case 0x0:
                    return exec_jalr(c, inst);
                default:
                    err_inst_not_implemented(c, inst);
                    return INST_FAIL;
            }
        case OP_S: // Type S : 0x23
            switch (f3) {
                case 0x0:
                    return exec_sb(c, inst);
                case 0x1:
                    return exec_sh(c, inst);
                case 0x2:
                    return exec_sw(c, inst);
                default:
                    err_inst_not_implemented(c, inst);
                    return INST_FAIL;
            }
        case OP_SB: // Type SB : 0x63
            switch (f3) {
                case 0x0:
                    return exec_beq(c, inst);
                case 0x1:
                    return exec_bne(c, inst);
                case 0x4:
                    return exec_blt(c, inst);
                case 0x5:
                    return exec_bge(c, inst);
                case 0x6:
                    return exec_bltu(c, inst);
                case 0x7:
                    return exec_bgeu(c, inst);
                default:
                    err_inst_not_implemented(c, inst);
                    return INST_FAIL;
            }
        case OP_U: // Type U : 0x37
            return exec_lui(c, inst);
        case OP_UJ: // Type UJ : 0x6f
            return exec_jal(c, inst);
        default:
            err_inst_not_implemented(c, inst);
            return INST_FAIL;
    }
}