#ifndef RPI3_TGA_FILE_H
#define RPI3_TGA_FILE_H

#include <string>

#include "logger.h"

class TGAFile
{
public:

  uint8_t* data() const { return data_; }
  auto width() const { return width_; }
  auto height() const { return height_; }

  TGAFile(const std::string& filename) {
    load(filename);
  }

  bool load(const std::string& filename) {
    FILE *file_handler;
    uint8_t byte_ignore;
    int16_t short_ignore;
    uint64_t size;
    int32_t color_mode;
  
    // Open the TGA file.
    file_handler = fopen(filename.c_str(), "rb");
    if (file_handler == nullptr)
    {
      Logger::error("File %s can't be opened", filename.c_str());
      return false;
    }
  
    // Read the two first bytes we don't need.
    fread(&byte_ignore, sizeof(byte_ignore), 1, file_handler);
    fread(&byte_ignore, sizeof(byte_ignore), 1, file_handler);
  
    // Which type of image gets stored in imageTypeCode.
    fread(&type_code_, sizeof(type_code_), 1, file_handler);
  
    // For our purposes, the type code should be 2 (uncompressed RGB image)
    // or 3 (uncompressed black-and-white images).
    if (type_code_ != 2 && type_code_ != 3)
    {
      fclose(file_handler);
      Logger::error("Incorrect type code: %d", type_code_);
      return false;
    }
  
    // Read 13 bytes of data we don't need.
    fread(&short_ignore, sizeof(short_ignore), 1, file_handler);
    fread(&short_ignore, sizeof(short_ignore), 1, file_handler);
    fread(&byte_ignore, sizeof(byte_ignore), 1, file_handler);
    fread(&short_ignore, sizeof(short_ignore), 1, file_handler);
    fread(&short_ignore, sizeof(short_ignore), 1, file_handler);
  
    // Read the image's width and height.
    fread(&width_, sizeof(width_), 1, file_handler);
    fread(&height_, sizeof(height_), 1, file_handler);
  
    // Read the bit depth.
    fread(&bit_count_, sizeof(bit_count_), 1, file_handler);
  
    // Read one byte of data we don't need.
    fread(&byte_ignore, sizeof(byte_ignore), 1, file_handler);
  
    // Color mode -> 3 = BGR, 4 = BGRA.
    color_mode = bit_count_ / 8;
    size = width_ * height_ * color_mode;
  
    // Allocate memory for the image data.
    data_ = (uint8_t*)malloc(sizeof(uint8_t) * size);
  
    // Read the image data.
    fread(data_, sizeof(uint8_t), size, file_handler);
  
    // Change from BGR to RGB so OpenGL can read the image data.
    for (uint64_t index = 0; index < size; index += color_mode)
    {
      std::swap(data_[index], data_[index + 2]);
    }
  
    fclose(file_handler);
  
    Logger::info("TGA loader: file %s successfully loaded: %dx%d", filename.c_str(), width_, height_);
  
    return true;
  }
private:
  uint8_t type_code_ {0};
  uint16_t width_ {0};
  uint16_t height_ {0};
  uint8_t bit_count_ {0};
  uint8_t *data_ {nullptr};
};

#endif