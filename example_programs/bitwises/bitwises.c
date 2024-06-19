// Taken from scaffold
inline int scan_int() {
    int result;
    int addr = 0x0816;
    
    asm volatile("lw %[res], 0(%[adr])": [res]"=r"(result): [adr]"r"(addr));

    return result;
}

void prints(char *str){
    while(*str) *((char *)0x0800) = *(str++);
}

inline int lshift(int num, int cnt){
    int result;
    asm volatile("sll %[res], %[num], %[nbits]"
        : [res]"=r"(result)
        : [nbits]"r"(cnt), [num]"r"(num));
    return result;
}

inline int rshift(int num, int cnt){
    int result;
    asm volatile("srl %[res], %[num], %[nbits]"
        : [res]"=r"(result)
        : [nbits]"r"(cnt), [num]"r"(num));
    return result;
}

int volatile *const ConsoleWriteSInt = (int *)0x0804;
int volatile *const ConsoleWriteHex = (int *)0x0808;

// End from Scaffold

inline int bit_and(int n1, int n2){
    int result;
    asm volatile("and %[res], %[num], %[nbits]"
        : [res]"=r"(result)
        : [nbits]"r"(n1), [num]"r"(n2));
    return result;
}

inline int bit_or(int n1, int n2){
    int result;
    asm volatile("or %[res], %[num], %[nbits]"
        : [res]"=r"(result)
        : [nbits]"r"(n1), [num]"r"(n2));
    return result;
}

inline int bit_xor(int n1, int n2){
    int result;
    asm volatile("xor %[res], %[num], %[nbits]"
        : [res]"=r"(result)
        : [nbits]"r"(n1), [num]"r"(n2));
    return result;
}



int main(void) {
    prints("Enter n1: ");
    int n1 = scan_int();
    prints("Enter n2: ");
    int n2 = scan_int();

    prints("[");
    *ConsoleWriteSInt = n1;
    prints(" (0x");
    *ConsoleWriteHex = n1;
    prints("), ");

    *ConsoleWriteSInt = n2;
    prints(" (0x");
    *ConsoleWriteHex = n2;
    prints(")]\n");

    *ConsoleWriteSInt = n1;
    prints(" & ");
    *ConsoleWriteSInt = n2;
    prints(": ");
    *ConsoleWriteSInt = bit_and(n1, n2);
    prints(" (0x");
    *ConsoleWriteHex = bit_and(n1, n2);
    prints(")\n");

    *ConsoleWriteSInt = n1;
    prints(" | ");
    *ConsoleWriteSInt = n2;
    prints(": ");
    *ConsoleWriteSInt = bit_or(n1, n2);
    prints(" (0x");
    *ConsoleWriteHex = bit_or(n1, n2);
    prints(")\n");

    *ConsoleWriteSInt = n1;
    prints(" ^ ");
    *ConsoleWriteSInt = n2;
    prints(": ");
    *ConsoleWriteSInt = bit_xor(n1, n2);
    prints(" (0x");
    *ConsoleWriteHex = bit_xor(n1, n2);
    prints(")\n");

    *ConsoleWriteSInt = n1;
    prints(" >> ");
    *ConsoleWriteSInt = n2;
    prints(": ");
    *ConsoleWriteSInt = rshift(n1, n2);
    prints(" (0x");
    *ConsoleWriteHex = rshift(n1, n2);
    prints(")\n");

    *ConsoleWriteSInt = n1;
    prints(" << ");
    *ConsoleWriteSInt = n2;
    prints(": ");
    *ConsoleWriteSInt = lshift(n1, n2);
    prints(" (0x");
    *ConsoleWriteHex = lshift(n1, n2);
    prints(")\n");

    return 0;
}