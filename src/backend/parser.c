//
// Created by Kai Evergarden on 24.09.2023.
//

#include "parser.h"

byte parser(char *file_name) {
    data_t data;
    file_checker(file_name);
    if (file_reader(file_name, FEEL_STATUS, &data)) {
        fprintf(stderr, "Failed to read status from file '%s'\n", file_name);
        return ERROR;
    }
    data.status.edges = data.status.vertexes + data.status.facets - 2;
    data.vertex_array.vertex_length = data.status.vertexes;
    data.vertex_array.vertex = calloc(data.vertex_array.vertex_length, sizeof(vertex_t));
    data.index_array.facet_length = 0;
    if (!data.vertex_array.vertex) {
        fprintf(stderr, "Failed to allocate memory for vertex array\n");
        return ERROR;
    }
    if (file_reader(file_name, FEEL_DATA, &data)) {
        fprintf(stderr, "Failed to read data from file '%s'\n", file_name);
        free(data.vertex_array.vertex);
        return ERROR;
    }
    puts("VERTEX");
    for(unsigned long long i = 0; i < data.vertex_array.vertex_length; i++) {
        printf("%f %f %f ", data.vertex_array.vertex[i].x, data.vertex_array.vertex[i].y, data.vertex_array.vertex[i].z);
        putchar('\n');
    }
    puts("FACET");
    for (unsigned long long i = 0; i < data.index_array.facet_length; i++) {
        printf("%u ", data.index_array.facet[i]);
        if ((i + 1) % 3 == 0)
            putchar('\n');
    }
    return OK;
}

byte file_checker(const char *file_name) {
    if (!strstr(file_name, ".obj")) {
        fprintf(stderr, "file %s is not .obj\n", file_name);
        return ERROR;
    }
    return OK;
}

byte file_reader(const char *file_name, byte type, data_t *data) {
    if (!file_name || !data) {
        fprintf(stderr, "file_name or data is NULL\n");
        return ERROR;
    }
    FILE *file = fopen(file_name, "r");
    if (!file) {
        fprintf(stderr, "Failed to open file: %s\n", file_name);
        return ERROR;
    }
    char *str = NULL;
    size_t len = 0;
    while ((getline(&str, &len, file)) != -1) {
        if (type == FEEL_STATUS) {
            if (feel_status(str, data)) {
                free(str);
                fclose(file);
                return ERROR;
            }
        } else if (type == FEEL_DATA) {
            char ch = *str;
            if (ch == VERTEX) {
                if (feel_vertex(str, data)) {
                    free(str);
                    fclose(file);
                    return ERROR;
                }
            } else if (ch == FACET) {
                if (feel_facet(str, data)) {
                    free(str);
                    fclose(file);
                    return ERROR;
                }
            }
        }
    }
    free(str);
    fclose(file);
    return OK;
}

byte feel_status(const char *str, data_t *data) {
    if (!str) {
        fprintf(stderr, "str is null\n");
        return ERROR;
    }
    const char ch = *str;
    if (ch == VERTEX) {
        data->status.vertexes++;
    } else if (ch == FACET) {
        data->status.facets++;
    }
    return OK;
}

byte feel_vertex(char *str, data_t *data) {
    if (!str) {
        fprintf(stderr, "str is null\n");
        return ERROR;
    }
    str++;
    static unsigned long long int vertex_counter = 0;
    if (vertex_counter >= data->vertex_array.vertex_length) {
        fprintf(stderr, "Vertex counter out of bounds\n");
        return ERROR;
    }
    vertex_t *vertex = &(data->vertex_array.vertex[vertex_counter]);
    const byte NUM_VERTEXES = 3;
    double values[NUM_VERTEXES];
    for (int i = 0; i < NUM_VERTEXES; i++) {
        values[i] = strtod(str, &str);
    }
    vertex->x = values[0];
    vertex->y = values[1];
    vertex->z = values[2];
    vertex_counter++;
    return OK;
}

byte feel_facet(char *str, data_t *data) {
    if (!str) {
        fprintf(stderr, "str is null\n");
        return ERROR;
    }
    str++;
    unsigned long long int size = 3;
    unsigned long long int i = 0;
    unsigned int *facet_array = calloc(size, sizeof(unsigned int));
    if (!facet_array) {
        fprintf(stderr, "Failed to allocate memory for facet array\n");
        return ERROR;
    }
    char *token = strtok(str, " ");
    while (token) {
        if (i >= size) {
            size *= 2;
            unsigned int *new_array = realloc(facet_array, size * sizeof(unsigned int));
            if (!new_array) {
                fprintf(stderr, "Failed to reallocate memory for facet array\n");
                free(facet_array);
                return ERROR;
            }
            facet_array = new_array;
        }
        facet_array[i] = (unsigned int) strtod(token, &token);
        token = strtok(NULL, " ");
        i++;
    }
    if (triangulation(facet_array, i, data) == ERROR) {
        free(facet_array);
        return ERROR;
    }
    free(facet_array);
    return OK;
}

byte triangulation(const unsigned int *arr, unsigned long long int length, data_t *data) {
    if (!arr || !data) {
        fprintf(stderr, "arr or data is NULL\n");
        return ERROR;
    }
    unsigned long long int new_length = data->index_array.facet_length + length * 2;
    unsigned int *new_facet_array = realloc(data->index_array.facet, new_length * sizeof(unsigned int));
    if (!new_facet_array) {
        fprintf(stderr, "Failed to reallocate memory for facet array\n");
        return ERROR;
    }
    data->index_array.facet = new_facet_array;
    for (unsigned long long int i = 1; i < length - 1; i++) {
        data->index_array.facet[data->index_array.facet_length++] = *arr;
        data->index_array.facet[data->index_array.facet_length++] = arr[i];
        data->index_array.facet[data->index_array.facet_length++] = arr[i + 1];
    }
    return OK;
}
