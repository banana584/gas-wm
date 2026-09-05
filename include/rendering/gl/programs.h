#ifndef _GAS_INCLUDE_RENDERING_GL_PROGRAMS_H
#define _GAS_INCLUDE_RENDERING_GL_PROGRAMS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include "shaders.h"
#include "../../data_structures/hashmap.h"
#include <GLES3/gl32.h>
#include <GLES2/gl2ext.h>

DECLARE_HASHMAP(uniform_map, GLint)

/**
 * @struct gas_gl_program
 * @brief Represents a program that can be used to render or compute.
 */
typedef struct gas_gl_program {
    /** OpenGL id of program. */
    GLuint id;

    /** Hashmap of uniform names to locations. */
    uniform_map uniforms;
} gas_gl_program;

/**
 * @brief Creates a program, ready to be ran.
 *
 * @param[in] num_shaders The number of shaders given in the array.
 * @param[in] shaders The array of shaders to link into a program.
 * @return The created program that can be ran.
 *
 * @warning After use, program must be destroyed by gas_gl_delete_program.
 * @see gas_gl_delete_program
 */
gas_gl_program gas_gl_create_program(const size_t num_shaders, gas_gl_shader* shaders);

/**
 * @brief Deletes a program from memory.
 *
 * @param[in] program The program to delete.
 */
void gas_gl_delete_program(gas_gl_program* program);

/**
 * @brief Gets a uniform location by name.
 *
 * Looks up the uniform in a hashmap, and if it is not found stores it and continues.
 *
 * @param[in] program The program who the uniform belongs to.
 * @param[in] name The name of the uniform to search for.
 */
GLint gas_gl_get_uniform(gas_gl_program* program, const char* name);

/**
 * @brief Sets a program as the currently used program.
 *
 * @param[in] program The program to set to use.
 */
void gas_gl_use_program(gas_gl_program* program);

#define _GAS_GL_UNIFORM_LIST(X) \
X(1f, 0, GLfloat v0) \
X(1fv, VEC, GLsizei count, const GLfloat* value) \
X(1i, 0, GLint v0) \
X(1iv, VEC, GLsizei count, const GLint* value) \
X(2f, 1, GLfloat v0, GLfloat v1) \
X(2fv, VEC, GLsizei count, const GLfloat* value) \
X(2i, 1, GLint v0, GLint v1) \
X(2iv, VEC, GLsizei count, const GLint* value) \
X(3f, 2, GLfloat v0, GLfloat v1, GLfloat v2) \
X(3fv, VEC, GLsizei count, const GLfloat* value) \
X(3i, 2, GLint v0, GLint v1, GLint v2) \
X(3iv, VEC, GLsizei count, const GLint* value) \
X(4f, 3, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) \
X(4fv, VEC, GLsizei count, const GLfloat* value) \
X(4i, 3, GLint v0, GLint v1, GLint v2, GLint v3) \
X(4iv, VEC, GLsizei count, const GLint* value) \
X(Matrix2fv, MATRIX, GLsizei count, GLboolean transpose, const GLfloat* value) \
X(Matrix3fv, MATRIX, GLsizei count, GLboolean transpose, const GLfloat* value) \
X(Matrix4fv, MATRIX, GLsizei count, GLboolean transpose, const GLfloat* value)

#define X(_name, _type, ...) void gas_gl_uniform##_name(gas_gl_program* program, const char* name, __VA_ARGS__);
_GAS_GL_UNIFORM_LIST(X)
#undef X

#endif
