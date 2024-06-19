#include "isa.h"
#include "heap.h"
#include "vm_riskxvii.h"


/* Write to reigster */
void reg_write(struct core *c, uint8_t rd, int32_t val) {
    if (rd < 0 || rd >= REG_SIZE) {
        illegal_access_errmsg(c);
        list_free(c->start);
        exit(1);
    }

    if (rd == 0) {
        return;
    }

    c->reg[rd] = val;
}

/* Maximum index 2047 to read a byte */
int8_t mload_b(struct core *c, int32_t addr) {
    if (addr >= VIRT_RTN_START && addr <= VIRT_RTN_END) {
        return virt_load(c, addr);
    /* Can load from range [0, 2047] */
    } else if (addr >= INST_MEM_START && addr <= DATA_MEM_END) {
        return c->mem[addr];
    } else if (get_access(c->start, addr - HEAP_START) != NULL) {
        return c->heap[addr - HEAP_START];
    } else {
        illegal_access_errmsg(c);
        list_free(c->start);
        exit(1);
    }
}

/* Maximum index 2046 to read 2 bytes */
int16_t mload_h(struct core *c, int32_t addr) {
    if (addr >= VIRT_RTN_START && addr <= VIRT_RTN_END) {
        return virt_load(c, addr);
    /* Can load from range [0, 2046] */
    } else if (addr >= INST_MEM_START && addr <= DATA_MEM_END - 1) {
        return c->mem[addr]
            | c->mem[addr + 1] << 8;
    } else if (get_access(c->start, addr - HEAP_START) != NULL) {
        return c->heap[addr - HEAP_START]
            | c->heap[addr - HEAP_START + 1] << 8;
    } else {
        illegal_access_errmsg(c);
        list_free(c->start);
        exit(1);
    }
}

/* Maximum index 2044 to read 4 bytes */
int32_t mload_w(struct core *c, int32_t addr) {
    if (addr >= VIRT_RTN_START && addr <= VIRT_RTN_END) {
        return virt_load(c, addr);
    /* Can load from range [0, 2044] */
    } else if (addr >= INST_MEM_START && addr <= DATA_MEM_END - 3) {
        return c->mem[addr]
            | c->mem[addr + 1] << 8
            | c->mem[addr + 2] << 16
            | c->mem[addr + 3] << 24;
    } else if (get_access(c->start, addr - HEAP_START) != NULL) {
        return c->heap[addr - HEAP_START]
            | c->heap[addr - HEAP_START + 1] << 8
            | c->heap[addr - HEAP_START + 2] << 16
            | c->heap[addr - HEAP_START + 3] << 24;
    } else {
        illegal_access_errmsg(c);
        list_free(c->start);
        exit(1);
    }
}

/* Maximum index 2047 to store 1 byte 
 * Instruction Memory is read only. Writing into INST_MEM is illegal. 
 */
void mstore_b(struct core *c, int32_t addr, int8_t val) {
    if (addr >= VIRT_RTN_START && addr <= VIRT_RTN_END) {
        virt_store(c, addr, val);
    /* Can can store in range [1024, 2047] */
    } else if (addr >= DATA_MEM_START && addr <= DATA_MEM_END) {
        c->mem[addr] = (val & 0xff);
    } else if (get_access(c->start, addr - HEAP_START) != NULL) {
        c->heap[addr - HEAP_START] = (val & 0xff);
    } else {
        illegal_access_errmsg(c);
        list_free(c->start);
        exit(1);
    }
}

/* Maximum index 2046 to store 2 bytes 
 * Instruction Memory is read only. Writing into INST_MEM is illegal. 
 */
void mstore_h(struct core *c, int32_t addr, int16_t val) {
    if (addr >= VIRT_RTN_START && addr <= VIRT_RTN_END) {
        virt_store(c, addr, val);
    /* Can can store in range [1024, 2046] */
    } else if (addr >= DATA_MEM_START && addr <= DATA_MEM_END - 1) {
        c->mem[addr] = (val & 0xff);
        c->mem[addr + 1] = (val >> 8) & 0xff;
    } else if (get_access(c->start, addr - HEAP_START) != NULL) {
        c->heap[addr - HEAP_START] = (val & 0xff);
        c->heap[addr -HEAP_START + 1] = (val >> 8) & 0xff;
    } else {
        illegal_access_errmsg(c);
        list_free(c->start);
        exit(1);
    }
}

