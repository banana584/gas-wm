#include "../../../include/rendering/gl/vertex_arrays.h"
#include <GLES3/gl32.h>

gas_gl_vertex_array gas_gl_create_vertex_array() {
    gas_gl_vertex_array array = {0};

    glGenVertexArrays(1, &array.id);

    return array;
}

void gas_gl_delete_vertex_array(gas_gl_vertex_array* array) {
    glDeleteVertexArrays(1, &array->id);
}

void gas_gl_bind_vertex_array(gas_gl_vertex_array* array) {
    glBindVertexArray(array->id);
}

void gas_gl_vertex_array_add_attrib(gas_gl_vertex_array* array, GLenum type, size_t amount, size_t stride) {
    gas_gl_bind_vertex_array(array);

    glVertexAttribPointer(array->current_attrib, amount, type, GL_FALSE, stride, (void*)array->current_off);
    glEnableVertexAttribArray(array->current_attrib++);

    array->current_off += stride;
}
