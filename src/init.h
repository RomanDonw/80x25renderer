#ifndef INIT_H
#define INIT_H

#include <libncore.h>
#include <stdbool.h>
#include <glad.h>
#include <GLFW/glfw3.h>

#ifdef INIT_C
    #define INIT_H_EXTERN_QUALIFIER
#else
    #define INIT_H_EXTERN_QUALIFIER const
#endif

extern INIT_H_EXTERN_QUALIFIER bool __libtmrenderer_inited;
#define ENSURE_INIT { if (!__libtmrenderer_inited) return NError_NotInitialized; }

// nullable context vars.
struct ctxn_s
{
    GLFWwindow *window;
    GLuint font, vvmem, curshape, prog;
};
extern INIT_H_EXTERN_QUALIFIER struct ctxn_s __libtmrenderer_ctxn;
#define ctxn (__libtmrenderer_ctxn)

 // uniform vars.
struct ctxu_s
{ GLint colors, textblinkstate, curenabled, curpos, curbounds, curblinkstate, curuseshape; }
extern INIT_H_EXTERN_QUALIFIER __libtmrenderere_ctxu;
#define ctxu (__libtmrenderere_ctxu)

#endif