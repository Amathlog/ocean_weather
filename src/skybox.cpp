#include <skybox.h>

#include <CImg.h>

#include <GLFW/glfw3.h>

#include <fileutils.h>

#include <shader.h>

#include <atomic>

namespace {

// Cube Map Texture Object Target
constexpr std::array<GLenum, 6> s_cubeMapTarget = {
    GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z};

constexpr std::array<float, 108> s_skyVertices{
    +1.000000, -1.000000, -1.000000, -1.000000, -1.000000, -1.000000, -1.000000,
    +1.000000, -1.000000, -1.000000, +1.000000, +1.000000, -1.000000, -1.000000,
    +1.000000, +0.999999, -1.000001, +1.000000, +1.000000, +1.000000, -1.000000,
    +1.000000, +0.999999, +1.000000, +0.999999, -1.000001, +1.000000, +1.000000,
    -1.000000, -1.000000, +0.999999, -1.000001, +1.000000, -1.000000, -1.000000,
    +1.000000, -1.000000, -1.000000, +1.000000, -1.000000, +1.000000, +1.000000,
    -1.000000, +1.000000, -1.000000, +1.000000, +1.000000, -1.000000, -1.000000,
    +1.000000, -1.000000, -1.000000, +1.000000, +1.000000, +1.000000, +1.000000,
    -1.000000, +1.000000, -1.000000, -1.000000, -1.000000, +1.000000, -1.000000,
    +1.000000, +0.999999, +1.000000, -1.000000, +1.000000, +1.000000, +0.999999,
    -1.000001, +1.000000, +1.000000, -1.000000, -1.000000, +1.000000, +1.000000,
    -1.000000, +0.999999, -1.000001, +1.000000, -1.000000, -1.000000, -1.000000,
    +1.000000, -1.000000, -1.000000, -1.000000, -1.000000, +1.000000, -1.000000,
    -1.000000, -1.000000, -1.000000, -1.000000, +1.000000, -1.000000, +1.000000,
    -1.000000, +1.000000, +0.999999, +1.000000, +1.000000, +1.000000, -1.000000,
    -1.000000, +1.000000, +1.000000};

constexpr std::array<int, 36> s_skyIndex{
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17,
    18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};

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
void SkyBox::draw() {
    // 1. Set the shader program
    glUseProgram(programObject);

    // 2. Bind the VAO
    glBindVertexArray(vertexArrayObject);
    // glBindVertexArrayOES(vertexArrayObject);

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
    glDrawElements(GL_TRIANGLES, s_skyIndex.size() / 4, GL_UNSIGNED_INT,
                   (void*)0);

    // 8. draw the pixels if the incoming depth value is less than the stored
    // depth value
    glDepthFunc(GL_LESS);

    // 9. Disable the VAO
    glBindVertexArray(0);
    // glBindVertexArrayOES(0);
}

//------------------------------------------------------------------------------
//
bool SkyBox::setupOpenGL() {
    /*

    // load the shaders, compile them and link them
    loadShaders("SkyShader.vsh", "SkyShader.fsh");

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
    glBufferData(GL_ARRAY_BUFFER, s_skyVertices.size(), NULL, GL_STATIC_DRAW);

    // 5a. Load vertex data with glBufferSubData
    glBufferSubData(GL_ARRAY_BUFFER, 0, s_skyVertices.size(),
                    s_skyVertices.data());

    // 6. Get the location of the shader attribute called "position"
    positionLocation = glGetAttribLocation(programObject, "position");

    // 8. Enable attribute locations

    // 8a. Enable the position attribute
    glEnableVertexAttribArray(positionLocation);

    // 9. Link the buffer data to the shader attribute locations

    // 9a. Link the buffer data to the shader's position location
    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0,
                          (const GLvoid*)0);

    // 10. Get Location of uniform
    modelViewProjectionUniformLocation =
        glGetUniformLocation(programObject, "modelViewProjectionMatrix");

    // Since we are going to start the rendering process by using glDrawElements
    // We are going to create a buffer for the indices. Read "Starting the
    // rendering process in OpenGL" if not familiar.
    // http://www.www.haroldserrano.com/blog/starting-the-primitive-rendering-process-in-opengl


    // 11. Create a new buffer for the indices
    GLuint elementBuffer;
    glGenBuffers(1, &elementBuffer);

    // 12. Bind the new buffer to binding point GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

    // 13. Load the buffer with the indices found in smallHouse1_index array
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, s_skyIndex.size(), s_skyIndex.data(),
                 GL_STATIC_DRAW);

    // 14. Activate GL_TEXTURE0
    glActiveTexture(GL_TEXTURE0);

    // 15.a Generate a texture buffer
    glGenTextures(1, &textureID[0]);

    // 16. Bind texture0
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID[0]);

    unsigned int image;
    // 17. Simple For loop to get each image reference
    for (int i = 0; i < cubeMapTextures.size(); i++) {
        // 17a.Decode each cube map image into its raw image data.
        if (convertImageToRawImage(cubeMapTextures.at(i))) {
            // 17b. if decompression was successful, set the texture parameters

            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER,
                            GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,
                            GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
                            GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
                            GL_CLAMP_TO_EDGE);

            // 17c. load the image data into the current bound texture buffer
            // cubeMapTarget[] contains the cube map targets
            glTexImage2D(s_cubeMapTarget[i], 0, GL_RGBA, imageWidth,
                         imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image);
        }
    }

    // 18. Get the location of the Uniform Sampler2D
    UVMapUniformLocation = glGetUniformLocation(programObject, "SkyBoxTexture");

    // 19. Unbind the VAO
    glBindVertexArray(0);
    // glBindVertexArrayOES(0);

    // setTransformation();
    */
    return true;
}

} // namespace OceanWeather
