#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include "Helper.h"
class ShaderLoader
{
public:
    static const bool IsDebug = false;

    inline static GLuint CompileVertShader(std::string_view _source)
    {
        // Creation
        GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
        const char* src = _source.data();
        glShaderSource(vertShader, 1, &src, nullptr);

        // Compile vertex shader
        if (IsDebug)
        {
            Print("Compiling vertex shader.");
        }
        glCompileShader(vertShader);

        // Debug
        int result;
        glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            int length;
            glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)_malloca(length * sizeof(char));
            glGetShaderInfoLog(vertShader, length, &length, message);
            Print("Failed to Compile Vertex Shader");
            Print(message);
            glDeleteShader(vertShader);
            return 0;
        }

        return vertShader;
    }

    inline static GLuint CompileFragShader(std::string_view _source)
    {
        GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* src = _source.data();
        glShaderSource(fragShader, 1, &src, nullptr);

        // Compile Frag shader
        if (IsDebug)
        {
            Print("Compiling fragment shader.");
        }
        glCompileShader(fragShader);

        // Debug
        int result;
        glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            int length;
            glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)_malloca(length * sizeof(char));
            glGetShaderInfoLog(fragShader, length, &length, message);
            Print("Failed to Compile Vertex Shader");
            Print(message);
            glDeleteShader(fragShader);
            return 0;
        }

        return fragShader;
    }

    inline static GLuint CompileGeoShader(std::string_view _source)
    {
        GLuint geoShader = glCreateShader(GL_GEOMETRY_SHADER);
        const char* src = _source.data();
        glShaderSource(geoShader, 1, &src, nullptr);

        // Compile Frag shader
        if (IsDebug)
        {
            Print("Compiling Geometry shader.");
        }
        glCompileShader(geoShader);

        // Debug
        int result;
        glGetShaderiv(geoShader, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            int length;
            glGetShaderiv(geoShader, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)_malloca(length * sizeof(char));
            glGetShaderInfoLog(geoShader, length, &length, message);
            Print("Failed to Compile Vertex Shader");
            Print(message);
            glDeleteShader(geoShader);
            return 0;
        }

        return geoShader;
    }

    inline static GLuint CreateShader(std::string_view _vertexShader, std::string_view _geoShader, std::string_view _fragmentShader)
    {
        // Program
        GLuint program = glCreateProgram();

        // Shaders

        GLuint vertShader = CompileVertShader(readFile(_vertexShader));
        GLuint geoShader = 0;
        if (_geoShader != "")
            geoShader = CompileGeoShader(readFile(_geoShader));
        GLuint fragShader = CompileFragShader(readFile(_fragmentShader));

        if (IsDebug)
        {
            Print("Attaching Shaders");
        }
        glAttachShader(program, vertShader);
        if (_geoShader != "")
            glAttachShader(program, geoShader);
        glAttachShader(program, fragShader);

        if (IsDebug)
        {
            Print("Linking program");
        }
        glLinkProgram(program);
        glValidateProgram(program);

        if (IsDebug)
        {
            Print("Deleting Shaders");
        }
        glDeleteShader(vertShader);
        if (_geoShader != "")
            glDeleteShader(geoShader);
        glDeleteShader(fragShader);

        return program;
    }

    inline static std::string readFile(std::string_view _fileAddress)
    {
        std::string content;
        std::ifstream fileStream(_fileAddress.data(), std::ios::in);

        if (!fileStream.is_open()) {
            Print("Could not read file ");
            Print(_fileAddress.data());
            Print(". File does not exist.");
            return "";
        }

        std::string line = "";
        while (!fileStream.eof()) {
            std::getline(fileStream, line);
            content.append(line + "\n");
        }

        fileStream.close();
        return content;
    }
};

