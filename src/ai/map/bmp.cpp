#include "bmp.h"

  // int img_width = width * 2; // number of bytes per row * 2 (1 byte = 2 pixels)
  // int img_height = -1 * height;
  // BMP bmp(width * 2, height * -1, pixel_data, size);

BMP::BMP(int32_t data_width, int data_height, uint8_t** data, std::string write_path) : 
  data_width(data_width), data_height(data_height), data(data), write_path(write_path) {
  // std::cout << "width: " << width << ", height: " << height << ", size: " << _size << "\n";
  size = data_width * data_height;

  bmp_info_header.width = data_width * 2;  // number of bytes per row * 2 (1 byte = 2 pixels)
  bmp_info_header.height = data_height * -1; // want image to print top to bottom
  bmp_info_header.size = sizeof(BMPInfoHeader);
  bmp_info_header.bit_count = 4;
  bmp_info_header.compression = 0;

  memset(color_table, 0, sizeof(uint32_t)*16);
  color_table[0] = 0x00FFFFFF;
  color_table[1] = 0x00AAAAAA;
  color_table[2] = 0x00555555;

  file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(uint32_t)*16;
  file_header.file_size = file_header.offset_data + static_cast<uint32_t>(size);
}

BMP::~BMP() {
  for (int i = 0; i < data_height; i++) {
    delete [] data[i];
  }
  delete [] data;
}

void BMP::write() {
    std::ofstream of{ write_path, std::ios_base::binary };
    if (of) {
      write_headers_and_data(of);
      of.close();
    } else {
        throw std::runtime_error("Unable to open the output image file.");
    }
}

void BMP::write_multiple(int num_times, int rows, int cols) {
  bmp_info_header.width *= cols;
  bmp_info_header.height *= rows;
  size *= num_times;
  file_header.file_size = file_header.offset_data + static_cast<uint32_t>(size);

  std::ofstream of;
  of.open(write_path, std::ios_base::binary);
  if (of) {
    write_headers(of);
    for (int i = 0; i < num_times; i++) {
      of.write((const char*)data, size/num_times);
    }
    of.close();
  } else {
      throw std::runtime_error("Unable to open the output image file.");
  }
}

void BMP::write_headers(std::ofstream &of) {
  of.write((const char*)&file_header, sizeof(file_header));
  of.write((const char*)&bmp_info_header, sizeof(bmp_info_header));
  of.write((const char*)color_table, sizeof(uint32_t)*16);
}

void BMP::write_data(std::ofstream &of) {
  // printf("writing data %d times\n\n", bmp_info_header.height * -1);
  for (int i = 0; i < -1 * bmp_info_header.height; i++) {
    // printf("i: %d, writing %d bytes\n", i, sizeof(uint8_t) * bmp_info_header.width / 2);
    of.write((const char*)data[i], sizeof(uint8_t) * bmp_info_header.width / 2);
  }
}

void BMP::write_headers_and_data(std::ofstream &of) {
  write_headers(of);
  write_data(of);
}