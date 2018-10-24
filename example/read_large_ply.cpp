/*************************************************************
 * Pretty random example; tests might be better reference    *
 * until this is improved with better examples.              *
 ************************************************************/
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <random>
#include <string>

#include <fastply.h>

struct __attribute__((packed)) Vertex {
  float x;
  float y;
  float z;
  float nx;
  float ny;
  float nz;
  uint8_t red;
  uint8_t green;
  uint8_t blue;

  bool operator==(const Vertex& rhs) const {
    return !std::memcmp(&x, &rhs.x, sizeof(float)) && y == rhs.y &&
           z == rhs.z && nx == rhs.nx && ny == rhs.ny && nz == rhs.nz &&
           red == rhs.red && green == rhs.green && blue == rhs.blue;
  }
};

int main(int argc, char** argv) {
  if (argc != 2)
    return 0;

  fastply::FastPly<Vertex> fp;
  fp.open(std::string(argv[1]));

  auto& vertices = fp.get<Vertex>();
  std::cout << ":: Opening PLY with " << vertices.size() << " vertices!"
            << std::endl;

  std::cout << ":: Random Read Test" << std::endl;
  std::cout << "    :: Generating " << 50000000 << " random indices to query"
            << std::endl;
  std::vector<unsigned int> rnd_indices(50000000);
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<unsigned int> dist{
      0, static_cast<unsigned int>(vertices.size())};
  auto generator = [&dist, &gen]() { return dist(gen); };
  std::generate(std::begin(rnd_indices), std::end(rnd_indices), generator);

  std::cout << "    :: Reading ..." << std::endl;
  auto start = std::chrono::high_resolution_clock::now();
  for (std::size_t ridx = 0; ridx < rnd_indices.size(); ridx++) {
    if (vertices.at(ridx).x != (ridx * 9) % 128) {
      std::cout << "ERR " << (ridx * 9) % 128 << " != " << vertices.at(ridx).x
                << std::endl;
    } else if (vertices.at(ridx).y != (ridx * 9 + 1) % 128) {
      std::cout << "ERR " << (ridx * 9) % 128 << " != " << vertices.at(ridx).y
                << std::endl;
    } else if (vertices.at(ridx).z != (ridx * 9 + 2) % 128) {
      std::cout << "ERR " << (ridx * 9) % 128 << " != " << vertices.at(ridx).z
                << std::endl;
    } else if (vertices.at(ridx).nx != (ridx * 9 + 3) % 128) {
      std::cout << "ERR " << (ridx * 9) % 128 << " != " << vertices.at(ridx).nx
                << std::endl;
    } else if (vertices.at(ridx).ny != (ridx * 9 + 4) % 128) {
      std::cout << "ERR " << (ridx * 9) % 128 << " != " << vertices.at(ridx).ny
                << std::endl;
    } else if (vertices.at(ridx).nz != (ridx * 9 + 5) % 128) {
      std::cout << "ERR " << (ridx * 9) % 128 << " != " << vertices.at(ridx).nz
                << std::endl;
    } else if (vertices.at(ridx).red != (ridx * 9 + 6) % 128) {
      std::cout << "ERR " << (ridx * 9) % 128 << " != " << vertices.at(ridx).red
                << std::endl;
    } else if (vertices.at(ridx).green != (ridx * 9 + 7) % 128) {
      std::cout << "ERR " << (ridx * 9) % 128
                << " != " << vertices.at(ridx).green << std::endl;
    } else if (vertices.at(ridx).blue != (ridx * 9 + 8) % 128) {
      std::cout << "ERR " << (ridx * 9) % 128
                << " != " << vertices.at(ridx).blue << std::endl;
    }
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto elapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "    :: Done in " << elapsed.count() << "ms" << std::endl;

  std::cout << ":: Count all matching vertices (std::count)" << std::endl;
  Vertex target1{0, 1, 2, 3, 4, 5, 6, 7, 8};
  start = std::chrono::high_resolution_clock::now();
  int num_items1 = std::count(vertices.begin(), vertices.end(), target1);
  end = std::chrono::high_resolution_clock::now();
  elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "    :: Done in " << elapsed.count() << "ms" << std::endl;

  return 0;
}
