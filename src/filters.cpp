#include "../include/filters.hpp"


namespace Filters {

    // Simple box blur filter (3x3)
    void BlurFilter::apply(BMPImage& image) {
        if (image.info_header.bits_per_pixel != 24) {
            return; // Only handle 24-bit images
        }

        int width = image.info_header.width;
        int height = image.info_header.height;
        int channels = 3; // BGR for BMP

        // Create a copy for reading
        std::vector<uint8_t> original = image.pixel_data;

        // Apply blur
        for (int y = 1; y < height - 1; ++y) {
            for (int x = 1; x < width - 1; ++x) {
                for (int c = 0; c < channels; ++c) {
                    uint32_t sum = 0;
                    for (int dy = -1; dy <= 1; ++dy) {
                        for (int dx = -1; dx <= 1; ++dx) {
                            int nx = x + dx;
                            int ny = y + dy;
                            size_t idx = (ny * width + nx) * channels + c;
                            sum += original[idx];
                        }
                    }
                    size_t current_idx = (y * width + x) * channels + c;
                    image.pixel_data[current_idx] = sum / 9;
                }
            }
        }
    }

    // Pixelate filter
    void PixelateFilter::apply(BMPImage& image) {
        if (image.info_header.bits_per_pixel != 24) {
            return; // Only handle 24-bit images
        }

        int width = image.info_header.width;
        int height = image.info_header.height;
        int block_size = 10; // Size of pixelation blocks
        int channels = 3;    // BGR

        for (int y = 0; y < height; y += block_size) {
            for (int x = 0; x < width; x += block_size) {
                // Calculate average color in block
                uint32_t avg_color[3] = {0, 0, 0};
                int count = 0;

                int max_y = std::min(y + block_size, height);
                int max_x = std::min(x + block_size, width);

                for (int by = y; by < max_y; ++by) {
                    for (int bx = x; bx < max_x; ++bx) {
                        size_t idx = (by * width + bx) * channels;
                        for (int c = 0; c < channels; ++c) {
                            avg_color[c] += image.pixel_data[idx + c];
                        }
                        count++;
                    }
                }

                // Apply average color to entire block
                for (int c = 0; c < channels; ++c) {
                    avg_color[c] /= count;
                }

                for (int by = y; by < max_y; ++by) {
                    for (int bx = x; bx < max_x; ++bx) {
                        size_t idx = (by * width + bx) * channels;
                        for (int c = 0; c < channels; ++c) {
                            image.pixel_data[idx + c] = avg_color[c];
                        }
                    }
                }
            }
        }
    }

    // Black filter - sets every pixel to 0
    void BlackFilter::apply(BMPImage& image) {
        if (image.info_header.bits_per_pixel != 24) {
            return; // Only handle 24-bit images
        }

        // Set all pixel values to 0 (black)
        std::fill(image.pixel_data.begin(), image.pixel_data.end(), 0);
    }

    // Greyscale filter - converts image to greyscale
    void GreyscaleFilter::apply(BMPImage& image) {
        if (image.info_header.bits_per_pixel != 24) {
            return; // Only handle 24-bit images
        }

        int width = image.info_header.width;
        int height = image.info_header.height;
        int channels = 3; 

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                size_t idx = (y * width + x) * channels;
                uint8_t b = image.pixel_data[idx];    
                uint8_t g = image.pixel_data[idx + 1]; 
                uint8_t r = image.pixel_data[idx + 2];

                uint8_t grey = static_cast<uint8_t>(0.114f * b + 0.587f * g + 0.299f * r);

                image.pixel_data[idx] = grey;
                image.pixel_data[idx + 1] = grey;
                image.pixel_data[idx + 2] = grey;
            }
        }
    }

}