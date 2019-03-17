#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

struct __attribute__((packed)) VertexLayout {
  float x;
  float y;
  float z;
  float nx;
  float ny;
  float nz;
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

void writeHeader(char* path, int nvert) {
  std::ofstream out(path);
  out << "ply" << std::endl;
  out << "format binary_little_endian 1.0" << std::endl;
  out << "element vertex " + std::to_string(nvert) << std::endl;
  out << "property float x" << std::endl;
  out << "property float y" << std::endl;
  out << "property float z" << std::endl;
  out << "property float nx" << std::endl;
  out << "property float ny" << std::endl;
  out << "property float nz" << std::endl;
  out << "property uchar red" << std::endl;
  out << "property uchar green" << std::endl;
  out << "property uchar blue" << std::endl;
  out << "end_header" << std::endl;
  out.close();
}

void writeData(char* path, int nvert) {
  VertexLayout vert;
  uint64_t count = 0;
  std::ofstream ofs(path, std::ios::binary | std::ios::app);
  for (int i = 0; i < nvert; ++i) {
    vert.x = float(count++ % 128);
    vert.y = float(count++ % 128);
    vert.z = float(count++ % 128);
    vert.nx = float(count++ % 128);
    vert.ny = float(count++ % 128);
    vert.nz = float(count++ % 128);
    vert.red = static_cast<uint8_t>(count++ % 128);
    vert.green = (count++ % 128);
    vert.blue = (count++ % 128);

    ofs.write((char*)&vert, sizeof(VertexLayout));

    if (i % 10000000 == 0)
      std::cout << i << "/" << nvert << std::endl;
  }
}

int main(int argc, char** argv) {
  if (argc != 3)
    return 0;

  int nvert = int(std::atof(argv[2]) * 1073741824 / sizeof(VertexLayout));

  std::cout << "Generating " << nvert << " vertices" << std::endl;

  writeHeader(argv[1], nvert);
  writeData(argv[1], nvert);

  return 0;
}
