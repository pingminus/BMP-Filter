#include "../include/read.hpp"

namespace read {

    BMPImage read_bmp(const std::string& path) {
        BMPImage image;
        std::ifstream file(path, std::ios::binary);

        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << path << std::endl;
            return image;
        }

        // Read file header (14 bytes)
        file.read(reinterpret_cast<char*>(&image.file_header.signature), 2);
        file.read(reinterpret_cast<char*>(&image.file_header.file_size), 4);
        file.read(reinterpret_cast<char*>(image.file_header.reserved1), 2);
        file.read(reinterpret_cast<char*>(image.file_header.reserved2), 2);
        file.read(reinterpret_cast<char*>(&image.file_header.offset), 4);

        // Read info header (40 bytes)
        file.read(reinterpret_cast<char*>(&image.info_header.size), 4);
        file.read(reinterpret_cast<char*>(&image.info_header.width), 4);
        file.read(reinterpret_cast<char*>(&image.info_header.height), 4);
        file.read(reinterpret_cast<char*>(&image.info_header.planes), 2);
        file.read(reinterpret_cast<char*>(&image.info_header.bits_per_pixel), 2);
        file.read(reinterpret_cast<char*>(&image.info_header.compression), 4);
        file.read(reinterpret_cast<char*>(&image.info_header.image_size), 4);
        file.read(reinterpret_cast<char*>(&image.info_header.x_pixels_per_meter), 4);
        file.read(reinterpret_cast<char*>(&image.info_header.y_pixels_per_meter), 4);
        file.read(reinterpret_cast<char*>(&image.info_header.colors_used), 4);
        file.read(reinterpret_cast<char*>(&image.info_header.colors_important), 4);

        // This checks for the bmp signature
        if (image.file_header.signature != 0x4D42) { 
            std::cerr << "Error: Invalid BMP file signature" << std::endl;
            return image;
        }

        // Seek to pixel data
        file.seekg(image.file_header.offset, std::ios::beg);

        // Read pixel data
        int bytes_per_pixel = image.info_header.bits_per_pixel / 8;
        int row_size = ((image.info_header.width * image.info_header.bits_per_pixel + 31) / 32) * 4;
        
        // Handle negative height (top-down bitmap)
        int height = image.info_header.height;
        bool is_top_down = height < 0;
        if (is_top_down) {
            height = -height;
        }
        
        int total_pixels = image.info_header.width * height;
        
       
        if (image.info_header.width <= 0 || height <= 0 || bytes_per_pixel <= 0) {
            std::cerr << "Error: Invalid image dimensions" << std::endl;
            return image;
        }
        
        int pixel_data_size = total_pixels * bytes_per_pixel;
        
        if (pixel_data_size <= 0 || pixel_data_size > 1000000000) { // 1GB limit
            std::cerr << "Error: Image size too large: " << pixel_data_size << " bytes" << std::endl;
            return image;
        }

        image.pixel_data.resize(pixel_data_size);

        // BMP stores pixels from bottom to top (unless top-down flag is set)
        std::vector<uint8_t> row_buffer(row_size);
        if (is_top_down) {
            // Top-down: read rows from top to bottom
            for (int y = 0; y < height; ++y) {
                file.read(reinterpret_cast<char*>(row_buffer.data()), row_size);
                std::memcpy(
                    image.pixel_data.data() + (y * image.info_header.width * bytes_per_pixel),
                    row_buffer.data(),
                    image.info_header.width * bytes_per_pixel
                );
            }
        } else {
            // Bottom-up: read rows from bottom to top
            for (int y = height - 1; y >= 0; --y) {
                file.read(reinterpret_cast<char*>(row_buffer.data()), row_size);
                std::memcpy(
                    image.pixel_data.data() + (y * image.info_header.width * bytes_per_pixel),
                    row_buffer.data(),
                    image.info_header.width * bytes_per_pixel
                );
            }
        }
        
        // Normalize height to positive
        image.info_header.height = height;

        file.close();
        return image;
    }

    void write_bmp(const std::string& path, const BMPImage& image) {
        std::ofstream file(path, std::ios::binary);

        if (!file.is_open()) {
            std::cerr << "Error: Could not create file " << path << std::endl;
            return;
        }

        // Write file header
        file.write(reinterpret_cast<const char*>(&image.file_header.signature), 2);
        file.write(reinterpret_cast<const char*>(&image.file_header.file_size), 4);
        file.write(reinterpret_cast<const char*>(image.file_header.reserved1), 2);
        file.write(reinterpret_cast<const char*>(image.file_header.reserved2), 2);
        file.write(reinterpret_cast<const char*>(&image.file_header.offset), 4);

        // Write info header
        file.write(reinterpret_cast<const char*>(&image.info_header.size), 4);
        file.write(reinterpret_cast<const char*>(&image.info_header.width), 4);
        file.write(reinterpret_cast<const char*>(&image.info_header.height), 4);
        file.write(reinterpret_cast<const char*>(&image.info_header.planes), 2);
        file.write(reinterpret_cast<const char*>(&image.info_header.bits_per_pixel), 2);
        file.write(reinterpret_cast<const char*>(&image.info_header.compression), 4);
        file.write(reinterpret_cast<const char*>(&image.info_header.image_size), 4);
        file.write(reinterpret_cast<const char*>(&image.info_header.x_pixels_per_meter), 4);
        file.write(reinterpret_cast<const char*>(&image.info_header.y_pixels_per_meter), 4);
        file.write(reinterpret_cast<const char*>(&image.info_header.colors_used), 4);
        file.write(reinterpret_cast<const char*>(&image.info_header.colors_important), 4);

        // Write pixel data - write rows in reverse order for BMP bottom-up format
        int bytes_per_pixel = image.info_header.bits_per_pixel / 8;
        int pixels_per_row = image.info_header.width * bytes_per_pixel;
        
        for (int y = image.info_header.height - 1; y >= 0; --y) {
            int row_start = y * pixels_per_row;
            file.write(reinterpret_cast<const char*>(image.pixel_data.data() + row_start), pixels_per_row);
        }

        file.close();
        std::cout << "Image saved to " << path << std::endl;
    }

}


// read header bmp file 
