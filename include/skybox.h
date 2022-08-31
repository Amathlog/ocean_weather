#pragma once

#include <string>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <array>

namespace OceanWeather {

class SkyBox {
public:
    SkyBox(std::string dirname);
    ~SkyBox();

    void draw();
    bool setupOpenGL();

private:
    // method to decompress image
    bool convertImageToRawImage(const char* uTexture);
    void loadShaders(const char* vShader, const char* fShader);

    std::string m_dirname;

    std::array<GLuint, 16> textureID; // Array for textures

    GLuint programObject;      // program object used to link shaders
    GLuint vertexArrayObject;  // Vertex Array Object
    GLuint vertexBufferObject; // Vertex Buffer Object

    GLint UVMapUniformLocation; // OpenGL location for the Texture Map
};
} // namespace OceanWeather
