#include "../../../include/rendering/gl/buffers.h"
#include <GLES3/gl32.h>

gas_gl_buffer gas_gl_create_buffer(GLenum type, size_t size, GLenum usage) {
    gas_gl_buffer buffer;
    buffer.type = type;
    buffer.usage = usage;
    buffer.size = size;

    glGenBuffers(1, &buffer.id);
    gas_gl_bind_buffer(&buffer);

    gas_gl_write_buffer_data(&buffer, size, NULL);

    return buffer;
}

void gas_gl_delete_buffer(gas_gl_buffer* buffer) {
    glDeleteBuffers(1, &buffer->id);
}

void gas_gl_bind_buffer(gas_gl_buffer* buffer) {
    glBindBuffer(buffer->type, buffer->id);
}

void gas_gl_write_buffer_data(gas_gl_buffer* buffer, size_t size, void* data) {
    gas_gl_bind_buffer(buffer);

    buffer->size = size;
    glBufferData(buffer->type, buffer->size, data, buffer->usage);
}

void gas_gl_write_buffer_subdata(gas_gl_buffer* buffer, size_t off, size_t size, void* data) {
    gas_gl_bind_buffer(buffer);

    glBufferSubData(buffer->type, off, size, data);
}

void* gas_gl_map_buffer(gas_gl_buffer* buffer, size_t off, size_t size, GLbitfield access) {
    gas_gl_bind_buffer(buffer);

    return glMapBufferRange(buffer->type, off, size, access);
}

void gas_gl_unmap_buffer(gas_gl_buffer* buffer) {
    glUnmapBuffer(buffer->type);
}
