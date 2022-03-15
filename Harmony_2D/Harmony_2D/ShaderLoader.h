#pragma once
#include "Helper.h"

static class ShaderLoader
{
public:
    ~ShaderLoader()
    {
        glUseProgram(0);
        for (auto& item : ShaderPrograms)
        {
            glDeleteProgram(item.second);
        }
        for (auto& item : Shaders)
        {
            glDeleteShader(item.second);
        }

        Shaders.clear();
        ShaderPrograms.clear();
        m_Uniforms.clear();
    }
    static const bool IsDebug = false;
    inline static std::vector<std::pair<ShaderProgramLocation, GLuint>> ShaderPrograms;
    inline static std::vector<std::pair<UniformLocation, GLint>> m_Uniforms;
    inline static std::vector<std::pair<std::string, GLuint>> Shaders;
    inline static GLuint CreateShader(std::string_view _vertexShader, std::string_view _geoShader, std::string_view _fragmentShader)
    {
        for (auto& item : ShaderPrograms)
        {
            if (item.first.vertShader == _vertexShader.data() && item.first.geoShader == _geoShader.data() && item.first.fragShader == _fragmentShader.data())
            {
                Print("Re-used Shader Program " + std::to_string(item.second) + "!");
                return item.second;
            }
        }

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

        ShaderPrograms.push_back(std::make_pair(ShaderProgramLocation{ _vertexShader.data(), _geoShader.data(), _fragmentShader.data() }, program));

        // Return Program ID
        return program;
    }
    inline static GLuint CreateShader(std::string_view _vertexShader, std::string_view _fragmentShader)
    {
        for (auto& item : ShaderPrograms)
        {
            if (item.first.vertShader == _vertexShader && item.first.geoShader == "" && item.first.fragShader == _fragmentShader)
            {
                Print("Re-used Shader Program " + std::to_string(item.second) + "!");
                return item.second;
            }
        }

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

        ShaderPrograms.push_back(std::make_pair(ShaderProgramLocation{ _vertexShader.data(), "", _fragmentShader.data() }, program));

        // Return Program ID
        return program;
    }
    inline static void SetUniform1i(const GLuint& _program, std::string_view _location, GLint _value)
    {
        GLint location; 
        for (auto& item : m_Uniforms)
        {
            if (item.first.program == _program && item.first.location == _location.data())
            {
                glUniform1i(item.second, _value);
                return;
            }
        }
        m_Uniforms.push_back(std::make_pair(UniformLocation{ _program, _location.data()}, glGetUniformLocation(_program, _location.data())));
        glUniform1i(m_Uniforms.back().second , _value);
    }
    inline static void SetUniform1f(const GLuint& _program, std::string_view _location, GLfloat _value)
    {
        GLint location;
        for (auto& item : m_Uniforms)
        {
            if (item.first.program == _program && item.first.location == _location.data())
            {
                glUniform1f(item.second, _value);
                return;
            }
        }
        m_Uniforms.push_back(std::make_pair(UniformLocation{ _program, _location.data() }, glGetUniformLocation(_program, _location.data())));
        glUniform1f(m_Uniforms.back().second, _value);
    }
    inline static void SetUniform2i(const GLuint& _program, std::string_view _location, GLint _value, GLint _value2)
    {
        GLint location;
        for (auto& item : m_Uniforms)
        {
            if (item.first.program == _program && item.first.location == _location.data())
            {
                glUniform2i(item.second, _value, _value2);
                return;
            }
        }
        m_Uniforms.push_back(std::make_pair(UniformLocation{ _program, _location.data() }, glGetUniformLocation(_program, _location.data())));
        glUniform2i(m_Uniforms.back().second, _value, _value2);
    }
    inline static void SetUniform2i(const GLuint& _program, std::string_view _location, GLfloat _value, GLfloat _value2)
    {
        GLint location;
        for (auto& item : m_Uniforms)
        {
            if (item.first.program == _program && item.first.location == _location.data())
            {
                glUniform2f(item.second, _value, _value2);
                return;
            }
        }
        m_Uniforms.push_back(std::make_pair(UniformLocation{ _program, _location.data() }, glGetUniformLocation(_program, _location.data())));
        glUniform2f(m_Uniforms.back().second, _value, _value2);
    }
    inline static void SetUniform3i(const GLuint& _program, std::string_view _location, GLint _value, GLint _value2, GLint _value3)
    {
        GLint location;
        for (auto& item : m_Uniforms)
        {
            if (item.first.program == _program && item.first.location == _location.data())
            {
                glUniform3i(item.second, _value, _value2, _value3);
                return;
            }
        }
        m_Uniforms.push_back(std::make_pair(UniformLocation{ _program, _location.data() }, glGetUniformLocation(_program, _location.data())));
        glUniform3i(m_Uniforms.back().second, _value, _value2, _value3);
    }
    inline static void SetUniform3f(const GLuint& _program, std::string_view _location, GLfloat _value, GLfloat _value2, GLfloat _value3)
    {
        GLint location;
        for (auto& item : m_Uniforms)
        {
            if (item.first.program == _program && item.first.location == _location.data())
            {
                glUniform3f(item.second, _value, _value2, _value3);
                return;
            }
        }
        m_Uniforms.push_back(std::make_pair(UniformLocation{ _program, _location.data() }, glGetUniformLocation(_program, _location.data())));
        glUniform3f(m_Uniforms.back().second, _value, _value2, _value3);
    }
    inline static void SetUniform3fv(const GLuint& _program, std::string_view _location, const glm::vec3& _value)
    {
        GLint location;
        for (auto& item : m_Uniforms)
        {
            if (item.first.program == _program && item.first.location == _location.data())
            {
                glUniform3fv(item.second, 1, glm::value_ptr(_value));
                return;
            }
        }
        m_Uniforms.push_back(std::make_pair(UniformLocation{ _program, _location.data() }, glGetUniformLocation(_program, _location.data())));
        glUniform3fv(m_Uniforms.back().second, 1, glm::value_ptr(_value));
    }
    inline static void SetUniform3iv(const GLuint& _program, std::string_view _location, const glm::ivec3& _value)
    {
        GLint location;
        for (auto& item : m_Uniforms)
        {
            if (item.first.program == _program && item.first.location == _location.data())
            {
                glUniform3iv(item.second, 1, glm::value_ptr(_value));
                return;
            }
        }
        m_Uniforms.push_back(std::make_pair(UniformLocation{ _program, _location.data() }, glGetUniformLocation(_program, _location.data())));
        glUniform3iv(m_Uniforms.back().second, 1, glm::value_ptr(_value));
    }
    inline static void SetUniformMatrix4fv(const GLuint& _program, std::string_view _location, const glm::mat4& _value)
    {
        GLint location;
        for (auto& item : m_Uniforms)
        {
            if (item.first.program == _program && item.first.location == _location.data())
            {
                glUniformMatrix4fv(item.second, 1, GL_FALSE, glm::value_ptr(_value));
                return;
            }
        }
        m_Uniforms.push_back(std::make_pair(UniformLocation{ _program, _location.data() }, glGetUniformLocation(_program, _location.data())));
        glUniformMatrix4fv(m_Uniforms.back().second, 1, GL_FALSE, glm::value_ptr(_value));
    }
private:
    inline static GLuint CompileShader(GLenum _type, std::string _source)
    {
        for (auto& item : Shaders)
        {
            if (item.first == _source)
            {
                Print("Re-used Shader " + std::to_string(item.second) + "!");
                return item.second;
            }
        }

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
            std::string debugOutput = "Failed to Compile Vertex Shader";
            debugOutput += message;
            Print(debugOutput);
            glDeleteShader(shader);
            return result;
        }

        Shaders.push_back(std::make_pair(_source, shader));
        
        return shader;
    }
    inline static std::string PassFileToString(std::string_view _fileAddress)
    {
        std::string content;
        std::ifstream fileStream(_fileAddress.data(), std::ios::in);

        if (!fileStream.is_open()) 
        {
            std::string debugOutput = "Could not read file ";
            debugOutput += _fileAddress.data();
            debugOutput += ". File does not exist.";
            Print(debugOutput);
            return "";
        }
        std::string line = "";
        while (!fileStream.eof()) 
        {
            std::getline(fileStream, line);
            content.append(line + "\n");
        }

        fileStream.close();
        return content;
    }
};

