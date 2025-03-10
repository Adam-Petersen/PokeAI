#ifndef BMP_H
#define BMP_H

#include <string>
#include <fstream>
#include <cstring>
#include "../util/common.h"

#pragma pack(push, 1)
struct BMPFileHeader {
  uint16_t file_type{ 0x4D42 };          // File type always BM which is 0x4D42 (stored as hex uint16_t in little endian)
  uint32_t file_size{ 0 };               // Size of the file (in bytes)
  uint16_t reserved1{ 0 };               // Reserved, always 0
  uint16_t reserved2{ 0 };               // Reserved, always 0
  uint32_t offset_data{ 0 };             // Start position of pixel data (bytes from the beginning of the file)
};

struct BMPInfoHeader {
  uint32_t size{ 0 };                      // Size of this header (in bytes)
  int32_t width{ 0 };                      // width of bitmap in pixels
  int32_t height{ 0 };                     // height of bitmap in pixels
                                            //       (if positive, bottom-up, with origin in lower left corner)
                                            //       (if negative, top-down, with origin in upper left corner)
  uint16_t planes{ 1 };                    // No. of planes for the target device, this is always 1
  uint16_t bit_count{ 0 };                 // No. of bits per pixel
  uint32_t compression{ 0 };               // 0 or 3 - uncompressed. THIS PROGRAM CONSIDERS ONLY UNCOMPRESSED BMP images
  uint32_t size_image{ 0 };                // 0 - for uncompressed images
  int32_t x_pixels_per_meter{ 0 };
  int32_t y_pixels_per_meter{ 0 };
  uint32_t colors_used{ 0 };               // No. color indexes in the color table. Use 0 for the max number of colors allowed by bit_count
  uint32_t colors_important{ 0 };          // No. of colors used for displaying the bitmap. If 0 all colors are required
};
#pragma pack(pop)

class BMP {
 public:
  BMP(int32_t width, int32_t height, uint8_t** _data, std::string write_path);
  ~BMP();

  uint8_t** getData() { return data; }
  void write();
  void write_multiple(int num_times, int rows, int cols);

 private:
  void write_headers(std::ofstream &of);
  void write_data(std::ofstream &of);
  void write_headers_and_data(std::ofstream &of);

  BMPFileHeader file_header;
  BMPInfoHeader bmp_info_header;
  uint32_t color_table[16];
  uint8_t** data;
  int data_height;
  int data_width;
  int size;
  std::string write_path;
};

#endif // BMP_H