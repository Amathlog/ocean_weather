#pragma once

#include <string>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>

#include <shader.h>

#include <array>

namespace OceanWeather {

class Camera;

class SkyBox {
public:
    SkyBox(std::string dirname);
    ~SkyBox();

    void draw(Camera const& camera);
    bool setupOpenGL();
    void tearDownOpenGL();

private:
    std::string m_dirname;

    std::array<GLuint, 16> textureID; // Array for textures

    ShaderProgram m_program;
    GLuint vertexArrayObject;  // Vertex Array Object
    GLuint vertexBufferObject; // Vertex Buffer Object

    GLint modelViewProjectionUniformLocation;
    GLint UVMapUniformLocation; // OpenGL location for the Texture Map
    glm::mat4x4 m_model{1.f};
};
} // namespace OceanWeather
