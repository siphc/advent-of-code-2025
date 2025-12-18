#ifndef PARSE_INPUT_H
#define PARSE_INPUT_H

typedef struct {
    char ***values;
    int rows;
    int cols;
} Matrix;

Matrix *matrix_parse(const char *filename, char delimiter);
int matrix_rows(const Matrix *m);
int matrix_cols(const Matrix *m);
const char *matrix_get(const Matrix *m, int row, int col);
void matrix_free(Matrix *m);

#endif