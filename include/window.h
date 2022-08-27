#pragma once

class GLFWwindow;

namespace OceanWeather {

class Window {
   public:
    Window(const char* name, unsigned width, unsigned height);
    ~Window();

    void Update();
    bool ShouldClose();

   private:
    GLFWwindow* m_window = nullptr;
};
}  // namespace OceanWeather