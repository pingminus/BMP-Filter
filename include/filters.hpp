#pragma once

#include <string>
#include "information.hpp"
#include <algorithm>

namespace Filters {
    // Base filter class
    class Filter {
    public:
        virtual ~Filter() = default;
        virtual void apply(BMPImage& image) = 0;
    };

    // Blur filter
    class BlurFilter final : public Filter {
    public:
        void apply(BMPImage& image) override;
    };

    // Pixelate filter
    class PixelateFilter final : public Filter {
    public:
        void apply(BMPImage& image) override;
    };

    // Black filter - sets every pixel to 0
    class BlackFilter final : public Filter {
    public:
        void apply(BMPImage& image) override;
    };

    // Greyscale filter - converts image to greyscale
    class GreyscaleFilter final : public Filter {
    public:
        void apply(BMPImage& image) override;
    };
}
