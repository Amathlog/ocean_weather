#pragma once

#include <memory>

namespace OceanWeather {

class SkyBox;
class Camera;

class Window {
public:
    Window(const char* name, unsigned width, unsigned height);
    ~Window();

    void Update();
    bool ShouldClose();

    explicit operator bool() const { return m_impl != nullptr; }

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};
} // namespace OceanWeather
