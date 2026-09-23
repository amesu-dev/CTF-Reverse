#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


// data using align(1)
struct matrix {
    long rows;
    long cols;
    uint8_t* data;
};

inline long matrix_get_row_offset(long row, long cols);
inline struct matrix* create_matrix(long rows, long cols);
inline void matrix_free(struct matrix* m);

inline int matrix_set_bit(struct matrix* m, long row, long col);
inline int matrix_get_bit(struct matrix* m,int row, int col);
inline int matrix_xor_rows(struct matrix* m, long target, long source);
inline int matrix_swap_rows(struct matrix* m, long target, long source);
struct matrix* matrix_mul(struct matrix* m1, struct matrix* m2);

void print_matrix(const struct matrix* m);