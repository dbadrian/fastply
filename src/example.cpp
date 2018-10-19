#include <iostream>
#include <algorithm>
#include <array>
#include <vector>
#include <chrono>



#include "fastply.h"

struct __attribute__ ((packed)) VertexLayout {
    float x;
    float y;
    float z;
    float nx;
    float ny;
    float nz;
    std::uint8_t red;
    std::uint8_t green;
    std::uint8_t blue;
};

struct __attribute__ ((packed)) FaceLayout {
    float x;
    float y;
    float z;
    float nx;
    float ny;
    float nz;
    std::uint8_t red;
    std::uint8_t green;
    std::uint8_t blue;
};

int main(int argc, char** argv)
{
    std::cout << "fastply example" << std::endl;

    std::vector<int> test{-1,3,4};
    std::cout << test.at(11) << std::endl;

    return 0;
}