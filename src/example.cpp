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

    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    fastply::FastPly<VertexLayout, FaceLayout> fp;
    auto path = std::string(argv[1]);
    std::cout << ":: Input: " << path << std::endl;
    auto success = fp.init(path);
    if (success){
        auto vh = fp.getVertexHeader();
        std::cout << vh.size << std::endl;

        std::vector<float> test;

        for (int j = 0; j < 10; ++j)
        {
            auto ptr = vh.start;
            while(ptr++ != vh.end)
            {
                test.push_back(ptr->x*j);
                test.push_back(ptr->nx*j);
                test.push_back(ptr->y*j);
                test.push_back(ptr->ny*j);
                test.push_back(ptr->z*j);
                test.push_back(ptr->nz*j);
            }
            std::cout << test.size() << std::endl;
        }
        std::cout << test.size() << std::endl;

    }
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();

    std::cout << duration << std::endl;
    return 0;
}