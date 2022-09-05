#include <window.h>

#include <camera.h>
#include <fileutils.h>
#include <mesh.h>
#include <skybox.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <iostream>

namespace OceanWeather {

//------------------------------------------------------------------------------
//
class Window::Impl {
public:
    static void framebuffer_size_callback(GLFWwindow* window,
                                          int         width,
                                          int         height);
    static void key_callback(
        GLFWwindow* window, int key, int scancode, int action, int mods);

    static void scroll_callback(GLFWwindow* window,
                                double      xoffset,
                                double      yoffset);

    static void mouse_button_callback(GLFWwindow* window,
                                      int         button,
                                      int         action,
                                      int         mods);

    Impl(const char* name, unsigned width, unsigned height);
    ~Impl();

    void Update();
    bool ShouldClose();

private:
    GLFWwindow* m_window = nullptr;
    double      lastTime = 0;
    glm::dvec2  lastCursor;

    std::unique_ptr<Camera> m_camera;
    std::unique_ptr<SkyBox> m_skybox;
    std::unique_ptr<Mesh>   m_mesh;
};

//------------------------------------------------------------------------------
//
void Window::Impl::framebuffer_size_callback(GLFWwindow* window,
                                             int         width,
                                             int         height) {
    assert(glfwGetCurrentContext() == window);
    auto impl = static_cast<Impl*>(glfwGetWindowUserPointer(window));
    assert(impl);
    impl->m_camera->setAspect(static_cast<float>(width) / height);

    glViewport(0, 0, width, height);
}

//------------------------------------------------------------------------------
//
void Window::Impl::key_callback(
    GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}

//------------------------------------------------------------------------------
//
void Window::Impl::scroll_callback(GLFWwindow* window, double, double yoffset) {
    auto impl = static_cast<Impl*>(glfwGetWindowUserPointer(window));
    assert(impl);
    Camera& camera = *impl->m_camera;
    camera.addDistance(-(float)yoffset);
}

//------------------------------------------------------------------------------
//
void Window::Impl::mouse_button_callback(GLFWwindow* window,
                                         int         button,
                                         int         action,
                                         int) {
    auto impl = static_cast<Impl*>(glfwGetWindowUserPointer(window));
    assert(impl);
    if ((button == GLFW_MOUSE_BUTTON_LEFT ||
         button == GLFW_MOUSE_BUTTON_MIDDLE) &&
        action == GLFW_PRESS) {
        auto& pos = impl->lastCursor;
        glfwGetCursorPos(window, &pos.x, &pos.y);
    }
}

//------------------------------------------------------------------------------
//
Window::Impl::Impl(const char* name, unsigned width, unsigned height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, name, nullptr, nullptr);
    if (m_window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glfwSetWindowUserPointer(m_window, this);
    m_camera = std::make_unique<Camera>();

    framebuffer_size_callback(m_window, width, height);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    glfwSetKeyCallback(m_window, key_callback);
    glfwSetScrollCallback(m_window, scroll_callback);
    glfwSetMouseButtonCallback(m_window, mouse_button_callback);

    m_skybox = std::make_unique<SkyBox>("yokohama2");
    m_skybox->setupOpenGL();

    m_mesh = Mesh::loadObj(FileUtils::getRelativeRoot() +
                           "assets/models/teapot.obj");
    m_mesh->setupOpenGL();

    // 3. Enable Depth Testing
    glEnable(GL_DEPTH_TEST);
}

//------------------------------------------------------------------------------
//
Window::Impl::~Impl() {
    m_skybox->tearDownOpenGL();
    m_mesh->tearDownOpenGL();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

//------------------------------------------------------------------------------
//
void Window::Impl::Update() {
    if (ShouldClose()) return;

    glfwPollEvents();

    auto curTime = glfwGetTime();
    // auto dt      = static_cast<float>(curTime - lastTime);
    lastTime = curTime;

    glm::dvec2 curCursor;
    glfwGetCursorPos(m_window, &curCursor.x, &curCursor.y);
    glm::vec2 dPos = curCursor - lastCursor;
    lastCursor     = curCursor;

    auto& camera = *m_camera;
    if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        constexpr float scale = .01f;
        camera.addRotation(dPos.x * scale, dPos.y * scale);
    }
    if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
        constexpr float scale = .01f;
        camera.translate(glm::vec3{-dPos.x * scale, dPos.y * scale, 0});
    }

    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glClearDepth(1.f);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Draw something...
    m_skybox->draw(*m_camera);
    m_mesh->draw(*m_camera);

    // Swap buffers
    glfwSwapBuffers(m_window);
}

//------------------------------------------------------------------------------
//
bool Window::Impl::ShouldClose() {
    return m_window ? glfwWindowShouldClose(m_window) : true;
}

//------------------------------------------------------------------------------
//
Window::Window(const char* name, unsigned width, unsigned height)
    : m_impl(std::make_unique<Impl>(name, width, height)) {}

//------------------------------------------------------------------------------
//
Window::~Window() = default;

//------------------------------------------------------------------------------
//
void Window::Update() { m_impl->Update(); }

//------------------------------------------------------------------------------
//
bool Window::ShouldClose() { return m_impl->ShouldClose(); }

} // namespace OceanWeather
