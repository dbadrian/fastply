#include <fstream>
#include <iostream>
#include <cstdint>
#include <string>
#include <cstdlib>
#include <random>

#include <fastply.h>

struct __attribute__ ((packed)) Vertex {
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


int main(int argc, char** argv)
{
    if(argc != 2) return 0;

    fastply::FastPly<Vertex> fp;
    fp.init(std::string(argv[1]));

    auto& vertices = fp.get<Vertex>();

    std::cout << "Found " << vertices.size << " vertices" << std::endl;

    std::cout << "Generating " << 500000 << " random indices ..." << std::endl;
    std::vector<unsigned int> rnd_indices(500000);
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<unsigned int> dist{0, static_cast<unsigned int>(vertices.size)};
    auto generator = [&dist, &gen](){
                   return dist(gen);
               };
    std::generate(std::begin(rnd_indices), std::end(rnd_indices), generator);

    std::cout << "Validating ply with random access" << std::endl;
    for (std::size_t ridx = 0; ridx < rnd_indices.size(); ridx++)
    {
        if (vertices[ridx].x != (ridx*9) % 128) {
            std::cout << "ERR" << (ridx*9) % 128 << " != " << vertices[ridx].x << std::endl;
        }
        else if (vertices[ridx].y != (ridx*9 + 1) % 128) {
            std::cout << "ERR" << (ridx*9) % 128 << " != " << vertices[ridx].y << std::endl;
        }
        else if (vertices[ridx].z != (ridx*9 + 2) % 128) {
            std::cout << "ERR" << (ridx*9) % 128 << " != " << vertices[ridx].z << std::endl;
        }
        else if (vertices[ridx].nx != (ridx*9 + 3) % 128) {
            std::cout << "ERR" << (ridx*9) % 128 << " != " << vertices[ridx].nx << std::endl;
        }
        else if (vertices[ridx].ny != (ridx*9 + 4) % 128) {
            std::cout << "ERR" << (ridx*9) % 128 << " != " << vertices[ridx].ny << std::endl;
        }
        else if (vertices[ridx].nz != (ridx*9 + 5) % 128) {
            std::cout << "ERR" << (ridx*9) % 128 << " != " << vertices[ridx].nz << std::endl;
        }
        else if (vertices[ridx].red != (ridx*9 + 6) % 128) {
            std::cout << "ERR" << (ridx*9) % 128 << " != " << vertices[ridx].red << std::endl;
        }
        else if (vertices[ridx].green != (ridx*9 + 7) % 128) {
            std::cout << "ERR" << (ridx*9) % 128 << " != " << vertices[ridx].green << std::endl;
        }
        else if (vertices[ridx].blue != (ridx*9 + 8) % 128) {
            std::cout << "ERR" << (ridx*9) % 128 << " != " << vertices[ridx].blue << std::endl;
        }
    }
    std::cout << "done" << std::endl;

    return 0;

}
