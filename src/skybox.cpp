#include <skybox.h>

#include <camera.h>
#include <fileutils.h>
#include <image.h>
#include <shader.h>

#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <glm/gtx/transform.hpp>

#include <atomic>
#include <filesystem>

#include <iostream>

namespace {

using namespace std::string_view_literals;

//------------------------------------------------------------------------------
//
constexpr std::array<GLenum, 6> s_cubeMapTarget = {
    GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z};

//------------------------------------------------------------------------------
//
constexpr std::array<float, 108> s_skyVertices{
    -1.0f, +1.0f, -1.0f, //
    -1.0f, -1.0f, -1.0f, //
    +1.0f, -1.0f, -1.0f, //
    +1.0f, -1.0f, -1.0f, //
    +1.0f, +1.0f, -1.0f, //
    -1.0f, +1.0f, -1.0f, //
                         //
    -1.0f, -1.0f, +1.0f, //
    -1.0f, -1.0f, -1.0f, //
    -1.0f, +1.0f, -1.0f, //
    -1.0f, +1.0f, -1.0f, //
    -1.0f, +1.0f, +1.0f, //
    -1.0f, -1.0f, +1.0f, //
                         //
    +1.0f, -1.0f, -1.0f, //
    +1.0f, -1.0f, +1.0f, //
    +1.0f, +1.0f, +1.0f, //
    +1.0f, +1.0f, +1.0f, //
    +1.0f, +1.0f, -1.0f, //
    +1.0f, -1.0f, -1.0f, //
                         //
    -1.0f, -1.0f, +1.0f, //
    -1.0f, +1.0f, +1.0f, //
    +1.0f, +1.0f, +1.0f, //
    +1.0f, +1.0f, +1.0f, //
    +1.0f, -1.0f, +1.0f, //
    -1.0f, -1.0f, +1.0f, //
                         //
    -1.0f, +1.0f, -1.0f, //
    +1.0f, +1.0f, -1.0f, //
    +1.0f, +1.0f, +1.0f, //
    +1.0f, +1.0f, +1.0f, //
    -1.0f, +1.0f, +1.0f, //
    -1.0f, +1.0f, -1.0f, //
                         //
    -1.0f, -1.0f, -1.0f, //
    -1.0f, -1.0f, +1.0f, //
    +1.0f, -1.0f, -1.0f, //
    +1.0f, -1.0f, -1.0f, //
    -1.0f, -1.0f, +1.0f, //
    +1.0f, -1.0f, +1.0f  //
};

//------------------------------------------------------------------------------
//
/// \brief Variables in the shaders
constexpr const char* s_varPosition      = "position";
constexpr const char* s_varMvpMatrix     = "modelViewProjectionMatrix";
constexpr const char* s_varSkyBoxTexture = "SkyBoxTexture";

//------------------------------------------------------------------------------
//
constexpr std::string_view s_vertShader = R"RAW(
#version 330 core

// 1. declare attributes
layout (location=0) in vec3 position;

// 2. declare varying type which will transfer the texture coordinates to the
//    fragment shader
out vec3 vTexCoordinates;

// 3. declare a uniform that contains the model-View-projection
uniform mat4 modelViewProjectionMatrix;

void main() {
    // 4. Generate the UV coordinates
    vTexCoordinates = position.xyz;

    // 5. transform every position vertex by the model-view-projection matrix
    gl_Position = modelViewProjectionMatrix * vec4(position, 1);

    // 6. Trick to place the skybox behind any other 3D model
    gl_Position = vec4(gl_Position.xyww);
}
)RAW"sv;

//------------------------------------------------------------------------------
//
constexpr std::string_view s_fragShader = R"RAW(
#version 330 core

// 1. declare a uniform sampler2d that contains the texture data
uniform samplerCube SkyBoxTexture;

// 2. declare varying type which will transfer the texture coordinates from the
//    vertex shader
in vec3 vTexCoordinates;

void main() {
    // 3. set the final color to the output of the fragment shader
    gl_FragColor = textureCube(SkyBoxTexture, vTexCoordinates);
    // gl_FragColor = vec4(vTexCoordinates, 1);
}
)RAW"sv;

} // namespace

namespace OceanWeather {

//------------------------------------------------------------------------------
//
SkyBox::SkyBox(std::string dirname) : m_dirname(std::move(dirname)) {}

//------------------------------------------------------------------------------
//
SkyBox::~SkyBox() {}

//------------------------------------------------------------------------------
//
void SkyBox::draw(Camera const& camera) {
    // 1. Set the shader program
    m_program.use();

    // 10. Assign the model-world-view-projection matrix data to the uniform
    // location:modelviewProjectionUniformLocation
    glm::mat4 view   = glm::mat4(glm::mat3(camera.getView()));
    auto      matrix = camera.getProjection() * (view * m_model);
    glUniformMatrix4fv(modelViewProjectionUniformLocation, 1, 0, &matrix[0][0]);

    // 2. Bind the VAO
    glBindVertexArray(vertexArrayObject);

    // 3. Activate the texture unit
    glActiveTexture(GL_TEXTURE0);

    // 4 Bind the texture object
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID[0]);

