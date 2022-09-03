#pragma once

#include <shader.h>

#include <vector>
#include <memory>

namespace OceanWeather {

class Camera;

class Mesh {
public:
    static std::unique_ptr<Mesh> loadObj(std::string_view path);

    Mesh();
    ~Mesh();

    void draw(Camera const& camera);
    void setupOpenGL();
    void tearDownOpenGL();

private:
    std::vector<float>    m_vertices;
    std::vector<unsigned> m_indices;

    ShaderProgram m_program;
    unsigned m_vao;
    unsigned m_vbo;

    int mvp;
};

} // namespace OceanWeather
