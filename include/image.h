#pragma once

#include <string_view>
#include <vector>

namespace OceanWeather {

class Image {
public:
    Image(std::string_view dirname, std::string_view filename);

    explicit operator bool() const { return m_width != 0; }

    unsigned width() const { return m_width; }
    unsigned height() const { return m_height; }
    unsigned char const* data() const { return m_data.data(); }

private:
    unsigned m_width = 0, m_height = 0;
    std::vector<unsigned char> m_data;
};

} // namespace OceanWeather
