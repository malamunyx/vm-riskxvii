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
    int min = 0x0fffffff; // MAX INT
    int max = 0xffffffff; // MIN INT

    prints("How many numbers is your data? ");
    int qty = scan_int();
    int sum = 0;
    int num;


    if (qty <= 0) {
        prints("Error: qty must be >= 1\n");
        return 0;
    }

    int i = 0;
    while (i++ < qty) {
        num = scan_int();
        if (num > max) {
            max = num;
        }
        if (num < min) {
            min = num;
            
        }
        sum += num;
    }

    prints("min: ");
    *ConsoleWriteSInt = min;
    prints("\n");

    prints("max: ");
    *ConsoleWriteSInt = max;
    prints("\n");

    prints("sum: ");
    *ConsoleWriteSInt = sum;
    prints("\n");

    return 0;
}