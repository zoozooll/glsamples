#ifndef ShaderUtil_hpp
#define ShaderUtil_hpp
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

class ShaderUtil
{
public:
    static GLuint createProgram(const char* vertexShaderSource,
                                          const char* fragmentShaderSource);
    static GLuint loadShader(const char* source, GLenum shaderType);
};

#endif
