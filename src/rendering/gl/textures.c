#include "../../../include/rendering/gl/textures.h"
#include <GLES3/gl32.h>

gas_gl_texture gas_gl_create_texture(GLint wrap, GLint filter, size_t width, size_t height, uint8_t* data) {
    gas_gl_texture tex = { .width = width, .height = height };

    glGenTextures(1, &tex.id);
    glBindTexture(GL_TEXTURE_2D, tex.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA8, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    return tex;
}

void gas_gl_delete_texture(gas_gl_texture* texture) {
    glDeleteTextures(1, &texture->id);
}

void gas_gl_bind_texture_read(gas_gl_texture* texture, GLenum unit) {
    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

void gas_gl_bind_texture_write(gas_gl_texture* texture, size_t unit) {
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glBindImageTexture(unit, texture->id, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
}

void gas_gl_update_texture(gas_gl_texture* texture, size_t width, size_t height, uint8_t* data) {
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA8, GL_UNSIGNED_BYTE, data);
}

void gas_gl_update_subtexture(gas_gl_texture* texture, size_t width, size_t height, size_t xoff, size_t yoff, uint8_t* data) {
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, xoff, yoff, width, height, GL_RGBA8, GL_UNSIGNED_BYTE, data);
}
