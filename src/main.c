#include <GL/glew.h>
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

    printf("window width: %i, window height: %i\n", winw, winh);

    GLFWwindow *w;
    if (!(w = glfwCreateWindow(winw, winh, wintitle, NULL, NULL)))
    { puts("error creating window"); glfwTerminate(); return 1; }
    glfwMakeContextCurrent(w);

    while (!glfwWindowShouldClose(w))
    {
        glClear(GL_COLOR_BUFFER_BIT);



        glfwSwapBuffers(w);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

