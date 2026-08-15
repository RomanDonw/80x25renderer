#include "shaders.h"

const char *vertexshadersource =
    "#version 330 core\n"

    "layout (location = 0) in vec2 vpos;\n"
    "out vec2 scrcoord;\n"

    "void main(void)\n"
    "{"
        "gl_Position = vec4(vpos, 0.0, 1.0);\n"
        "scrcoord = clamp(vec2((1 + vpos.x) * 0.5, (1 - vpos.y) * 0.5), 0, 1);\n"
    "}"
;

const char *fragmentshadersource =
    "#version 330 core\n"

    "in vec2 scrcoord;\n"
    "out vec4 FragColor;\n"

    //"uniform uint blinkstate;\n"
    "uniform vec3 colors[16];\n"
    "uniform usampler2D vmem;\n"
    "uniform usampler2D font;\n"

    "void main(void)\n"
    "{\n"
        "vec2 scrres = vec2(80, 25);\n"
        "uvec2 cell = uvec2(scrcoord * scrres);\n"

        "vec2 incell = fract(scrcoord * scrres);\n"
        "uint fontrow = clamp(uint(incell.y * 16), 0u, 15u);\n"
        "uint bitindex = uint(7) - clamp(uint(incell.x * 8u), 0u, 7u);\n"
        
        //"uint glyphindex = (cell.y * 80u + cell.x) % 256u;\n"
        "uvec4 chardata = uvec4(texelFetch(vmem, ivec2(cell.x, cell.y), 0));\n"
        "uint glyphrow = uvec4(texelFetch(font, ivec2(fontrow, chardata.r & 0xFFu), 0)).r & 0xFFu;\n"

        //"bool render = true;\n"
        //"if ((uint(chardata & (1u << 7u)) != 0u) && (blinkstate != 0u)) render = false;\n"
        "FragColor = ((uint(glyphrow) & (1u << uint(bitindex))) != 0u)"
                "? vec4(colors[chardata.g & 0xFu], 1)"
                ": vec4(colors[(chardata.g >> 4) & 7u], 1);\n"
    "}"
;
