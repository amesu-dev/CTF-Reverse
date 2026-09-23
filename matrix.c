#include "matrix.h"



inline long matrix_get_row_offset(long row, long cols) {
    // div computes divition and module, so no need to change on ">> 4"
    return row * (cols / 8 + !!(cols % 8));
}

inline struct matrix* create_matrix(long rows, long cols) {
    // align(8)
    uint8_t* data = (uint8_t*)calloc(
        matrix_get_row_offset(rows, cols), 1
    );
    
    struct matrix* m = malloc(sizeof(struct matrix));
    m->rows = rows;
    m->cols = cols;
    m->data = data;
    return m;
}

inline int matrix_set_bit(struct matrix* m, long row, long col) {
    if (row >= m->rows || col >= m->cols) return -1;
    long byte_pos = matrix_get_row_offset(row, m->cols);

    m->data[byte_pos + (col / 8)] |= 1 << col % 8;
    return 0;
}

inline int matrix_get_bit(struct matrix* m,int row, int col) {
    if (row >= m->rows || col >= m->cols) return -1;
    long byte_pos = matrix_get_row_offset(row, m->cols);

    return (m->data[byte_pos + (col / 8)] >> (col % 8)) & 1;
}

inline int matrix_xor_rows(struct matrix* m, long target, long source) {
    if (target >= m->rows || source >= m->rows) return -1;

    long start_target = matrix_get_row_offset(target, m->cols);
    long start_source = matrix_get_row_offset(source, m->cols);

    long bytes_in_line = m->cols / 8 + !!(m->cols % 8);
    for (register long byte = 0; byte < bytes_in_line; byte += 1)
        m->data[start_target + byte] ^= m->data[start_source + byte];

    return 0;
}

inline int matrix_swap_rows(struct matrix* m, long target, long source) {
    if (target >= m->rows || source >= m->rows) return -1;

    long start_target = matrix_get_row_offset(target, m->cols);
    long start_source = matrix_get_row_offset(source, m->cols);

    long length = m->cols / 8 + !!(m->cols % 8);
    uint8_t* row = (uint8_t*)malloc(length);
    if (!row) return -1;

    memcpy(row, m->data + start_target, length);

    memcpy(m->data + start_target,  m->data + start_source, length);
    memcpy(m->data + start_source, row, length);

    free(row);
    return 0;
}

inline void matrix_free(struct matrix* m) {
    free(m->data);
    free(m);
}

void print_matrix(const struct matrix* m) {
    const bit_step = 8;
    for (long r = 0; r < m->rows; r += 1) {
        for (long col = 0; col < m->cols; col += bit_step) {
            // for (int bit = 7; bit >= 0; bit -=1) printf("%d", dummy[col] >> bit & 1);
            for (int bit = 0; bit < bit_step; bit +=1) {
                int res = matrix_get_bit(m, r, col + bit);
                if (res >= 0) printf("%d", res);
            }
            printf(" ");
        }
        printf("\n");
    }
}

struct matrix* matrix_mul(struct matrix* m1, struct matrix* m2) {
    if (m1->cols != m2->rows) return 0;

    struct matrix* res = matrix_create(m1->rows, m2->cols);
    for (int row = 0; row < res->rows; row += 1) {
        for (int row = 0; row < res->rows; row += 1) {
            int val = 0;

            for(int r = 0; r < m1->rows; r += 1) {
                for (int c = 0; c < m2->cols; c += 1)
                    val |= matrix_get_bit(m1, r, c) & matrix_get_bit(m2, r, c);
            }

            if (val) matrix_set_bit(res, row, col);
        }
    }

    return res;
}
