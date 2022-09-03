#include <fileutils.h>

#include <fstream>
#include <optional>
#include <sstream>

namespace OceanWeather {

//------------------------------------------------------------------------------
//
std::optional<std::string> FileUtils::readText(std::string_view filename) {
    std::ifstream file{filename.data(), std::ios::in};
    if (!file.is_open()) return std::nullopt;

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string FileUtils::getRelativeRoot() { return "../../"; }

} // namespace OceanWeather
