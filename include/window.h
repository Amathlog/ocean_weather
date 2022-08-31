#pragma once

struct GLFWwindow;

namespace OceanWeather {

class Window {
public:
    Window(const char* name, unsigned width, unsigned height);
    ~Window();

    void Update();
    bool ShouldClose();

    explicit operator bool() const { return m_window != nullptr; }

private:
    GLFWwindow* m_window = nullptr;
};
} // namespace OceanWeather
