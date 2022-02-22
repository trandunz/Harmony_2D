#pragma once
#include "Helper.h"
class ShaderLoader
{
public:
    static const bool IsDebug = false;
    inline static GLuint CreateShader(std::string_view _vertexShader, std::string_view _geoShader, std::string_view _fragmentShader)
    {
        // Create Program
        GLuint program = glCreateProgram();

        // Create Shaders And Store There ID's
        GLuint vertShader = CompileShader(GL_VERTEX_SHADER, PassFileToString(_vertexShader));
        GLuint geoShader = CompileShader(GL_GEOMETRY_SHADER, PassFileToString(_geoShader));
        GLuint fragShader = CompileShader(GL_FRAGMENT_SHADER, PassFileToString(_fragmentShader));

        // Attach Shaders To Program
        if (IsDebug)
        {
            Print("Attaching Shaders");
        }
        glAttachShader(program, vertShader);
        glAttachShader(program, geoShader);
        glAttachShader(program, fragShader);

        // Link And Validate
        if (IsDebug)
        {
            Print("Linking program");
        }
        glLinkProgram(program);
        glValidateProgram(program);

        // Cleanup
        if (IsDebug)
        {
            Print("Deleting Shaders");
        }
        glDeleteShader(vertShader);
        glDeleteShader(geoShader);
        glDeleteShader(fragShader);

        // Return Program ID
        return program;
    }
    inline static GLuint CreateShader(std::string_view _vertexShader, std::string_view _fragmentShader)
    {
        // Create Program
        GLuint program = glCreateProgram();

        // Create Shaders And Store There ID's
        GLuint vertShader = CompileShader(GL_VERTEX_SHADER, PassFileToString(_vertexShader));
        GLuint fragShader = CompileShader(GL_FRAGMENT_SHADER, PassFileToString(_fragmentShader));

        // Attach Shaders To Program
        if (IsDebug)
        {
            Print("Attaching Shaders");
        }
        glAttachShader(program, vertShader);
        glAttachShader(program, fragShader);

        // Link And Validate
        if (IsDebug)
        {
            Print("Linking program");
        }
        glLinkProgram(program);
        glValidateProgram(program);

        // Cleanup
        if (IsDebug)
        {
            Print("Deleting Shaders");
        }
        glDeleteShader(vertShader);
        glDeleteShader(fragShader);

        // Return Program ID
        return program;
    }
private:
    inline static GLuint CompileShader(GLenum _type, std::string_view _source)
    {
        GLuint shader = glCreateShader(_type);
        const GLchar* src = _source.data();
        glShaderSource(shader, 1, &src, nullptr);

        // Compile Shader
        if (IsDebug)
        {
            switch (_type)
            {
            case GL_VERTEX_SHADER:
            {
                Print("Compiling vertex shader.");
                break;
            }
            case GL_FRAGMENT_SHADER:
            {
                Print("Compiling fragment shader.");
                break;
            }
            case GL_GEOMETRY_SHADER:
            {
                Print("Compiling geometry shader.");
                break;
            }
            default:
                break;
            }
        }
        glCompileShader(shader);

        // Debug
        int result;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            int length;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)_malloca(length * sizeof(char));
            glGetShaderInfoLog(shader, length, &length, message);
            Print("Failed to Compile Vertex Shader");
            Print(message);
            glDeleteShader(shader);
            return result;
        }

        return shader;
    }
    inline static std::string PassFileToString(std::string_view _fileAddress)
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

