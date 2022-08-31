#include <window.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <iostream>

namespace {
void framebuffer_size_callback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}
} // namespace

namespace OceanWeather {

//------------------------------------------------------------------------------
//
Window::Window(const char* name, unsigned width, unsigned height) {
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
    framebuffer_size_callback(m_window, width, height);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
}

//------------------------------------------------------------------------------
//
Window::~Window() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

//------------------------------------------------------------------------------
//
void Window::Update() {
    if (ShouldClose()) return;

    glfwPollEvents();

    glClearColor(0.5f, 0.5f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw something...

    // Swap buffers
    glfwSwapBuffers(m_window);
}

//------------------------------------------------------------------------------
//
bool Window::ShouldClose() {
    return m_window ? glfwWindowShouldClose(m_window) : true;
}
} // namespace OceanWeather
