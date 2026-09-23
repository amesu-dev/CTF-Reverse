#include "math.h"
#include "matrix.h"


unsigned int hash (unsigned char* data, int length) {
    unsigned int result = 0;
    
    for (int index = 0; index < length; index += 1) {
        result ^= (unsigned int)data[index];
        
        for (int bit = 0; bit < 8; bit += 1) {
            unsigned int prev = result;
            result >>= 1;
            if ((prev & 1) == 1) result ^= 0xEDB88320;
        }
    }
    
    return result;
}

struct solve_space* solve_space_create(int rank, int free) {
    struct solve_space* space = malloc(sizeof(struct solve_space));
    space->part = malloc(sizeof(uint8_t) * rank);
    space->pivot_cols = malloc(sizeof(uint8_t) * rank);
    space->nullspace = create_matrix(free, rank + free);

    space->pivot_num = rank;
    space->free_num = free;

    return space;
}
void solve_space_free(struct solve_space* space) {
    free(space->part);
    free(space->pivot_cols);
    matrix_free(space->nullspace);
    free(space);
}

unsigned int solve_linear_system(int target_hash, int cols_count) {
    struct matrix* matrix = create_matrix(32, cols_count + 1);
    
    // Filling matrix
    unsigned char dummy[16] = {0};
    for (int col = 0; col < matrix->cols - 1; col += 1) {
        memset(dummy, 0, sizeof(dummy));
        dummy[col / 8] = 1 << col % 8;
        
        unsigned int res = hash(dummy, 16);
        
        for (int bit = 0; bit < matrix->rows; bit += 1) {
            if (res & (1 << bit)) matrix_set_bit(matrix, matrix->rows - bit - 1, col);
        }
    }

    // unsigned int target_hash = 0xAB8140DE;
    for (int bit = 0; bit < 32; bit += 1) {
        if ((target_hash >> bit) & 1) matrix_set_bit(matrix, 31 - bit, 128);
    }

    int rank = 0;
    int* pivot_cols = (int*)malloc(matrix->rows * sizeof(int));

    for (int col = 0; col < matrix->cols; col += 1) {
        int pivot = -1;
        for (int row = rank; row < matrix->rows; row += 1) {
            if (matrix_get_bit(matrix, row, col)) { pivot = row; break; }
        }

        // Not found
        if (pivot == -1) continue;

        // Swap rows
        int res = matrix_swap_rows(matrix, rank, pivot);
        if (res) {
            printf("Swap failed %d %d %d\n", res, rank, pivot);
            exit(1);
        }

        // Eliminate
        for (int row = 0; row < 128; row += 1) {
            if (row != rank && matrix_get_bit(matrix, row, col))
                matrix_xor_rows(matrix, row, rank);
        }

        pivot_cols[rank] = col;
        rank += 1;
        if (rank >= 128) break;
    }

    // Validate matrix
    for (int row = rank; row < matrix->rows; row += 1) {
        int has_nonzero = 0;
        for (int col = 0; col < matrix->cols; col += 1) {
            if (matrix_get_bit(matrix, row, col) != 1) continue;
            has_nonzero = 1;
            break;
        }

        if(!has_nonzero && matrix_get_bit(matrix, row, matrix->cols - 1)) {
            matrix_free(matrix);
            free(pivot_cols);
            printf("There are no solution\n");
            exit(0);
        }
    }

    struct solve_space* space = solve_space_create(rank, matrix->cols - rank - 1);
    memcpy(space->pivot_cols, pivot_cols, rank * sizeof(uint32_t));
    
    free(pivot_cols);
    
    // Solution space
    for (int row = 0; row < rank; row += 1) {
        int col = pivot_cols[row];
        int val = matrix_get_bit(matrix, row, matrix->cols);
        if (val == 1) space->part[col / 8] |= 1 << col % 8;
    }

    int* is_pivot = (int*)calloc(matrix->cols, sizeof(int));
    for (int i = 0; i < rank; i++) {
        is_pivot[pivot_cols[i]] = 1;
    }
    
    int num_free = 0;
    int* free_cols = (int*)malloc(matrix->cols * sizeof(int));
    for (int col = 0; col < matrix->cols; col++) {
        if (is_pivot[col]) continue;

        matrix_set_bit(space->nullspace, num_free, col);
        
        // Выражаем главные переменные через свободные
        for (int r = 0; r < rank; r++) {
            int pivot_col = pivot_cols[r];
            // Ищем в строке r столбец free_col
            if (matrix_get_bit(matrix, r, col)) {
                matrix_set_bit(space->nullspace, num_free, col);
            }
        }

        num_free += 1;
    }

    print_matrix(matrix);
    printf("------------\n");
    print_matrix(space->nullspace);
    printf("------------\n");
    printf("Solution part: %s (0x%016X)\n", (char*)space->part, *(uint64_t*)space->part);

    generate_messages(target_hash, space);

    matrix_free(matrix);
    return 0;
}

unsigned int generate_messages(int target_hash, struct solve_space* space) {
    printf("Total variables: %d\n", space->pivot_num + space->free_num);
    printf("Free variables: %d (=approx= %d solutions)\n", space->free_num, (unsigned long long)1 << space->free_num);

    
}
