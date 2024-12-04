#if !defined(UTILS_H)
#define UTILS_H

#include "ShadersErrorHandler.hpp"

#include <string>
#include <exception>
#include <fstream>
#include <sstream>

namespace Utils
{
    constexpr float POINT_DRAG_THRESHHOLD = 0.02;

    inline std::string readShaderSource(const char *filePath)
    {
        std::string content;
        std::ifstream ifs(filePath, std::ios::in);
        if (!ifs.is_open())
        {
            std::stringstream ss;
            ss << "Shader file " << filePath << " not found.\n";
            throw std::runtime_error(ss.str());
        }
        std::string line = "";
        while (!ifs.eof())
        {
            std::getline(ifs, line);
            content.append(line + '\n');
        }
        ifs.close();
        return content;
    }

    inline GLuint createShaderProgram(const std::string &vertShader, const std::string &fragShader)
    {
        GLint vertCompiled;
        GLint fragCompiled;
        GLint linked;

        std::string vertShaderStr = readShaderSource(vertShader.c_str());
        std::string fragShaderStr = readShaderSource(fragShader.c_str());

        GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

        const char *vshaderSource = vertShaderStr.c_str();
        const char *fshaderSource = fragShaderStr.c_str();

        // loads the GLSL code from the strings into the empty shader objects
        glShaderSource(vShader, 1, &vshaderSource, NULL);
        glShaderSource(fShader, 1, &fshaderSource, NULL);

        // compile Vertex Shader
        glCompileShader(vShader);
        ShaderErrorHandler::checkOpenGLError();
        glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
        if (vertCompiled != 1)
        {
            ShaderErrorHandler::printShaderLog(vShader);
            throw std::runtime_error("Vertex compilation failed.");
        }

        // compile Fragment Shader
        glCompileShader(fShader);
        ShaderErrorHandler::checkOpenGLError();
        glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
        if (vertCompiled != 1)
        {
            ShaderErrorHandler::printShaderLog(fShader);
            throw std::runtime_error("Fragment compilation failed.");
        }

        // contains a series of compiled shaders
        GLuint vfProgram = glCreateProgram();

        glAttachShader(vfProgram, vShader);
        glAttachShader(vfProgram, fShader);

        glLinkProgram(vfProgram);
        ShaderErrorHandler::checkOpenGLError();
        glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
        if (linked != 1)
        {
            ShaderErrorHandler::printProgramLog(vfProgram);
            throw std::runtime_error("Shader Linking failed.");
        }

        return vfProgram;
    }

    inline int calculateLowResNumSegments(std::size_t pointCount)
    {
        if (pointCount < 5)
            return 80;
        if (pointCount < 15)
            return 60;
        if (pointCount < 20)
            return 40;
        if (pointCount < 25)
            return 20;
        return 15;
    }

    inline bool isPointBeingHovered(float mouseX, float mouseY, float x, float y)
    {
        return mouseX >= x - POINT_DRAG_THRESHHOLD && mouseX <= x + POINT_DRAG_THRESHHOLD &&
               mouseY >= y - POINT_DRAG_THRESHHOLD && mouseY <= y + POINT_DRAG_THRESHHOLD;
    }
}

#endif // UTILS_H