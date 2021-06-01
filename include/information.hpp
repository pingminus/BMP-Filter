#pragma once

#include <cstdint>
#include <vector>

// BMP File Header (14 bytes)
struct BMPFileHeader {
    uint16_t signature;      // BM
    uint32_t file_size;      // File size
    uint8_t reserved1[2];    // Reserved
    uint8_t reserved2[2];    // Reserved
    uint32_t offset;         // Offset to pixel data
};

// BMP Info Header (40 bytes)
struct BMPInfoHeader {
    uint32_t size;           // Header size
    int32_t width;           // Width in pixels
    int32_t height;          // Height in pixels
    uint16_t planes;         // Always 1
    uint16_t bits_per_pixel; // 8, 16, 24, or 32
    uint32_t compression;    // Compression type
    uint32_t image_size;     // Can be 0
    int32_t x_pixels_per_meter;
    int32_t y_pixels_per_meter;
    uint32_t colors_used;
    uint32_t colors_important;
};

// Represent a BMP image
struct BMPImage {
    BMPFileHeader file_header;
    BMPInfoHeader info_header;
    std::vector<uint8_t> pixel_data;
};