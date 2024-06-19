inline void my_free(int tfa) { // to free address
    int result;
    int addr = 0x0834;
    
    // Call free(), free address.
    // Store word, where size is the value of alloc size. (tf -> M[0 + addr])
    asm volatile("sw %[tf], 0(%[adr])": [tf]"=r"(tfa) : [adr]"r"(addr));
}

int main() {
    // Some reason, accessing heap is not working?
    int heap_start = 0xb700;
    my_free(heap_start);

    return 0;
}