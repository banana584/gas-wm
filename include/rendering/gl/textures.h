#ifndef _GAS_INCLUDE_RENDERING_GL_TEXTURES_H
#define _GAS_INCLUDE_RENDERING_GL_TEXTURES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <GLES3/gl32.h>
#include <GLES2/gl2ext.h>

/**
 * @struct gas_gl_texture
 * @brief Represents a texture that can be binded for reading or writing.
 */
typedef struct gas_gl_texture {
    /** OpenGL id of texture. */
    GLuint id;

    /** Width of image data. */
    size_t width;
    /** Height of image data. */
    size_t height;
} gas_gl_texture;

/**
 * @brief Creates a texture with specified parameters.
 *
 * @param[in] wrap OpenGL wrapping parameter for edges of image.
 * @param[in] filter OpenGL filter parameter. GL_NEAREST leads to blockier images and GL_LINEAR leads to smoother boundaries between pixels.
 * @param[in] width The width of image data given.
 * @param[in] height The height of image data given.
 * @param[in] data The RGBA8 image data.
 * @return The created texture with specified parameters and pixel data.
 *
 * @warning Created texture must be destroyed with gas_gl_delete_texture.
 * @see gas_gl_delete_texture.
 */
gas_gl_texture gas_gl_create_texture(GLint wrap, GLint filter, size_t width, size_t height, uint8_t* data);

/**
 * @brief Deletes a texture from OpenGL.
 *
 * @param[in] texture The texture to delete.
 */
void gas_gl_delete_texture(gas_gl_texture* texture);

/**
 * @brief Binds a texture to a specific unit.
 *
 * @param[in] texture The texture to bind.
 * @param[in] unit The unit to bind to. For example, GL_TEXTURE0.
 */
void gas_gl_bind_texture_read(gas_gl_texture* texture, GLenum unit);

/**
 * @brief Binds a texture so it can be written to in a compute shader.
 *
 * @param[in] texture The texture to bind.
 * @param[in] unit The unit number to bind to.
 */
void gas_gl_bind_texture_write(gas_gl_texture* texture, size_t unit);

/**
 * @brief Updates a texture to a different size and data.
 *
 * @param[in] texture The texture to update completely.
 * @param[in] width The new width of the texture.
 * @param[in] height The new height of the texture.
 * @param[in] data The new pixel data to write in. RGBA8 format.
 *
 * @warning Is slower than gas_gl_update_subtexture, because it reallocated internal structures.
 */
void gas_gl_update_texture(gas_gl_texture* texture, size_t width, size_t height, uint8_t* data);

/**
 * @brief Updates part or all of a texture without rellocating.
 *
 * @param[in] texture The texture to update.
 * @param[in] width The width of pixel data given.
 * @param[in] height The height of pixel data given.
 * @param[in] xoff The x offset to start writing at.
 * @param[in] yoff The y offset to start writing at.
 * @param[in] data The pixel data to write into the texture. RGBA8 format.
 *
 * @warning Cannot change dimensions or format of texture, only pixel data.
 */
void gas_gl_update_subtexture(gas_gl_texture* texture, size_t width, size_t height, size_t xoff, size_t yoff, uint8_t* data);

#endif
