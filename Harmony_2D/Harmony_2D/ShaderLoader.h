#pragma once
#include "Helper.h"

class ShaderLoader
{
public:
    static const bool IsDebug = false;

    /// <summary>
    /// Cleans Up All Shader And Program Caches
    /// </summary>
    ~ShaderLoader();

    /// <summary>
    /// Creates A Given Shader Program Using Cache Optimization With The Specified Vertex Shader And Fragment Shader Paths And Returns Its ID.
    /// </summary>
    /// <param name="_vertexShader"></param>
    /// <param name="_fragmentShader"></param>
    /// <returns></returns>
    static GLuint CreateShader(std::string_view _vertexShader, std::string_view _fragmentShader);

    /// <summary>
    /// Sets Uniform 1i At Location Using Caching Optimization
    /// </summary>
    /// <param name="_program"></param>
    /// <param name="_location"></param>
    /// <param name="_value"></param>
    static void SetUniform1i(const GLuint& _program, std::string_view _location, GLint _value);
    
    /// <summary>
    /// Sets Uniform 1f At Location Using Caching Optimization
    /// </summary>
    /// <param name="_program"></param>
    /// <param name="_location"></param>
    /// <param name="_value"></param>
    static void SetUniform1f(const GLuint& _program, std::string_view _location, GLfloat _value);
    
    /// <summary>
    /// Sets Uniform 2i At Location Using Caching Optimization
    /// </summary>
    /// <param name="_program"></param>
    /// <param name="_location"></param>
    /// <param name="_value"></param>
    /// <param name="_value2"></param>
    static void SetUniform2i(const GLuint& _program, std::string_view _location, GLint _value, GLint _value2);
    
    /// <summary>
    /// Sets Uniform 2f At Location Using Caching Optimization
    /// </summary>
    /// <param name="_program"></param>
    /// <param name="_location"></param>
    /// <param name="_value"></param>
    /// <param name="_value2"></param>
    static void SetUniform2f(const GLuint& _program, std::string_view _location, GLfloat _value, GLfloat _value2);
    
    /// <summary>
    /// Sets Uniform 3i At Location Using Caching Optimization
    /// </summary>
    /// <param name="_program"></param>
    /// <param name="_location"></param>
    /// <param name="_value"></param>
    /// <param name="_value2"></param>
    /// <param name="_value3"></param>
    static void SetUniform3i(const GLuint& _program, std::string_view _location, GLint _value, GLint _value2, GLint _value3);
    
    /// <summary>
    /// Sets Uniform 3f At Location Using Caching Optimization
    /// </summary>
    /// <param name="_program"></param>
    /// <param name="_location"></param>
    /// <param name="_value"></param>
    /// <param name="_value2"></param>
    /// <param name="_value3"></param>
    static void SetUniform3f(const GLuint& _program, std::string_view _location, GLfloat _value, GLfloat _value2, GLfloat _value3);
    
    /// <summary>
    /// Sets Uniform 3fv At Location Using Caching Optimization
    /// </summary>
    /// <param name="_program"></param>
    /// <param name="_location"></param>
    /// <param name="_value"></param>
    static void SetUniform3fv(const GLuint& _program, std::string_view _location, const glm::vec3& _value);
    
    /// <summary>
    /// Sets Uniform 3iv At Location Using Caching Optimization
    /// </summary>
    /// <param name="_program"></param>
    /// <param name="_location"></param>
    /// <param name="_value"></param>
    static void SetUniform3iv(const GLuint& _program, std::string_view _location, const glm::ivec3& _value);
    
    /// <summary>
    /// Sets Uniform 4fv At Location Using Caching Optimization
    /// </summary>
    /// <param name="_program"></param>
    /// <param name="_location"></param>
    /// <param name="_value"></param>
    static void SetUniformMatrix4fv(const GLuint& _program, std::string_view _location, const glm::mat4& _value);
private:
    /// <summary>
    /// Compiles A Shader Of A Given Type And Source And Returns Its ID.
    /// </summary>
    /// <param name="_type"></param>
    /// <param name="_source"></param>
    /// <returns></returns>
    static GLuint CompileShader(GLenum _type, std::string _source);

    /// <summary>
    /// Passes A File At The Given Adress Into A String And Returns It.
    /// </summary>
    /// <param name="_fileAddress"></param>
    /// <returns></returns>
    static std::string PassFileToString(std::string_view _fileAddress);

    inline static std::vector<std::pair<ShaderProgramLocation, GLuint>> m_ShaderPrograms;
    inline static std::vector<std::pair<UniformLocation, GLint>> m_Uniforms;
    inline static std::vector<std::pair<std::string, GLuint>> m_Shaders;
};

