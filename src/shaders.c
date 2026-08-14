#include "shaders.h"

const char *vertexshadersource =
    "#version 330 core\n"

    "layout (location = 0) in vec2 vpos;\n"
    
    "out vec2 scrpos;\n"

    "void main(void)\n"
    "{"
        "gl_Position = vec4(vpos, 0.0, 1.0);\n"
        "scrpos = vpos;\n"
    "}"
;

const char *fragmentshadersource =
    "#version 330 core\n"
    
    "uniform uvec2 scrsize;\n"
    "uniform uvec2 cellsize;\n"
    "uniform uvec2 fontscalefactor;\n"
    
    "layout(origin_upper_left, pixel_center_integer) in vec4 gl_FragCoord;\n"

    "in vec2 scrpos;\n"
    "out vec4 FragColor;\n"
    "uniform sampler1D font;\n"

    "void main(void)\n"
    "{\n"
        "//FragColor = vec4((1 + scrpos.x) * 0.5, (1 - scrpos.y) * 0.5, 0.5, 1.0);\n"
        "//FragColor = vec4(gl_FragCoord.x / cellsize.x * 0.01, gl_FragCoord.y / cellsize.y * 0.01, 0.0, 1.0);\n"

        "uvec2 pos = uvec2(gl_FragCoord.xy);\n"
        "uvec2 cellindex = pos / cellsize;\n"
        "if (cellindex.x >= uint(80) || cellindex.y >= uint(25)) discard;\n"

        "uvec2 incellpos = pos % cellsize;\n"
        "uvec2 glyphpixindex = incellpos / fontscalefactor;\n"

        "uint glyphline = uvec4(texelFetch(font, int((cellindex.y * uint(80) + cellindex.x) % uint(256) * uint(16) + glyphpixindex.y % uint(16)), 0)).r;\n"
        //"FragColor = ((glyphline & (uint(1) << (uint(7) - glyphpixindex.x % uint(8)))) != uint(0)) ? vec4(0.5, 0.5, 0.5, 1) : vec4(0.0, 0.0, 0.0, 1.0);\n"
        //"FragColor = vec4(float(glyphpixindex.x) / 7, float(glyphpixindex.y) / 15, 0.5, 1.0);\n"
        "FragColor = vec4(texelFetch(font, int(cellindex.y * uint(80) + cellindex.x), 0));\n"
    "}"
;
