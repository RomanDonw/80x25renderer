#include <glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <getopt.h>

static const char *wintitle = "80x25 CGA Display";

int main(int argc, char *argv[])
{
    int winw = 800, winh = 600;
    {
        int p;
        while ((p = getopt(argc, argv, "w:h:")) != -1)
        {
            switch (p)
            {
                case 'w':
                    if (sscanf(optarg, "%d", &winw) < 1)
                    { puts("error parsing decimal integer in option -w"); return 1; }
                    break;

                case 'h':
                    if (sscanf(optarg, "%d", &winh) < 1)
                    { puts("error parsing decimal integer in option -h"); return 1; }
                    break;
            }
        }
    }

    if (!glfwInit()) { puts("failed to initialize GLFW"); return 1; }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    printf("window width: %i, window height: %i\n", winw, winh);

    GLFWwindow *w;
    if (!(w = glfwCreateWindow(800, 600, "gay", NULL, NULL)))
    { puts("error creating window"); glfwTerminate(); return 1; }
    glfwMakeContextCurrent(w);

    if (!gladLoadGL(glfwGetProcAddress))
    { puts("error loading OpenGL context through GLAD"); glfwTerminate(); return 1; }

    glViewport(0, 0, winw, winh);

    while (!glfwWindowShouldClose(w))
    {
        glClear(GL_COLOR_BUFFER_BIT);



        glfwSwapBuffers(w);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

