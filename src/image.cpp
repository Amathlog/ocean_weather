#include <image.h>

#include <CImg.h>

#include <iostream>

namespace OceanWeather {

Image::Image(std::string_view dirname, std::string_view filename) {
    std::string filepath;
    filepath.reserve(dirname.size() + filename.size() + 1);
    filepath += dirname;
    filepath += '/';
    filepath += filename;

    using namespace cimg_library;

    CImg<unsigned char> src(filepath.c_str());
    m_width   = src.width();
    m_height  = src.height();
    m_data.reserve(m_width * m_height * 3);

    for (unsigned i = 0; i < m_width; ++i) {
        for (unsigned j = 0; j < m_height; ++j) {
            for (unsigned c = 0; c < 3; ++c) {
                auto data = src.data(j, i, 0, c);
                m_data.push_back(*(data+0));
            }
        }
    }

    // auto data = src.data();
    // m_data.insert(m_data.begin(), data, data + src.size());

    // std::cerr << m_width << ", " << m_height << ", " << src.size() << " vs " << (m_width * m_height * 3) << std::endl;
    // std::cerr << src.depth() << ", " << src.spectrum() << std::endl;
}

} // namespace OceanWeather
