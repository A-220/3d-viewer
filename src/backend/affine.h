#ifndef INC_3D_AFFINE_H
#include "parser.h"
#include <math.h>

/**
 * @brief Moves the model along the X-axis by a specified value.
 * @param A [in] Pointer to the matrix containing the vertices of the model.
 * @param a [in] Value to be added to the X-coordinate of each vertex.
 * @return `OK` if the move is successful, `ERROR` otherwise.
 */
uint8_t move_x(matrix_t *A, double a);

/**
 * @brief Moves the model along the Y-axis by a specified value.
 * @param A [in] Pointer to the matrix containing the vertices of the model.
 * @param b [in] Value to be added to the Y-coordinate of each vertex.
 * @return `OK` if the move is successful, `ERROR` otherwise.
 */
uint8_t move_y(matrix_t *A, double b);

/**
 * @brief Moves the model along the Z-axis by a specified value.
 * @param A [in] Pointer to the matrix containing the vertices of the model.
 * @param c [in[ Value to be added to the Z-coordinate of each vertex.
 * @return `OK` if the move is successful, `ERROR` otherwise.
 */
uint8_t move_z(matrix_t *A, double c);

/**
 * @brief Rotates the model around the Y-axis by a specified angle.
 * @param A [in] Pointer to the matrix containing the vertices of the model.
 * @param angle [in] Angle of rotation in radians.
 * @return `OK` if the rotation is successful, `ERROR` otherwise.
 */
uint8_t rotation_by_oy(matrix_t *A, double angle);

/**
 * @brief Rotates the model around the Z-axis by a specified angle.
 * @param A [in] Pointer to the matrix containing the vertices of the model.
 * @param angle [in] Angle of rotation in radians.
 * @return `OK` if the rotation is successful, `ERROR` otherwise.
 */
uint8_t rotation_by_oz(matrix_t *A, double angle);

/**
 * @brief Rotates the model around the X-axis by a specified angle.
 * @param A [in] Pointer to the matrix containing the vertices of the model.
 * @param angle [in] Angle of rotation in radians.
 * @return `OK` if the rotation is successful, `ERROR` otherwise.
 */
uint8_t rotation_by_ox(matrix_t *A, double angle);

/**
* @brief Scales the matrix by a given value.
* This function multiplies each element of the matrix by the specified scaling factor.
* @param A A pointer to the matrix_t structure representing the matrix.
* @param scale The value of the scaling factor.
 * @return `OK` if the scale is successful, `ERROR` otherwise.
*/
uint8_t scale_model(matrix_t *A, double scale);
#define INC_3D_AFFINE_H

#endif //INC_3D_AFFINE_H