/* Maximum index 2044 to store 4 bytes 
 * Instruction Memory is read only. Writing into INST_MEM is illegal. 
 */
void mstore_w(struct core *c, int32_t addr, int32_t val) {
    if (addr >= VIRT_RTN_START && addr <= VIRT_RTN_END) {
        virt_store(c, addr, val);
    /* Can can store in range [1024, 2044] */
    } else if (addr >= DATA_MEM_START && addr <= DATA_MEM_END - 3) {
        c->mem[addr] = (val & 0xff);
        c->mem[addr + 1] = (val >> 8) & 0xff;
        c->mem[addr + 2] = (val >> 16) & 0xff;
        c->mem[addr + 3] = (val >> 24) & 0xff;    
    } else if (get_access(c->start, addr - HEAP_START) != NULL) {
        c->heap[addr - HEAP_START] = (val & 0xff);
        c->heap[addr - HEAP_START + 1] = (val >> 8) & 0xff;
        c->heap[addr - HEAP_START + 2] = (val >> 16) & 0xff;
        c->heap[addr - HEAP_START+ 3] = (val >> 24) & 0xff;
    } else {
        illegal_access_errmsg(c);
        list_free(c->start);
        exit(1);
    }
}

/* Decoding Functions */
uint8_t opcode(uint32_t inst) {
    return inst & 0x7f;
}

uint8_t func3(uint32_t inst) {
    return (inst >> 12) & 0x7;
}

uint8_t func7(uint32_t inst) {
    return (inst >> 25) & 0x7f;
}

uint8_t rd(uint32_t inst) {
    return (inst >> 7) & 0x1f;
}

uint8_t rs1(uint32_t inst) {
    return (inst >> 15) & 0x1f;
}

uint8_t rs2(uint32_t inst) {
    return (inst >> 20) & 0x1f;
}

int32_t imm_I(uint32_t inst) {
    inst >>= 20; // [31 : 20] -> [11: 0]
    return sext(inst, 12);
}

int32_t imm_S(uint32_t inst) {
    inst = ((inst & 0xfe000000) >> 20) 
        | ((inst >> 7) & 0x1f); 
    return sext(inst, 12);
}

int32_t imm_SB(uint32_t inst) {
    inst = ((inst & 0x80000000) >> 19)
        | ((inst & 0x80) <<  4) 
        | ((inst & 0x7e000000) >> 20) 
        | ((inst & 0xf00) >> 7); 
    return sext(inst, 13);
}

int32_t imm_U(uint32_t inst) {
    return (inst & 0xfffff000);
}

int32_t imm_UJ(uint32_t inst) {
    inst = ((inst & 0x80000000) >> 11) // [31] -> [20]
        | ((inst & 0xff000)) // [19: 12] -> [19 : 12]
        | ((inst & 0x100000) >> 9) // [20[ -> [11] 
        | ((inst & 0x7fe00000) >> 20); // [30 :21] to [10:1] (Zero at end)
    return sext(inst, 21);
} 

/* Sign Extensions */
int32_t sext(int32_t val, int8_t bitsize) {
    if ((val >> (bitsize - 1)) & 1) { // If MSB is 1, neg.
        val |= (0xffffffff << bitsize);
    }
    return val;
}

/* Arithemtic Functions */
int32_t exec_add(struct core *c, uint32_t inst) {
    int32_t rs1_val = c->reg[rs1(inst)];
    int32_t rs2_val = c->reg[rs2(inst)];
    reg_write(c, rd(inst), rs1_val + rs2_val);
    return INST_SUCCESS;
}

int32_t exec_addi(struct core *c, uint32_t inst) {
    int32_t rs1_val = c->reg[rs1(inst)];
    int32_t imm_val = imm_I(inst);
    reg_write(c, rd(inst), rs1_val + imm_val);
    return INST_SUCCESS;
}

