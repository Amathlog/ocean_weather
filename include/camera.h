#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace OceanWeather {

class Camera {
public:
    Camera();

    void setPerspective(float fov, float aspect, float near, float far);
    void setAspect(float aspect);

    float getFov() const { return m_fov; }
    float getAspect() const { return m_aspect; }
    float getNear() const { return m_near; }
    float getFar() const { return m_far; }

    void translate(glm::vec3 const& translation);
    void addRotation(float theta, float phi);
    void addDistance(float distance);

    glm::mat4 const& getView() const { return m_view; }
    glm::mat4 const& getProjection() const { return m_projection; }

private:
    void updateView();
    void updateProjection();

    float m_fov;
    float m_aspect;
    float m_near;
    float m_far;

    float     m_theta    = 0;
    float     m_phi      = .5;
    float     m_distance = 5;
    glm::vec3 m_center{0, 1, 0};

    glm::mat4 m_view;
    glm::mat4 m_projection;
};
} // namespace OceanWeather
