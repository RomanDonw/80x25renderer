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
    "uniform usampler1D font;\n"

    "void main(void)\n"
    "{\n"
        /*
        "//FragColor = vec4((1 + scrpos.x) * 0.5, (1 - scrpos.y) * 0.5, 0.5, 1.0);\n"
        "//FragColor = vec4(gl_FragCoord.x / cellsize.x * 0.01, gl_FragCoord.y / cellsize.y * 0.01, 0.0, 1.0);\n"

        "uvec2 pos = uvec2(gl_FragCoord.xy);\n"
        "uvec2 cellindex = pos / cellsize;\n"
        "if (cellindex.x >= uint(80) || cellindex.y >= uint(25)) discard;\n"

        "uint glyphinfontindex = (cellindex.y * uint(80) + cellindex.x) % uint(256);\n"

        "uvec2 incellpos = pos % cellsize;\n"
        "uvec2 glyphpixindex = clamp(incellpos / fontscalefactor, uvec2(0), uvec2(7, 15));\n"

        "uint glyphline = uvec4(texelFetch(font, int(glyphinfontindex * uint(16) + glyphpixindex.y), 0)).r & uint(0xFF);\n"
        "FragColor = ((glyphline & (uint(1) << (uint(7) - glyphpixindex.x))) != uint(0)) ? vec4(0.5, 0.5, 0.5, 1) : vec4(0.0, 0.0, 0.0, 1.0);\n"

        //"FragColor = vec4(float(glyphpixindex.x) / 7, float(glyphpixindex.y) / 15, 0.5, 1.0);\n"
        //"FragColor = vec4(float(uvec4(texelFetch(font, int(cellindex.y * uint(80) + cellindex.x), 0)).r) / 255, 0.1, 0.1, 1.0);\n"
        //"FragColor = vec4(float(glyphline) / 255, 0, 0, 1);\n"
        //"vec4 c = texture(font, (1 + scrpos.x) * 0.5 + (1 - scrpos.y) * 0.5 * scrsize.x);\n"
        //"FragColor = c;\n"
        */
        "vec2 scrcoord = clamp(vec2((1 + scrpos.x) * 0.5, (1 - scrpos.y) * 0.5), 0, 1);\n"
        "vec2 scrres = vec2(80, 25);\n"
        "uvec2 cell = uvec2(scrcoord * scrres);\n"

        "vec2 incell = fract(scrcoord * scrres);\n"
        "uint fontrow = clamp(uint(incell.y * 16), 0u, 15u);\n"
        "uint bitindex = uint(7) - clamp(uint(incell.x * 8u), 0u, 7u);\n"
        
        "uint glyphindex = (cell.y * 80u + cell.x) % 256u;\n"
        "uint glyphrow = uvec4(texelFetch(font, int(glyphindex * 16u + fontrow), 0)).r;\n"
        "FragColor = ((uint(glyphrow) & (1u << uint(bitindex))) != 0u) ? vec4(0.5, 0.5, 0.5, 1.0) : vec4(0.0, 0.0, 0.0, 1.0);\n"
    "}"
;
