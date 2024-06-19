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

int volatile *const ConsoleWriteSInt = (int *)0x0804;
// End from Scaffold


int cumulative_sum(int num) {
    int sum = 0;
    int i = 0;
    while (i++ < num) {
        sum += scan_int();
    }
    return sum;
}

int main(void) {
    prints("How many numbers? ");
    int n = scan_int();

    if (n <= 0) {
        prints("Must have >= 1 numbers\n");
        return 0;
    }

    int sum = cumulative_sum(n);

    prints("Cumulative sum: ");
    *ConsoleWriteSInt = sum;
    prints("\n");

    return 0;
}