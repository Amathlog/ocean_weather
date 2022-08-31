#include <shader.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <array>
#include <cassert>
#include <iostream>

namespace {

//------------------------------------------------------------------------------
//
using InfoLog = std::array<char, 1024>;

//------------------------------------------------------------------------------
//
constexpr GLuint S_INVALID_SHADER  = ~GLuint();
constexpr GLuint S_INVALID_PROGRAM = 0;

//------------------------------------------------------------------------------
//
template <typename... Args>
void reportError(Args const&... args) {
    ((std::cerr << args), ...);
    std::cerr << std::endl;
}

//------------------------------------------------------------------------------
//
GLuint createShader(std::string_view shaderText, GLenum shaderType) {
    // Create shader object
    GLuint shader = glCreateShader(shaderType);

    // Set the source
    GLchar const* front = shaderText.data();
    glShaderSource(shader, 1, &front, NULL);

    // Compile the shader
    glCompileShader(shader);

    // Check if shader object compiled successfully
    GLint testVal;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &testVal);
    if (testVal == GL_FALSE) {
        InfoLog infoLog;
        glGetShaderInfoLog(shader, infoLog.size(), NULL, infoLog.data());
        reportError("Failed to compile shader \n", shaderText, '\n',
                    "with the following errors:\n",
                    std::string_view{infoLog.data(), infoLog.size()});
        glDeleteShader(shader);
        return S_INVALID_SHADER;
    }
    return shader;
}

//------------------------------------------------------------------------------
//
GLuint createProgram(std::string_view vert, std::string_view frag) {
    // Create and compile the vertex shader
    auto vertShaderId = createShader(vert, GL_VERTEX_SHADER);
    if (vertShaderId == S_INVALID_SHADER) return S_INVALID_PROGRAM;

    // Create and compile the fragment shader
    auto fragShaderId = createShader(frag, GL_FRAGMENT_SHADER);
    if (fragShaderId == S_INVALID_SHADER) return S_INVALID_PROGRAM;

    //  Create a shader program object
    auto program = glCreateProgram();
    static_assert(std::is_same_v<decltype(program), GLuint>);

    // Attach the shader objects to the shader program object
    glAttachShader(program, vertShaderId);
    glAttachShader(program, fragShaderId);

    // Link both shader objects to the program object
    glLinkProgram(program);

    // Make sure link had no errors
    GLint testVal;
    glGetProgramiv(program, GL_LINK_STATUS, &testVal);
    if (testVal == GL_FALSE) {
        InfoLog infoLog;
        glGetProgramInfoLog(program, infoLog.size(), NULL, infoLog.data());
        reportError("Failed to link shaders \n", vert, "\nand\n", frag,
                    "with the following errors:\n",
                    std::string_view{infoLog.data(), infoLog.size()});
        glDeleteProgram(program);
        program = S_INVALID_PROGRAM;
    }
    glDeleteShader(vertShaderId);
    glDeleteShader(fragShaderId);
    return program;
}

} // namespace

namespace OceanWeather {

//------------------------------------------------------------------------------
//
ShaderProgram::ShaderProgram(std::string_view vert, std::string_view frag)
    : m_program(createProgram(vert, frag)) {}

//------------------------------------------------------------------------------
//
void ShaderProgram::use() {
    assert(isValid());
    glUseProgram(m_program);
}

//------------------------------------------------------------------------------
//
void ShaderProgram::destroy() {
    if (m_program != S_INVALID_PROGRAM) {
        glDeleteProgram(m_program);
        m_program = S_INVALID_PROGRAM;
    }
}

} // namespace OceanWeather
