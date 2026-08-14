#include "shaders.h"

const char *vertexshadersource = R"(
    #version 330 core

    layout (location = 0) in vec2 vpos;
    
    out vec2 scrpos;

    void main(void)
    {
        gl_Position = vec4(vpos, 0.0, 1.0);
        scrpos = vpos;
    }
)";

const char *fragmentshadersource = R"(
    #version 330 core

    in vec2 scrpos;
    out vec4 FragColor;

    void main(void)
    {
        FragColor = vec4((1 + scrpos.x) * 0.5, (1 - scrpos.y) * 0.5, 0.5, 1.0);
    }
)";
