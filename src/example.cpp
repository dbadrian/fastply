#include <iostream>
#include <algorithm>
#include <array>
#include <vector>


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

    fastply::FastPly<VertexLayout, FaceLayout> fp;
    auto path = std::string(argv[1]);
    std::cout << ":: Input: " << path << std::endl;
    auto success = fp.init(path);
    if (success){
        auto vh = fp.getVertexHeader();
        std::cout << vh.size << std::endl;

        std::vector<int> test(vh.size);
        std::for_each(vh.start, vh.end, [test](VertexLayout i) mutable { test.push_back(i.x); } );

        std::cout << test.size() << std::endl;
    }
    return 0;
}