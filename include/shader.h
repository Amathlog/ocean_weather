#pragma once

#include <string_view>
#include <optional>

namespace OceanWeather {

//==============================================================================
// CLASS ShaderProgram
//==============================================================================

class ShaderProgram {
public:
    ShaderProgram() = default;
    ShaderProgram(std::string_view vert, std::string_view frag);
    ~ShaderProgram() { destroy(); }

    // ShaderProgram cannot be copied
    ShaderProgram(ShaderProgram const&) = delete;
    ShaderProgram& operator=(ShaderProgram const&) = delete;

    // ShaderProgram can be moved
    ShaderProgram(ShaderProgram&& o) noexcept;
    ShaderProgram& operator=(ShaderProgram&& o) noexcept;

    bool     isValid() const { return m_program != 0; }
    explicit operator bool() const { return isValid(); }
    unsigned id() const { return m_program; }

    void use();

private:
    explicit ShaderProgram(unsigned program) : m_program(program) {}
    void destroy();
    unsigned m_program = 0;
};

//------------------------------------------------------------------------------
//
inline ShaderProgram::ShaderProgram(ShaderProgram&& o) noexcept
    : m_program(o.m_program) {
    o.m_program = 0;
}

//------------------------------------------------------------------------------
//
inline ShaderProgram& ShaderProgram::operator=(ShaderProgram&& o) noexcept {
    destroy();
    m_program   = o.m_program;
    o.m_program = 0;
    return *this;
}

} // namespace OceanWeather
