# Image Filter Application

A C++ application for applying various filters to BMP images.

## Features

- **Blur Filter**: Applies a 3x3 box blur to smooth the image
- **Pixelate Filter**: Applies pixelation effect with 10x10 pixel blocks

## Project Structure

```
Image-Filter/
├── main.cpp                # Application entry point
├── include/
│   ├── filters.hpp         # Filter class definitions
│   ├── read.hpp            # File I/O declarations
│   └── information.hpp      # BMP header structures
├── src/
│   ├── filters.cpp         # Filter implementations
│   └── read.cpp            # File I/O implementations
└── sample/                 # Sample BMP images
```

## Building

```bash
    g++ main.cpp src/filters.cpp src/read.cpp -o main

```

## Usage

```bash
./image_filter <input_image.bmp> <filter_name> [output_image.bmp]
```

### Examples

```bash
# Apply blur filter (output to output.bmp)
./image_filter input.bmp blur

# Apply blur filter with custom output
./image_filter input.bmp blur blurred.bmp

# Apply pixelate filter
./image_filter input.bmp pixelate pixelated.bmp
```

## Supported Filters

- `blur` - Applies a smoothing blur effect
- `pixelate` - Applies pixelation effect
- `greyscale` - Applies pixelation effect

## Notes

- Only 24-bit BMP images are supported
- Filters preserve image dimensions and format
- Output files are saved in BMP format
- Input validation is performed on file signatures
