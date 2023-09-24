#include "affine.h"

uint8_t move_x(matrix_t *A, double a) {
    if (A == NULL) {
        printf("Error: Invalid matrix\n");
        return ERROR;
    }
    for (int i = 0; i < A->rows; i++)
        A->matrix[i][0] += a;
    return OK;
}

uint8_t move_y(matrix_t *A, double b) {
    if (A == NULL) {
        printf("Error: Invalid matrix\n");
        return ERROR;
    }
    for (int i = 0; i < A->rows; i++)
        A->matrix[i][1] += b;
    return OK;
}

uint8_t move_z(matrix_t *A, double c) {
    if (A == NULL) {
        printf("Error: Invalid matrix\n");
        return ERROR;
    }
    for (int i = 0; i < A->rows; i++)
        A->matrix[i][2] += c;
    return OK;
}

uint8_t rotate_oy(matrix_t *A, double angle) {
    if (A == NULL) {
        printf("Error: Invalid matrix\n");
        return ERROR;
    }
    for (int i = 0; i < A->rows; i++) {
        double temp_x = A->matrix[i][0];
        double temp_z = A->matrix[i][2];
        A->matrix[i][0] = cos(angle) * temp_x - sin(angle) * temp_z;
        A->matrix[i][2] = sin(angle) * temp_x + cos(angle) * temp_z;
    }
    return OK;
}

uint8_t rotate_oz(matrix_t *A, double angle) {
    if (A == NULL) {
        printf("Error: Invalid matrix\n");
        return ERROR;
    }
    for (int i = 0; i < A->rows; i++) {
        double temp_x = A->matrix[i][0];
        double temp_y = A->matrix[i][1];
        A->matrix[i][0] = cos(angle) * temp_x + sin(angle) * temp_y;
        A->matrix[i][1] = -sin(angle) * temp_x + cos(angle) * temp_y;
    }
    return OK;
}

uint8_t rotate_ox(matrix_t *A, double angle) {
    if (A == NULL) {
        printf("Error: Invalid matrix\n");
        return ERROR;
    }
    for (int i = 0; i < A->rows; i++) {
        double temp_y = A->matrix[i][1];
        double temp_z = A->matrix[i][2];
        A->matrix[i][1] = cos(angle) * temp_y + sin(angle) * temp_z;
        A->matrix[i][2] = -sin(angle) * temp_y + cos(angle) * temp_z;
    }
    return OK;
}

uint8_t scale_model(matrix_t *A, double scale) {
    if (A == NULL) {
        printf("Error: Invalid matrix\n");
        return ERROR;
    }
    if (scale == 0) {
        printf("Error: Scale cannot be zero\n");
        return ERROR;
    }
    for(int i = 0; i < A->rows; i++) {
        for(int j = 0; j < A->columns; j++) {
            A->matrix[i][j] *= scale;
        }
    }
    return OK;
}