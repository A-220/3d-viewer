#include "parser.h"

uint8_t parser(char *file_name) {
    data_t data;
    if (file_checker(file_name) == ERROR) {
        fprintf(stderr, "Failed to check file '%s'\n", file_name);
        return ERROR;
    }
    if (file_reader(file_name, FEEL_STATUS, &data) == ERROR) {
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
    if (file_reader(file_name, FEEL_DATA, &data) == ERROR) {
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

uint8_t file_checker(const char *file_name) {
    if (!file_name) {
        fprintf(stderr, "Error: File name is NULL\n");
        return ERROR;
    }
    if (!strstr(file_name, ".obj")) {
        fprintf(stderr, "Error: File '%s' is not in .obj format\n", file_name);
        return ERROR;
    }
    return OK;
}
uint8_t file_reader(const char *file_name, uint8_t type, data_t *data) {
    if (!file_name || !data) {
        fprintf(stderr, "Error: file_name or data is NULL\n");
        return ERROR;
    }
    FILE *file = fopen(file_name, "r");
    if (!file) {
        fprintf(stderr, "Error: Failed to open file '%s'\n", file_name);
        return ERROR;
    }
    char *str = NULL;
    size_t len = 0;
    while ((getline(&str, &len, file)) != -1) {
        if (type == FEEL_STATUS) {
            if (feel_status(str, data) == ERROR) {
                fprintf(stderr, "Error: Failed to feel status from file '%s'\n", file_name);
                free(str);
                fclose(file);
                return ERROR;
            }
        } else if (type == FEEL_DATA) {
            char ch = *str;
            if (ch == VERTEX) {
                if (feel_vertex(str, data) == ERROR) {
                    fprintf(stderr, "Error: Failed to feel vertex from file '%s'\n", file_name);
                    free(str);
                    fclose(file);
                    return ERROR;
                }
            } else if (ch == FACET) {
                if (feel_facet(str, data) == ERROR) {
                    fprintf(stderr, "Error: Failed to feel facet from file '%s'\n", file_name);
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

uint8_t feel_status(const char *str, data_t *data) {
    if (str == NULL) {
        fprintf(stderr, "Error: str is NULL\n");
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

uint8_t feel_vertex(char *str, data_t *data) {
    if (str == NULL) {
        fprintf(stderr, "Error: str is NULL\n");
        return ERROR;
    }
    str++;
    static size_t vertex_counter = 0;
    if (vertex_counter >= data->vertex_array.vertex_length) {
        fprintf(stderr, "Error: Vertex counter is out of bounds\n");
        return ERROR;
    }
    vertex_t *vertex = &(data->vertex_array.vertex[vertex_counter]);
    double values[SIZE];
    for (int i = 0; i < SIZE; i++) {
        values[i] = strtod(str, &str);
        if (*str == '\0') {
            fprintf(stderr, "Error: Failed to parse vertex coordinates from string '%s'\n", str);
            return ERROR;
        }
    }
    vertex->x = values[0];
    vertex->y = values[1];
    vertex->z = values[2];
    vertex_counter++;
    return OK;
}


uint8_t feel_facet(char *str, data_t *data) {
    if (str == NULL) {
        fprintf(stderr, "Error: str is NULL\n");
        return ERROR;
    }
    str++;
    size_t int size = SIZE, count = 0;
    unsigned int *facet_array = calloc(size, sizeof(unsigned int));
    if (!facet_array) {
        fprintf(stderr, "Error: Failed to allocate memory for facet array\n");
        return ERROR;
    }
    char *token = strtok(str, SPACE);
    while (token) {
        if (count >= size) {
            size *= 2;
            facet_array = realloc(facet_array, size * sizeof(unsigned int));
            if (facet_array == NULL) {
                fprintf(stderr, "Error: Failed to reallocate memory for facet array\n");
                free(facet_array);
                return ERROR;
            }
        }
        unsigned int value = (unsigned int) strtod(token, &token);
        facet_array[count] = value;
        token = strtok(NULL, SPACE);
        count++;
        if (count >= ULLONG_MAX / 2) {
            fprintf(stderr, "Error: Maximum facet count exceeded\n");
            free(facet_array);
            return ERROR;
        }
    }
    if (triangulation(facet_array, count, data) == ERROR) {
        free(facet_array);
        return ERROR;
    }
    free(facet_array);
    return OK;
}

uint8_t triangulation(const unsigned int *arr, unsigned long long int length, data_t *data) {
    if (!arr || !data) {
        fprintf(stderr, "Error: arr or data is NULL\n");
        return ERROR;
    }
    size_t new_length = data->index_array.facet_length * 100 * length;
    data->index_array.facet = realloc(data->index_array.facet, new_length * sizeof(unsigned int));
    if (!data->index_array.facet) {
        fprintf(stderr, "Error: Failed to reallocate memory for facet array\n");
        return ERROR;
    }
    for (unsigned long long int i = 1; i < length - 1; i++) {
        data->index_array.facet[data->index_array.facet_length++] = *arr;
        data->index_array.facet[data->index_array.facet_length++] = arr[i];
        data->index_array.facet[data->index_array.facet_length++] = arr[i + 1];
    }
    return OK;
}

