#ifndef _GAS_INCLUDE_RENDERING_GL_LOADING_H
#define _GAS_INCLUDE_RENDERING_GL_LOADING_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <gbm.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl32.h>

typedef struct gas_gl_context {
    int fd;
    struct gbm_device* gbm;
    EGLDisplay egl;
    EGLContext ctx;
} gas_gl_context;

gas_gl_context gas_gl_create_context(const char* path);

void gas_gl_delete_context(gas_gl_context* ctx);

#endif
