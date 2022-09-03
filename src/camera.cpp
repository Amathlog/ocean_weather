#include <camera.h>

#include <glm/gtx/transform.hpp>

namespace OceanWeather {

//------------------------------------------------------------------------------
//
Camera::Camera()
    : m_fov(glm::radians(70.f)),
      m_aspect(4.f / 3.f),
      m_near(.01f),
      m_far(1000.f),
      m_view(glm::mat4{1.f}) {
    updateView();
    updateProjection();
}

//------------------------------------------------------------------------------
//
void Camera::setPerspective(float fov, float aspect, float near, float far) {
    m_fov    = fov;
    m_aspect = aspect;
    m_near   = near;
    m_far    = far;
    updateProjection();
}

//------------------------------------------------------------------------------
//
void Camera::setAspect(float aspect) {
    m_aspect = aspect;
    updateProjection();
}

//------------------------------------------------------------------------------
//
void Camera::translate(glm::vec3 const& translation) {
    auto tmp = glm::vec4(translation, 1);
    auto r1  = glm::rotate(-m_theta, glm::vec3(0, 1, 0));
    auto r2  = glm::rotate(-m_phi, glm::vec3(1, 0, 0));
    tmp      = (r1 * r2) * tmp;
    m_center += glm::vec3(tmp);
    updateView();
}

//------------------------------------------------------------------------------
//
void Camera::addRotation(float theta, float phi) {
    m_theta += theta;
    m_phi += phi;
    updateView();
}

//------------------------------------------------------------------------------
//
void Camera::addDistance(float distance) {
    m_distance += distance;
    updateView();
}

//------------------------------------------------------------------------------
//
void Camera::updateView() {
    auto t1 = glm::translate(glm::mat4{1.f}, glm::vec3(0, 0, -m_distance));
    auto r1 = glm::rotate(m_theta, glm::vec3(0, 1, 0));
    auto r2 = glm::rotate(m_phi, glm::vec3(1, 0, 0));
    auto t2 = glm::translate(glm::mat4{1.f}, -m_center);
    m_view  = t1 * r2 * r1 * t2;
}

//------------------------------------------------------------------------------
//
void Camera::updateProjection() {
    m_projection = glm::perspective(m_fov, m_aspect, m_near, m_far);
}

} // namespace OceanWeather
