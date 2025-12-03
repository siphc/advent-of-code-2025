#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "parse_input.h"

static int count_tokens(const char *line, char delimiter) {
    int count = 0;
    int in_token = 0;

    for (; *line; line++) {
        if (*line == delimiter) {
            in_token = 0;
        }
        else if (!in_token) {
            in_token = 1;
            count++;
        }
    }
    return count;
}

static int get_dimensions(FILE *f, int *rows, int *cols, char delimiter) {
    char *line = NULL;
    size_t cap = 0;

    *rows = 0;
    *cols = 0;

    /*
        Q: Why the double pointer for **line?
        A: getline() needs to modify *line, you silly.

        We also don't really use cap, but just know that it stores the
        allocated size of *line.
    */
    while (getline(&line, &cap, f) != -1) {
        int tokens = count_tokens(line, delimiter);
        if (tokens == 0) continue;

        if (tokens > *cols) *cols = tokens;
        (*rows)++;
    }

    /*
        We call getline() at least once.
        POSIX specifies that we must free the buffer, even if getline() failed.
    */
    free(line);
    return 0;
}

static char **parse_row(const char *line, int cols, char delimiter) {
    char **values = malloc(cols * sizeof(char *));
    if (!values) return NULL;

    int col = 0;
    const char *p = line;

    while (*p && col < cols) {
        while (*p && *p == delimiter) p++;
        if (!*p) break;

        const char *start = p;
        while (*p && *p != delimiter) p++;
        size_t len = p - start;
        values[col] = malloc(len + 1);
        if (!values[col]) goto fail;
        
        memcpy(values[col], start, len);
        values[col][len] = '\0';
        col++;
    }

    // Pad remaining columns with empty strings
    while (col < cols) {
        values[col] = calloc(1, 1);  // empty string
        if (!values[col]) goto fail;
        col++;
    }

    return values;

fail:
    for (int i = 0; i < col; i++)
        free(values[i]);
    free(values);
    return NULL;
}

static int read_data(FILE *f, Matrix *m, char delimiter) {
    char *line = NULL;
    size_t cap = 0;
    int row = 0;

    while (getline(&line, &cap, f) != -1) {
        // removes trailing LF character.
        // getline() is POSIX anyway, so this code is valid.
        if (line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0';
        m->values[row] = parse_row(line, m->cols, delimiter);
        if (!m->values[row]) { // we must have failed an allocation...
            free(line);
            return 1;
        }
        row++;
    }

    free(line);
    return 0;
}

/* Recursively frees, in the following order:
    each string
    each row
    the matrix
    the Matrix (since we malloc'd the struct for object lifetime purposes)
*/
void matrix_free(Matrix *m) {
    if (!m) return;

    for (int i = 0; i < m->rows; i++) {
        if (m->values[i]) {
            for (int j = 0; j < m->cols; j++) {
                free(m->values[i][j]);
            }
            free(m->values[i]);
        }
    }
    free(m->values);
    free(m);
}

Matrix *matrix_parse(const char *filename, char delimiter) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror(filename);
        return NULL;
    }

    int rows, cols;
    if (get_dimensions(f, &rows, &cols, delimiter) != 0 || rows == 0) {
        fclose(f);
        return NULL;
    }

    Matrix *m = malloc(sizeof(Matrix));
    if (!m) {
        fclose(f);
        return NULL;
    }

    m->rows = rows;
    m->cols = cols;
    m->values = calloc(rows, sizeof(char **));
    if (!m->values) {
        free(m);
        fclose(f);
        return NULL;
    }

    rewind(f);

    if (read_data(f, m, delimiter) != 0) {
        matrix_free(m);
        fclose(f);
        return NULL;
    }

    fclose(f);
    return m;
}

int matrix_rows(const Matrix *m) {
    return m->rows;
}

int matrix_cols(const Matrix *m) {
    return m->cols;
}

const char *matrix_get(const Matrix *m, int row, int col) {
    if (row < 0 || row >= m->rows) return NULL;
    if (col < 0 || col >= m->cols) return NULL;
    return m->values[row][col];
}

/* int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }

    Matrix *m = matrix_parse(argv[1]);
    if (!m) return 1;

    printf("Parsed %d x %d matrix:\n\n", matrix_rows(m), matrix_cols(m));

    for (int i = 0; i < matrix_rows(m); i++) {
        for (int j = 0; j < matrix_cols(m); j++) {
            const char *val = matrix_get(m, i, j);
            printf("[%s] ", val);
        }
        printf("\n");
    }

    matrix_free(m);
    return 0;
}
 */