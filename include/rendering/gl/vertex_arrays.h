#ifndef _GAS_INCLUDE_RENDERING_GL_VERTEX_ARRAYS_H
#define _GAS_INCLUDE_RENDERING_GL_VERTEX_ARRAYS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <GLES3/gl32.h>
#include <GLES2/gl2ext.h>

/**
 * @struct gas_gl_vertex_array
 * @brief Describes a vertex array used to descrive layout of the vertex buffer.
 */
typedef struct gas_gl_vertex_array {
    /** OpenGL id of vertex array. */
    GLuint id;

    /** Current attribute number. Starts at 0 and increases when attributes are added. */
    size_t current_attrib;
    /** Current offset pointer. Starts at 0 and increased based on the size of attributes. */
    size_t current_off;
} gas_gl_vertex_array;

/**
 * @brief Creates a new vertex array with no attributes.
 *
 * @return The created vertex array.
 *
 * @warning Vertex array must be deleted with gas_gl_delete_vertex_array.
 * @see gas_gl_delete_vertex_array.
 */
gas_gl_vertex_array gas_gl_create_vertex_array();

/**
 * @brief Deletes a vertex array from OpenGL.
 *
 * @param[in] array The vertex array to delete.
 */
void gas_gl_delete_vertex_array(gas_gl_vertex_array* array);

/**
 * @brief Binds a vertex array as the current in OpenGL.
 *
 * @param[in] array The vertex array to bind.
 */
void gas_gl_bind_vertex_array(gas_gl_vertex_array* array);

/**
 * @brief Adds an attribute to the vertex array.
 *
 * @param[in] array The vertex array to have an attribute added to.
 * @param[in] type The OpenGL type of data. For example, GL_FLOAT.
 * @param[in] amount How many items in the vertex buffer makes 1 element.
 * @param[in] stride How far to step forward to get to the next element.
 */
void gas_gl_vertex_array_add_attrib(gas_gl_vertex_array* array, GLenum type, size_t amount, size_t stride);

#endif