    // 5. Specify the value of the UV Map uniform
    glUniform1i(UVMapUniformLocation, 0);

    // 6. draw the pixels if the incoming depth value is less than or equal to
    // the stored depth value.
    glDepthFunc(GL_LEQUAL);

    // 7. Start the rendering process
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 8. draw the pixels if the incoming depth value is less than the stored
    // depth value
    glDepthFunc(GL_LESS);

    // 9. Disable the VAO
    glBindVertexArray(0);
}

//------------------------------------------------------------------------------
//
bool SkyBox::setupOpenGL() {
    // load the shaders, compile them and link them
    m_program = ShaderProgram(s_vertShader, s_fragShader);

    // 1. Generate a Vertex Array Object
    glGenVertexArrays(1, &vertexArrayObject);
    // glGenVertexArraysOES(1,&vertexArrayObject);

    // 2. Bind the Vertex Array Object
    glBindVertexArray(vertexArrayObject);
    // glBindVertexArrayOES(vertexArrayObject);

    // 3. Generate a Vertex Buffer Object
    glGenBuffers(1, &vertexBufferObject);

    // 4. Bind the Vertex Buffer Object
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

    // 5. Dump the data into the Buffer
    // Read "Loading data into OpenGL Buffers" if not familiar with loading data
    // using glBufferSubData.
    // http://www.www.haroldserrano.com/blog/loading-vertex-normal-and-uv-data-onto-opengl-buffers
    constexpr auto size = s_skyVertices.size() * sizeof(s_skyVertices[0]);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);

    // 5a. Load vertex data with glBufferSubData
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_skyVertices.data());

    // 6. Get the location of the shader attribute called "position"
    auto positionLocation = glGetAttribLocation(m_program.id(), s_varPosition);

    // 8. Enable attribute locations

    // 8a. Enable the position attribute
    glEnableVertexAttribArray(positionLocation);

    // 9. Link the buffer data to the shader attribute locations

    // 9a. Link the buffer data to the shader's position location
    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // 10. Get Location of uniform
    modelViewProjectionUniformLocation =
        glGetUniformLocation(m_program.id(), s_varMvpMatrix);

    // Since we are going to start the rendering process by using glDrawElements
    // We are going to create a buffer for the indices. Read "Starting the
    // rendering process in OpenGL" if not familiar.
    // http://www.www.haroldserrano.com/blog/starting-the-primitive-rendering-process-in-opengl

    // 14. Activate GL_TEXTURE0
    glActiveTexture(GL_TEXTURE0);

    // 15.a Generate a texture buffer
    glGenTextures(1, &textureID[0]);

    // 16. Bind texture0
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID[0]);

    // 1. load each texture image reference into a vector
    std::string prefix = FileUtils::getRelativeRoot() + "assets/cubemaps/";

    std::array<std::string_view, 6> cubeMapTextures{"posx.jpg"sv, "negx.jpg"sv,
                                                    "posy.jpg"sv, "negy.jpg"sv,
                                                    "posz.jpg"sv, "negz.jpg"sv};

    std::string dir;
    dir.reserve(prefix.size() + m_dirname.size() + 1);
    dir += prefix;
    dir += m_dirname;

    // 17. Simple For loop to get each image reference
    for (unsigned i = 0; i < cubeMapTextures.size(); i++) {
        // 17a.Decode each cube map image into its raw image data.
        auto image = Image(dir, cubeMapTextures[i]);
        if (image) {
            // 17c. load the image data into the current bound texture buffer
            // cubeMapTarget[] contains the cube map targets
            // Note: Images are loaded as BGR, not RGB
            glTexImage2D(s_cubeMapTarget[i], 0, GL_RGB, image.width(),
                         image.height(), 0, GL_BGR, GL_UNSIGNED_BYTE,
                         image.data());
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // 18. Get the location of the Uniform Sampler2D
    UVMapUniformLocation =
        glGetUniformLocation(m_program.id(), s_varSkyBoxTexture);

    // 19. Unbind the VAO
    glBindVertexArray(0);

    return true;
}

//------------------------------------------------------------------------------
//
void SkyBox::tearDownOpenGL() {
    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteVertexArrays(1, &vertexArrayObject);
    m_program = ShaderProgram();
}

} // namespace OceanWeather
