// Example program
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "matrix.h"
#include "math.h"



int main() {
    long long a = 0x10000000;
    long long b = 0x20000000;
    long long c = 0x30000000;
    printf("0x10000000 -> 0x%08X\n", hash((unsigned char*)&a, 8));
    printf("0x20000000 -> 0x%08X\n", hash((unsigned char*)&b, 8));
    printf("0x30000000 -> 0x%08X\n", hash((unsigned char*)&c, 8));
    printf("a ^ b      -> 0x%08X\n", hash((unsigned char*)&a, 8) ^ hash((unsigned char*)&b, 8));


    const unsigned int target_hash = 0xAB8140DE; 
    unsigned char solution[16] = {0, 0, 0, 0, 0x10, 0x10, 'A', 'M', 'E', 'S', 'U'};
    solve_linear_system(target_hash, 128);

    return 0;
}