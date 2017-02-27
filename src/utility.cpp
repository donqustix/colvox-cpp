#include "utility.h"

#include <fstream>
#include <stdexcept>

std::string minecpp::utility::readFile(const std::string& filepath)
{
    std::ifstream stream{filepath, std::ios::in | std::ios::binary};
    if (!stream)
        throw std::runtime_error{"file reading error"};

    std::ostringstream sstream;
    sstream << stream.rdbuf();

    return sstream.str();
}

