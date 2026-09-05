#include "../../../include/rendering/gl/shaders.h"
#include <GLES3/gl32.h>

static char* load_source(const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) {
        perror("fopen");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buf = (char*)malloc(size + 1);
    fread(buf, size, 1, file);

    return buf;
}

const GLuint compile_shader(const char* source, const GLenum type) {
    GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, NULL);

    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, NULL, log);
        fputs(log, stderr);

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

gas_gl_shader gas_gl_load_shader(const char* path, const GLenum type) {
    char* source = load_source(path);
    GLuint id = compile_shader(source, type);
    free(source);

    gas_gl_shader shader = { .id = id, .type = type };
    return shader;
}

void gas_gl_delete_shader(gas_gl_shader* shader) {
    glDeleteShader(shader->id);
}
