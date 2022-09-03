#pragma once

#include <optional>
#include <string>
#include <string_view>

namespace OceanWeather {

class FileUtils {
public:
    static std::optional<std::string> readText(std::string_view filename);
    static std::string                getRelativeRoot();

private:
    FileUtils()  = delete;
    ~FileUtils() = delete;
};

} // namespace OceanWeather
