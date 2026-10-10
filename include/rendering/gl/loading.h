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

/**
 * @struct gas_gl_context
 * @brief The context where OpenGL ES is loaded.
*/
typedef struct gas_gl_context {
    /** Open fd of rendering device. */
    int fd;
    /** Gbm device used to bridge DRM and EGL.  */
    struct gbm_device* gbm;
    /** EGL display manager.  */
    EGLDisplay egl;
    /** EGL context used to load GL ES.  */
    EGLContext ctx;
} gas_gl_context;

/**
 * @brief Creates an OpenGL ES context.
 *
 * @warning Context must be deleted with gas_gl_delete_context.
 * @see gas_gl_delete_context
 *
 * @param[in] path The rendering device path to use. For example, "/dev/dri/card0".
 * @return The created GL ES context loaded with EGL.
*/
gas_gl_context gas_gl_create_context(const char* path);

/**
 * @brief Deletes an OpenGL ES context.
 *
 * @warning Context and any objects associated with it must never be used again.
 *
 * @param[in] ctx The GL ES context to delete.
*/
void gas_gl_delete_context(gas_gl_context* ctx);

#endif