int32_t exec_sub(struct core *c, uint32_t inst) {
    int32_t rs1_val = c->reg[rs1(inst)];
    int32_t rs2_val = c->reg[rs2(inst)];
    reg_write(c, rd(inst), rs1_val - rs2_val);
    return INST_SUCCESS;
}

int32_t exec_lui(struct core *c, uint32_t inst) {
    int32_t imm_val = imm_U(inst);
    reg_write(c, rd(inst), imm_val);
    return INST_SUCCESS;
}

int32_t exec_xor(struct core *c, uint32_t inst) {
    int32_t rs1_val = c->reg[rs1(inst)];
    int32_t rs2_val = c->reg[rs2(inst)];
    reg_write(c, rd(inst), rs1_val ^ rs2_val);
    return INST_SUCCESS;
}
int32_t exec_xori(struct core *c, uint32_t inst) {
    int32_t rs1_val = c->reg[rs1(inst)];
    int32_t imm_val = imm_I(inst);
    reg_write(c, rd(inst), rs1_val ^ imm_val);
    return INST_SUCCESS;
}

int32_t exec_or(struct core *c, uint32_t inst) {
    int32_t rs1_val = c->reg[rs1(inst)];
    int32_t rs2_val = c->reg[rs2(inst)];
    reg_write(c, rd(inst), rs1_val | rs2_val);
    return INST_SUCCESS;
}

int32_t exec_ori(struct core *c, uint32_t inst) {
    int32_t rs1_val = c->reg[rs1(inst)];
    int32_t imm_val = imm_I(inst);
    reg_write(c, rd(inst), rs1_val | imm_val);
    return INST_SUCCESS;
}

int32_t exec_and(struct core *c, uint32_t inst) {
    int32_t rs1_val = c->reg[rs1(inst)];
    int32_t rs2_val = c->reg[rs2(inst)];
    reg_write(c, rd(inst), rs1_val & rs2_val);
    return INST_SUCCESS;
}

int32_t exec_andi(struct core *c, uint32_t inst) {
    int32_t rs1_val = c->reg[rs1(inst)];
    int32_t imm_val = imm_I(inst);
    reg_write(c, rd(inst), rs1_val & imm_val);
    return INST_SUCCESS;
}

int32_t exec_sll(struct core *c, uint32_t inst) {
    uint32_t rs1_val = c->reg[rs1(inst)];
    uint32_t rs2_val = c->reg[rs2(inst)];
    reg_write(c, rd(inst), rs1_val << rs2_val);
    return INST_SUCCESS;
}

int32_t exec_srl(struct core *c, uint32_t inst) {
    uint32_t rs1_val = c->reg[rs1(inst)];
    uint32_t rs2_val = c->reg[rs2(inst)];
    reg_write(c, rd(inst), rs1_val >> rs2_val);
    return INST_SUCCESS;
}

int32_t exec_sra(struct core *c, uint32_t inst) {
    int32_t rs1_val = c->reg[rs1(inst)];
    int32_t rs2_val = c->reg[rs2(inst)];
    int32_t tmp = (rs1_val >> rs2_val) | (rs1_val << (INT_BITS - rs2_val));
    reg_write(c, rd(inst), tmp);
    return INST_SUCCESS;
}

/* Memory Access Operations */
int32_t exec_lb(struct core *c, uint32_t inst) {
    int32_t rs1_val = c->reg[rs1(inst)];
    int32_t imm_val = imm_I(inst);
    int8_t tmp = mload_b(c, rs1_val + imm_val);
    reg_write(c, rd(inst), sext(tmp, 8));
    return INST_SUCCESS;
}

int32_t exec_lh(struct core *c, uint32_t inst) {
    int32_t rs1_val = c->reg[rs1(inst)];
    int32_t imm_val = imm_I(inst);
    int16_t tmp = mload_h(c, rs1_val + imm_val); 
    reg_write(c, rd(inst), sext(tmp, 16)); 
    return INST_SUCCESS;
}

int32_t exec_lw(struct core *c, uint32_t inst) {
    int32_t rs1_val = c->reg[rs1(inst)];
    int32_t imm_val = imm_I(inst);
    int32_t tmp = mload_w(c, rs1_val + imm_val);
    reg_write(c, rd(inst), tmp);
    return INST_SUCCESS;
}

