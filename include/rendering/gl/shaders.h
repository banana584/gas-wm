#ifndef _GAS_INCLUDE_RENDERING_GL_SHADERS_H
#define _GAS_INCLUDE_RENDERING_GL_SHADERS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <GLES3/gl32.h>
#include <GLES2/gl2ext.h>

/**
 * @struct gas_gl_shader
 * @brief Represents a shader loaded from GLSL source and linked to programs.
 */
typedef struct gas_gl_shader {
    /** OpenGL id of shader. */
    GLuint id;

    /** Type of shader. For example, vertex, fragment, etc. */
    GLenum type;
} gas_gl_shader;

/**
 * @brief Loads a shader from source file.
 *
 * @param[in] path The path to load source code from.
 * @param[in] type The type of shader to compile.
 * @return The loaded shader struct, to be linked into a program.
 *
 * @warning Shader must be destroyed by either attaching to a program or manually. Most of the time, shaders are attached to programs and deletion is handled by the program.
 */
gas_gl_shader gas_gl_load_shader(const char* path, const GLenum type);

/**
 * @brief Deletes a loaded shader.
 *
 * @param[in] shader The shader to delete.
 *
 * @warning Most of the time, shaders are not deleted manually but instead by attaching to a program.
 */
void gas_gl_delete_shader(gas_gl_shader* shader);

#endif
