//
// Created by Kai Evergarden on 24.09.2023.
//

#ifndef INC_3D_PARSER_H
#define INC_3D_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FEEL_STATUS 0
#define FEEL_DATA 1
#define VERTEX 'v'
#define FACET 'f'

enum {
    OK, ERROR
};

typedef unsigned char byte;

/**
 * @file backend_parser.h
 * @brief This file contains the declarations for the backend parser.
 */

/**
 * @struct vertex_t
 * @brief Structure representing the coordinates of a point.
 */
typedef struct {
    double x; /**< The x coordinate. */
    double y; /**< The y coordinate. */
    double z; /**< The z coordinate. */
} vertex_t;

/**
 * @struct Vertex_array
 * @brief Structure representing an array of vertices.
 */
typedef struct {
    vertex_t *vertex; /**< The array of vertices. */
    unsigned long long int vertex_length; /**< The length of the vertex array. */
} Vertex_array;

/**
 * @struct Index_array
 * @brief Structure representing an array of triangle indices.
 */
typedef struct {
    unsigned int *facet; /**< The array of triangle indices. */
    unsigned long long int facet_length; /**< The length of the facet array. */
} Index_array;

/**
 * @struct matrix_t
 * @brief Structure representing a matrix.
 */
typedef struct {
    double **matrix; /**< The matrix. */
    unsigned long long int rows; /**< The number of rows in the matrix. */
    unsigned long long int columns; /**< The number of columns in the matrix. */
} matrix_t;

/**
 * @struct Status
 * @brief Structure representing information about the obj file.
 */
typedef struct {
    unsigned long long int vertexes; /**< Number of vertices in the obj file. */
    unsigned long long int facets; /**< Number of facets in the obj file. */
    unsigned long long int edges; /**< Number of edges in the obj file. (vertexes + facets - 2) */
} Status;

/**
 * @struct data_t
 * @brief Structure representing the data containing vertex array, index array, matrix, and status information.
 */
typedef struct {
    Vertex_array vertex_array; /**< The vertex array. */
    Index_array index_array; /**< The index array. */
    matrix_t matrix; /**< The matrix. */
    Status status; /**< Status information about the obj file. */
} data_t;

/**
 * @brief Checks if the specified file is in the .obj format.
 *
 * @param file_name The file name.
 * @return `OK` if the check is successful, `ERROR` otherwise.
 */
byte file_checker(const char *file_name);

/**
 * @brief Reads data from the specified file and performs the appropriate actions based on the type.
 *
 * @param file_name The file name.
 * @param type The type of operation.
 * @param data A pointer to the `data_t` structure where the data will be written.
 * @return `OK` if the reading is successful, `ERROR` otherwise.
 */
byte file_reader(const char *file_name, byte type, data_t *data);

/**
 * @brief Processes the status information from the string.
 *
 * @param str The string containing the status information.
 * @param data A pointer to the `data_t` structure where the data will be written.
 * @return `OK` if the processing is successful, `ERROR` otherwise.
 */
byte feel_status(const char *str, data_t *data);

/**
 * @brief Processes the vertex information from the string.
 *
 * @param str The string containing the vertex information.
 * @param data A pointer to the `data_t` structure where the data will be written.
 * @return `OK` if the processing is successful, `ERROR` otherwise.
 */
byte feel_vertex(char *str, data_t *data);

/**
 * @brief Processes the facet information from the string.
 *
 * @param str The string containing the facet information.
 * @param data A pointer to the `data_t` structure where the data will be written.
 * @return `OK` if the processing is successful, `ERROR` otherwise.
 */
byte feel_facet(char *str, data_t *data);

/**
 * @brief Performs triangulation of the specified array of facet indices.
 *
 * @param arr The array of facet indices.
 * @param length The length of the array.
 * @param data A pointer to the `data_t` structure where the data will be written.
 * @return `OK` if the triangulation is successful, `ERROR` otherwise.
 */
byte triangulation(const unsigned int *arr, unsigned long long int length, data_t *data);

byte parser(char *file_name);


#endif //INC_3D_PARSER_H
