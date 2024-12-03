#if !defined(SHADER_ERROR_HANDLER)
#define SHADER_ERROR_HANDLER

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace ShaderErrorHandler
{
    inline void printShaderLog(GLuint shader)
    {
        int len = 0;
        int chWrittn = 0;
        char *log;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        if (len > 0)
        {
            log = (char *)malloc(len);
            glGetShaderInfoLog(shader, len, &chWrittn, log);
            std::cout << "Shader Info Log: " << log << std::flush;
            free(log);
        }
    }

    inline void printProgramLog(int prog)
    {
        int len = 0;
        int chWrittn = 0;
        char *log;
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
        if (len > 0)
        {
            log = (char *)malloc(len);
            glGetProgramInfoLog(prog, len, &chWrittn, log);
            std::cout << "Program Info Log: " << log << std::flush;
            free(log);
        }
    }

    inline bool checkOpenGLError()
    {
        bool foundError = false;
        int glErr = glGetError();
        while (glErr != GL_NO_ERROR)
        {
            std::cout << "glError: " << glErr << '\n';
            foundError = true;
            glErr = glGetError();
        }
        std::cout << std::flush;
        return foundError;
    }

}

#endif // SHADER_ERROR_HANDLER