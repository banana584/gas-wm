#include "../../../include/rendering/gl/programs.h"
#include <GLES3/gl32.h>

IMPL_HASHMAP(uniform_map, GLint);

gas_gl_program gas_gl_create_program(const size_t num_shaders, gas_gl_shader* shaders) {
    gas_gl_program program;
    program.id = glCreateProgram();
    program.uniforms = uniform_map_create();

    for (size_t i = 0; i < num_shaders; i++) {
        glAttachShader(program.id, shaders[i].id);
        gas_gl_delete_shader(&shaders[i]);
    }

    glLinkProgram(program.id);

    int success;
    glGetProgramiv(program.id, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(program.id, 512, NULL, log);
        fputs(log, stderr);

        gas_gl_delete_program(&program);
        return program;
    }

    return program;
}

void gas_gl_delete_program(gas_gl_program* program) {
    glDeleteProgram(program->id);
    uniform_map_destroy(&program->uniforms);
}

GLint gas_gl_get_uniform(gas_gl_program* program, const char* name) {
    bool found;
    GLint loc = uniform_map_get(&program->uniforms, name, &found);

    if (!found) {
        loc = glGetUniformLocation(program->id, name);
        uniform_map_set(&program->uniforms, name, &loc);
    }

    return loc;
}

void gas_gl_use_program(gas_gl_program* program) {
    glUseProgram(program->id);
}

#define GAS_UNIFORM_0(_name) glUniform##_name(gas_gl_get_uniform(program, name), v0);
#define GAS_UNIFORM_1(_name) glUniform##_name(gas_gl_get_uniform(program, name), v0, v1);
#define GAS_UNIFORM_2(_name) glUniform##_name(gas_gl_get_uniform(program, name), v0, v1, v2);
#define GAS_UNIFORM_3(_name) glUniform##_name(gas_gl_get_uniform(program, name), v0, v1, v2, v3);

#define GAS_UNIFORM_VEC(_name) glUniform##_name(gas_gl_get_uniform(program, name), count, value);

#define GAS_UNIFORM_MATRIX(_name) glUniform##_name(gas_gl_get_uniform(program, name), count, transpose, value);

#define X(_name, _type, ...) void gas_gl_uniform##_name(gas_gl_program* program, const char* name, __VA_ARGS__) { \
    GAS_UNIFORM_##_type(_name); \
}
_GAS_GL_UNIFORM_LIST(X)
#undef X