int32_t exec_lbu(struct core *c, uint32_t inst) {
    int32_t rs1_val = c->reg[rs1(inst)];
    int32_t imm_val = imm_I(inst);
    uint8_t tmp = mload_b(c, rs1_val + imm_val); // Based on spec back to  uint8_t?
    reg_write(c, rd(inst), tmp & 0xff); // (int32_t) tmp
    return INST_SUCCESS;
}

int32_t exec_lhu(struct core *c, uint32_t inst) {
    int32_t rs1_val = c->reg[rs1(inst)];
    int32_t imm_val = imm_I(inst);
    uint16_t tmp = mload_h(c, rs1_val + imm_val); // Based on spec change back to  uint16_t?
    reg_write(c, rd(inst), tmp & 0xffff);  // (int32_t) tmp
    return INST_SUCCESS;
}

int32_t exec_sb(struct core *c, uint32_t inst) {
    int32_t rs1_val = c->reg[rs1(inst)];
    int8_t rs2_val = c->reg[rs2(inst)] & 0xff; // Store only 8 bits
    int32_t imm_val = imm_S(inst);
    mstore_b(c, rs1_val + imm_val, rs2_val);
    return INST_SUCCESS;
}

int32_t exec_sh(struct core *c, uint32_t inst) {
    int32_t rs1_val = c->reg[rs1(inst)];
    int16_t rs2_val = c->reg[rs2(inst)] & 0xffff; // Store only 16 bits
    int32_t imm_val = imm_S(inst);
    mstore_h(c, rs1_val + imm_val, rs2_val);
    return INST_SUCCESS;
}

int32_t exec_sw(struct core *c, uint32_t inst) {
    int32_t rs1_val = c->reg[rs1(inst)];
    int32_t rs2_val = c->reg[rs2(inst)];
    int32_t imm_val = imm_S(inst);
    mstore_w(c, rs1_val + imm_val, rs2_val);
    return INST_SUCCESS;
}

/* Program Flow Operations */
int32_t exec_slt(struct core *c, uint32_t inst) {
    int32_t rs1_val = c->reg[rs1(inst)];
    int32_t rs2_val = c->reg[rs2(inst)];
    reg_write(c, rd(inst), (rs1_val < rs2_val) ? 1 : 0);
    return INST_SUCCESS;
}

int32_t exec_slti(struct core *c, uint32_t inst) {
    int32_t rs1_val = c->reg[rs1(inst)];
    int32_t imm_val = imm_I(inst);
    reg_write(c, rd(inst), (rs1_val < imm_val) ? 1 : 0);
    return INST_SUCCESS;
}

int32_t exec_sltu(struct core *c, uint32_t inst) {
    uint32_t rs1_val = c->reg[rs1(inst)]; // Based on spec
    uint32_t rs2_val = c->reg[rs2(inst)];
    reg_write(c, rd(inst), (rs1_val < rs2_val) ? 1 : 0);
    return INST_SUCCESS;
}

int32_t exec_sltiu(struct core *c, uint32_t inst) {
    uint32_t rs1_val = c->reg[rs1(inst)];
    uint32_t imm_val = imm_I(inst); // According to spec 
    reg_write(c, rd(inst), (rs1_val < imm_val) ? 1 : 0);
    return INST_SUCCESS;
}

int32_t exec_beq(struct core *c, uint32_t inst) {
    int32_t rs1_val = c->reg[rs1(inst)];
    int32_t rs2_val = c->reg[rs2(inst)];
    if (rs1_val == rs2_val) {
        c->pc += imm_SB(inst);
        return INST_JUMP;
    }
    return INST_SUCCESS;
}

int32_t exec_bne(struct core *c, uint32_t inst) {
    int32_t rs1_val = c->reg[rs1(inst)];
    int32_t rs2_val = c->reg[rs2(inst)];
    if (rs1_val != rs2_val) {
        c->pc += imm_SB(inst);
        return INST_JUMP;
    }
    return INST_SUCCESS;
}

