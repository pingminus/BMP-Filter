#include <iostream>
#include <memory>
#include "include/read.hpp"
#include "include/filters.hpp"

/*
    Entry point of the image processing application. This program takes an image file and a filter name as command-line arguments, applies the specified filter to the image, and outputs the result.
    Usage: ./image_processor <image_file> <filter_name> [output_file]
    Example: ./image_processor input.bmp blur output.bmp
    Supported filters: blur, pixelate
    

    compile with:
    g++ main.cpp src/filters.cpp src/read.cpp -o main

    @author Niklas Kost
*/

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <image_file> <filte  r_name> [output_file]" << std::endl;
        std::cerr << "Supported filters: blur, pixelate" << std::endl;
        return 1;
    }

    std::string image_file = argv[1];
    std::string filter_name = argv[2];
    std::string output_file = (argc >= 4) ? argv[3] : "output.bmp";

    std::cout << "Loading image: " << image_file << std::endl;
    
    // Read the BMP image
    BMPImage image = read::read_bmp(image_file);
    
    if (image.file_header.signature == 0) {
        std::cerr << "Failed to load image" << std::endl;
        return 1;
    }

    std::cout << "Image dimensions: " << image.info_header.width << "x" << image.info_header.height << std::endl;
    std::cout << "Bits per pixel: " << image.info_header.bits_per_pixel << std::endl;

    Filters::Filter* filter = nullptr;

    if (filter_name == "blur") {
        std::cout << "Applying blur filter..." << std::endl;
        filter = new Filters::BlurFilter();
    } else if (filter_name == "pixelate") {
        std::cout << "Applying pixelate filter..." << std::endl;
        filter = new Filters::PixelateFilter();
    } else if (filter_name == "black") {
        std::cout << "Applying black filter..." << std::endl;
        filter = new Filters::BlackFilter();
    } else if (filter_name == "greyscale") {
        std::cout << "Applying greyscale filter..." << std::endl;
        filter = new Filters::GreyscaleFilter(); 
    } else {
        std::cerr << "Unknown filter: " << filter_name << std::endl;
        std::cerr << "Supported filters: blur, pixelate, black, greyscale" << std::endl;
        return 1;
    }

    filter->apply(image);

    std::cout << "Saving filtered image: " << output_file << std::endl;
    read::write_bmp(output_file, image);

    std::cout << "Done!" << std::endl;
    delete filter;
    return 0;
}