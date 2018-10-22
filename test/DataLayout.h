/*****************************************************
 * Automatically generated. Please verify that types *
 * used have the correct size on your platform.      *
 *****************************************************/
#include <cstdint>
#include <cstring>

struct __attribute__ ((packed)) Vertex {
    const float x;
    const float y;
    const float z;
    const float nx;
    const float ny;
    const float nz;
    const uint8_t red;
    const uint8_t green;
    const uint8_t blue;

    bool operator==(const Vertex& rhs) const {
        return x == rhs.x &&
               y == rhs.y &&
               z == rhs.z &&
               nx == rhs.nx &&
               ny == rhs.ny &&
               nz == rhs.nz &&
               red == rhs.red &&
               green == rhs.green &&
               blue == rhs.blue;
    }
};

struct __attribute__ ((packed)) Camera {
    const float view_px;
    const float view_py;
    const float view_pz;
    const float x_axisx;
    const float x_axisy;
    const float x_axisz;
    const float y_axisx;
    const float y_axisy;
    const float y_axisz;
    const float z_axisx;
    const float z_axisy;
    const float z_axisz;
    const float focal;
    const float scalex;
    const float scaley;
    const float centerx;
    const float centery;
    const int32_t viewportx;
    const int32_t viewporty;
    const float k1;
    const float k2;

    bool operator==(const Camera& rhs) const {
        return view_px == rhs.view_px &&
               view_py == rhs.view_py &&
               view_pz == rhs.view_pz &&
               x_axisx == rhs.x_axisx &&
               x_axisy == rhs.x_axisy &&
               x_axisz == rhs.x_axisz &&
               y_axisx == rhs.y_axisx &&
               y_axisy == rhs.y_axisy &&
               y_axisz == rhs.y_axisz &&
               z_axisx == rhs.z_axisx &&
               z_axisy == rhs.z_axisy &&
               z_axisz == rhs.z_axisz &&
               focal == rhs.focal &&
               scalex == rhs.scalex &&
               scaley == rhs.scaley &&
               centerx == rhs.centerx &&
               centery == rhs.centery &&
               viewportx == rhs.viewportx &&
               viewporty == rhs.viewporty &&
               k1 == rhs.k1 &&
               k2 == rhs.k2;
    }
};

struct __attribute__ ((packed)) Alltypes {
    const int8_t c;
    const uint8_t uc;
    const int16_t s;
    const uint16_t us;
    const int32_t i;
    const uint32_t ui;
    const float f;
    const double d;

    bool operator==(const Alltypes& rhs) const {
        return c == rhs.c &&
               uc == rhs.uc &&
               s == rhs.s &&
               us == rhs.us &&
               i == rhs.i &&
               ui == rhs.ui &&
               f == rhs.f &&
               d == rhs.d;
    }
};

struct __attribute__ ((packed)) Face {
    const uint8_t vertex_index_length = 4;
    const int32_t vertex_index[4];

    bool operator==(const Face& rhs) const {
        return !std::memcmp(&vertex_index, &rhs.vertex_index, sizeof(int32_t)*4);
    }
};