int32_t exec_blt(struct core *c, uint32_t inst) {
    int32_t rs1_val = c->reg[rs1(inst)];
    int32_t rs2_val = c->reg[rs2(inst)];
    if (rs1_val < rs2_val) {
        c->pc += imm_SB(inst);
        return INST_JUMP;
    }
    return INST_SUCCESS;
}

int32_t exec_bltu(struct core *c, uint32_t inst) {
    uint32_t rs1_val = c->reg[rs1(inst)];
    uint32_t rs2_val = c->reg[rs2(inst)]; // According to spec, just treat as unsigned value.
    if (rs1_val < rs2_val) {
        c->pc += imm_SB(inst);
        return INST_JUMP;
    }
    return INST_SUCCESS;
}

int32_t exec_bge(struct core *c, uint32_t inst) {
    int32_t rs1_val = c->reg[rs1(inst)];
    int32_t rs2_val = c->reg[rs2(inst)]; 
    if (rs1_val >= rs2_val) {
        c->pc += imm_SB(inst);
        return INST_JUMP;
    }
    return INST_SUCCESS;
}

int32_t exec_bgeu(struct core *c, uint32_t inst) {
    uint32_t rs1_val = c->reg[rs1(inst)];
    uint32_t rs2_val = c->reg[rs2(inst)]; // According to spec, just treat as unsigned value.
    if (rs1_val >= rs2_val) {
        c->pc += imm_SB(inst);
        return INST_JUMP;
    }
    return INST_SUCCESS;
}

int32_t exec_jal(struct core *c, uint32_t inst) {
    reg_write(c, rd(inst), c->pc + 4);
    c->pc += imm_UJ(inst);
    return INST_JUMP;
}

int32_t exec_jalr(struct core *c, uint32_t inst) {
    reg_write(c, rd(inst), c->pc + 4);
    c->pc = c->reg[rs1(inst)] + imm_I(inst);
    return INST_JUMP;
}

/* Virt Reg */
int32_t get_int(struct core *c) {
    int32_t x = 0;

    /* Read integer, clearing buffer if it fails */
    int32_t rc;
    while ((rc = scanf("%d", &x)) == 0) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }

    return x;
}

void reg_dump(struct core *c) {
    for (int32_t i = 0; i < 32; i++) {
        printf("R[%d] = 0x%08x;\n", i, c->reg[i]);
    }
}

void pc_dump(struct core *c) {
    printf("PC = 0x%08x;\n", c->pc);
}

int32_t virt_load(struct core *c, int32_t addr) {
    switch (addr) {
        case 0x812:
            return fgetc(stdin);
        case 0x816:
            return get_int(c);
        default:
            printf("PC: %x, Unknown virtual register read addr 0x%x\n", c->pc, addr);
            list_free(c->start);
            exit(1);
    }
}

void virt_store(struct core *c, int32_t addr, int32_t val) {
    switch (addr) {
        case 0x800:
            putchar(val);
            break;
        case 0x804:
            printf("%d", val);
            break;
        case 0x808:
            printf("%x", (uint32_t) val); // Promotion Casting
            break;
        case 0x080c:
            printf("CPU Halt Requested\n");
            list_free(c->start);
            exit(0);
            break;
        case 0x820:
            pc_dump(c);
            break;
        case 0x824:
            pc_dump(c);
            reg_dump(c);
            break;
        case 0x828:
            printf("0x%08x;\n", val);
            break;
        case 0x830: // malloc
            reg_write(c, 28 , list_add(c->start, val));
            break;
        case 0x834: // free
            if (get_free(c->start, val - HEAP_START) == NULL) {
                illegal_access_errmsg(c);
                list_free(c->start);
                exit(1);
            }
            list_delete(c->start, val - HEAP_START);
            break;
        default:
            printf("PC: %x, Uknown virtual register write addr 0x%x\n", c->pc, addr);
            list_free(c->start);
            exit(1);
    }
}

/* Errors */
void illegal_access_errmsg(struct core *c) {
    printf("Illegal Operation: 0x%08x\n", fetch(c));
    pc_dump(c);
    reg_dump(c);
}