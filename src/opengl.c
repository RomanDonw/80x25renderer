/*
#include "opengl.h"

#include <stddef.h>

GLFWwindow *setupwindow(const char *title, int width, int height, int GLmajver, int GLminver, int GLprofile)
{
    if (!glfwInit()) return NULL;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLmajver);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLminver);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLprofile);

    GLFWwindow *ret;
    if (!(ret = glfwCreateWindow(width, height, title, NULL, NULL)))
    { puts("error creating window"); glfwTerminate(); return 1; }
    glfwMakeContextCurrent(w);
}
*/
