/*****************************************************
 * Automatically generated. Please verify that types *
 * used have the correct size on your platform.      *
 *****************************************************/
#include <cstdint>

struct __attribute__ ((packed)) VertexLayout {
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

struct __attribute__ ((packed)) CameraLayout {
    float view_px;
    float view_py;
    float view_pz;
    float x_axisx;
    float x_axisy;
    float x_axisz;
    float y_axisx;
    float y_axisy;
    float y_axisz;
    float z_axisx;
    float z_axisy;
    float z_axisz;
    float focal;
    float scalex;
    float scaley;
    float centerx;
    float centery;
    int32_t viewportx;
    int32_t viewporty;
    float k1;
    float k2;
};

struct __attribute__ ((packed)) AlltypesLayout {
    int8_t c;
    uint8_t uc;
    int16_t s;
    uint16_t us;
    int32_t i;
    uint32_t ui;
    float f;
    double d;
};

struct __attribute__ ((packed)) FaceLayout {
    uint8_t list_length;
    int32_t vertex_index[4];
};

