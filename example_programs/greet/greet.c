/* Taken from the scaffold */
void prints(char *str){
    while(*str) *((char *)0x0800) = *(str++);
}
/* Taken from the scaffold */
inline int scan_char() {
    int result;
    int addr = 0x0812;
    
    asm volatile("lw %[res], 0(%[adr])": [res]"=r"(result): [adr]"r"(addr));

    return result;
}

/* "homebrew" fgets function */
void get_str(char *buf, int buflen) {
    int i = 0;
    while (i < buflen - 1) {
        buf[i] = scan_char();
        if (buf[i] == 0 || buf[i] == '\n') {
            break;
        }
        i++;
    }
}

/* Flush stdin buffer 
 * If character string contains newline, 
 * the whole input has been stored.
 */
void flush_stdin() {
    while (scan_char() != '\n');
}
/* Check if it contains a character */
int contains_c(char *str, char c) {
    while (*str) {
        if (*str == '\n') {
            return 1;
        }
        str++;
    }
    return 0;
}

int main() {

    prints("What's your name? ");

    char name[16];
    get_str(name, 16);

    prints("Hello, ");
    prints(name);

    if (!contains_c(name, '\n')) {
        flush_stdin();
    }
    
    return 0;
}