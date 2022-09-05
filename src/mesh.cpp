#include <mesh.h>

#include <camera.h>
#include <fileutils.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <array>
#include <sstream>

namespace {

using namespace std::string_view_literals;

//------------------------------------------------------------------------------
//
constexpr const char* s_pos = "pos";
constexpr const char* s_mvp = "mvp";

//------------------------------------------------------------------------------
//
constexpr std::string_view s_vertShader = R"RAW(
#version 330 core

in vec3 pos;

out vec3 color;

uniform mat4 mvp;

void main() {
    color = pos / 6.f + vec3(.5f, .5f, .5f);
    gl_Position = mvp * vec4(pos, 1);
}
)RAW"sv;

//------------------------------------------------------------------------------
//
constexpr std::string_view s_fragShader = R"RAW(
#version 330 core

in vec3 color;

void main() {
    gl_FragColor = vec4(color, 1);
}
)RAW"sv;

} // namespace

namespace OceanWeather {

//------------------------------------------------------------------------------
//
Mesh::Mesh()  = default;
Mesh::~Mesh() = default;

//------------------------------------------------------------------------------
//
std::unique_ptr<Mesh> Mesh::loadObj(std::string_view path) {
    auto objTxt = FileUtils::readText(path);
    assert(objTxt);

    std::unique_ptr<Mesh> mesh_ptr = std::make_unique<Mesh>();
    auto& mesh = *mesh_ptr;
    std::istringstream iss{*objTxt};
    while (!iss.eof()) {
        char kind;
        iss >> kind;
        if (kind == 'v') {
            float x, y, z;
            iss >> x >> y >> z;
            mesh.m_vertices.push_back(x);
            mesh.m_vertices.push_back(y);
            mesh.m_vertices.push_back(z);
        } else if (kind == 'f') {
            int ids[3];
            iss >> ids[0] >> ids[1] >> ids[2];
            for (auto id : ids) {
                if (id < 0) id = (int)mesh.m_vertices.size() + id + 1;
                id -= 1;
                mesh.m_indices.push_back(static_cast<unsigned>(id));
            }
        }
    }
    return mesh_ptr;
}

//------------------------------------------------------------------------------
//
void Mesh::draw(Camera const& camera) {
    // 1. Set the shader program
    m_program.use();

    // 10. Assign the model-world-view-projection matrix data to the uniform
    // location:modelviewProjectionUniformLocation
    auto matrix = camera.getProjection() * (camera.getView());
    glUniformMatrix4fv(mvp, 1, 0, &matrix[0][0]);

    // 2. Bind the VAO
    glBindVertexArray(m_vao);

    // 7. Start the rendering process
    glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT,
                   nullptr);

    // 9. Disable the VAO
    glBindVertexArray(0);
}

//------------------------------------------------------------------------------
//
void Mesh::setupOpenGL() {
    // load the shaders, compile them and link them
    m_program = ShaderProgram(s_vertShader, s_fragShader);

    // Setup buffers
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    unsigned vSizeBytes = (unsigned)(m_vertices.size() * sizeof(m_vertices[0]));
    glBufferData(GL_ARRAY_BUFFER, vSizeBytes, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vSizeBytes, m_vertices.data());

    // 11. Create a new buffer for the indices
    unsigned iSizeBytes = (unsigned)(m_indices.size() * sizeof(m_indices[0]));
    GLuint             elementBuffer;
    glGenBuffers(1, &elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSizeBytes, m_indices.data(),
                 GL_STATIC_DRAW);

    // 6. Get the location of the shader attribute called "position"
    auto posLoc = glGetAttribLocation(m_program.id(), s_pos);
    assert(posLoc != 1);

    // 8. Enable attribute locations
    // 8a. Enable the position attribute
    glEnableVertexAttribArray(posLoc);

    // 9. Link the buffer data to the shader attribute locations
    // 9a. Link the buffer data to the shader's position location
    glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);

    // 10. Get Location of uniform
    mvp = glGetUniformLocation(m_program.id(), s_mvp);

    glBindVertexArray(0);
}

//------------------------------------------------------------------------------
//
void Mesh::tearDownOpenGL() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
    m_program = ShaderProgram();
}

} // namespace OceanWeather
