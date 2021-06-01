#pragma once

#include <string>
#include "information.hpp"
#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>

namespace read {
    // Read a BMP image file
    BMPImage read_bmp(const std::string& path);
    
    // Write a BMP image file
    void write_bmp(const std::string& path, const BMPImage& image);
}
