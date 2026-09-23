#pragma once
#include <stdint.h>

struct solve_space {
    int pivot_num;
    int free_num;
    uint32_t* pivot_cols;
    uint8_t* part;
    struct matrix* nullspace;
};

unsigned int hash (unsigned char* data, int length);
struct solve_space* solve_space_create(int rank, int free);
void solve_space_free(struct solve_space* space);

unsigned int solve_linear_system(int target_hash, int cols_count);
unsigned int generate_messages(int target_hash, struct solve_space* space);