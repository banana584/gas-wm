#include "../../../include/rendering/gl/loading.h"

gas_gl_context gas_gl_create_context(const char* path) {
    gas_gl_context ctx = {0};

    ctx.fd = open(path, O_RDWR | O_CLOEXEC);
    if (ctx.fd < 0) {
        perror("open");
        return ctx;
    }

    ctx.gbm = gbm_create_device(ctx.fd);
    if (!ctx.gbm) {
        fprintf(stderr, "gbm_create_device failed\n");
        close(ctx.fd);
        return ctx;
    }
    
    ctx.egl = eglGetPlatformDisplay(EGL_PLATFORM_GBM_KHR, ctx.gbm, NULL);
    if (ctx.egl == EGL_NO_DISPLAY) {
        fprintf(stderr, "eglGetPlatformDisplay failed: 0x%x\n", eglGetError());
        gbm_device_destroy(ctx.gbm);
        close(ctx.fd);
        return ctx;
    }

    EGLint major, minor;
    if (!eglInitialize(ctx.egl, &major, &minor)) {
        fprintf(stderr, "eglInitialize failed0x%x\n", eglGetError());
        return ctx;
    }

    const EGLint config_attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,

        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,

        EGL_NONE
    };

    EGLConfig config;
    EGLint num_configs;

    if (!eglChooseConfig(ctx.egl, config_attribs, &config, 1, &num_configs)) {
        fprintf(stderr, "eglChooseConfig failed 0x%x\n", eglGetError());
        return ctx;
    }

    if (num_configs == 0) {
        fprintf(stderr, "No suitable EGL configs\n");
        return ctx;
    }

    const EGLint context_attribs[] = {
        EGL_CONTEXT_MAJOR_VERSION_KHR, 3,
        EGL_CONTEXT_MINOR_VERSION_KHR, 2,
        
        EGL_NONE
    };

    ctx.ctx = eglCreateContext(ctx.egl, config, EGL_NO_CONTEXT, context_attribs);
    if (ctx.ctx == EGL_NO_CONTEXT) {
        fprintf(stderr, "eglCreateContext failed 0x%x\n", eglGetError());
        return ctx;
    }

    return ctx;
}

void gas_gl_delete_context(gas_gl_context* ctx) {
    eglDestroyContext(ctx->egl, ctx->ctx);
    eglTerminate(ctx->egl);
    gbm_device_destroy(ctx->gbm);
    close(ctx->fd);
}
