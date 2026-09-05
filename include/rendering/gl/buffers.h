#ifndef _GAS_INCLUDE_RENDERING_GL_BUFFERS_H
#define _GAS_INCLUDE_RENDERING_GL_BUFFERS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <GLES3/gl32.h>
#include <GLES2/gl2ext.h>

/**
 * @struct gas_gl_buffer
 * @brief Represents a GPU buffer from OpenGL.
 */
typedef struct gas_gl_buffer {
    /** OpenGL id of buffer. */
    GLuint id;
    /** Type of buffer. For example, SSBO, UBO, VBO, etc. */
    GLenum type;
    /** Usage of buffer. Passed to OpenGL to determine parameters such as memory placement. */
    GLenum usage;
    /** Size of buffer to allocate on GPU. */
    size_t size;
} gas_gl_buffer;

/**
 * @brief Creates and allocates a buffer on the GPU of a certain type and size.
 *
 * @param[in] type The type of buffer to create. For example, VBO, SSBO, etc.
 * @param[in] size The size to allocate for the buffer in bytes.
 * @param[in] usage The usage flag passed to OpenGL.
 * @return The created buffer.
 *
 * @warning Must be deleted after use by gas_gl_delete_buffer.
 * @see gas_gl_delete_buffer
 */
gas_gl_buffer gas_gl_create_buffer(GLenum type, size_t size, GLenum usage);

/**
 * @brief Deletes a buffer from OpenGL and GPU memory.
 *
 * @param[in] buffer The buffer to delete.
 */
void gas_gl_delete_buffer(gas_gl_buffer* buffer);

/**
 * @brief Binds a buffer to its type.
 *
 * @param[in] buffer The buffer to bind.
 */
void gas_gl_bind_buffer(gas_gl_buffer* buffer);

/**
 * @brief Reallocates a buffer and writes all data to it.
 *
 * @param[in] buffer The buffer to write to.
 * @param[in] size The size of the data being written in bytes.
 * @param[in] data The data to write into GPU memory.
 *
 * @warning Is slower than writing subdata, as this reallocates the whole GPU memory.
 */
void gas_gl_write_buffer_data(gas_gl_buffer* buffer, size_t size, void* data);

/**
 * @brief Writes data into part of a GPU buffer.
 *
 * @param[in] buffer The buffer to write data to.
 * @param[in] off The part of the buffer to start writing at.
 * @param[in] size The amount of data to write in bytes.
 * @param[in] data The data to write.
 */
void gas_gl_write_buffer_subdata(gas_gl_buffer* buffer, size_t off, size_t size, void* data);

/**
 * @brief Maps a buffer into a pointer that can be read like any other.
 *
 * @param[in] buffer The buffer whose contents will be mapped.
 * @param[in] off The offset to start reading from in bytes.
 * @param[in] size The amount of data to read in bytes.
 * @param[in] access OpenGL bitfield of what permissions are required.
 * @return A pointer to the memory that was mapped.
 *
 * @warning Memory must be unmapped with gas_gl_unmap_buffer.
 * @see gas_gl_unmap_buffer
 */
void* gas_gl_map_buffer(gas_gl_buffer* buffer, size_t off, size_t size, GLbitfield access);

/**
 * @brief Unmaps the memory from a mapped buffer.
 *
 * @param[in] buffer The buffer whose memory should be unmapped.
 */
void gas_gl_unmap_buffer(gas_gl_buffer* buffer);

#endif
