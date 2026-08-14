#include "shaders.h"

const char *vertexshadersource = R"(
    #version 330 core

    layout (location = 0) in vec2 _vpos;
    
    out vec2 vpos;

    void main(void)
    {
        gl_Position = vec4(_vpos.xy, 0.0, 1.0);
        vpos = _vpos.xy;
    }
)";

const char *fragmentshadersource = R"(
    #version 330 core

    in vec2 vpos;
    out vec4 FragColor;

    void main(void)
    {
        FragColor = vec4(0.4, 0.3, 0.2, 1.0);
    }
)";
