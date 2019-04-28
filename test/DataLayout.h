/*****************************************************
 * Automatically generated. Please verify that types *
 * used have the correct size on your platform.      *
 *****************************************************/
#include "fastply/fastply_macros.h"

#include <cstdint>
#include <cstring>

FASTPLY_ELEMENT(Vertex,
  const float x;
  const float y;
  const float z;
  const float nx;
  const float ny;
  const float nz;
  const uint8_t red;
  const uint8_t green;
  const uint8_t blue;

  FASTPLY_GENERATE_OPERATORS(Vertex, x, y, z, nx, ny, nz, red, green, blue)
)

FASTPLY_ELEMENT(Camera,
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

  FASTPLY_GENERATE_OPERATORS(Camera, view_px, view_py, view_pz, x_axisx, x_axisy, x_axisz, y_axisx, y_axisy, y_axisz, z_axisx, z_axisy, z_axisz, focal, scalex, scaley, centerx, centery)
)

FASTPLY_ELEMENT(Alltypes,
  const int8_t c;
  const uint8_t uc;
  const int16_t s;
  const uint16_t us;
  const int32_t i;
  const uint32_t ui;
  const float f;
  const double d;

  FASTPLY_GENERATE_OPERATORS(Alltypes, c, uc, s, us, i, ui, f, d)
)

FASTPLY_ELEMENT(Face,
    const uint8_t vertex_index_length = 4;
    const int32_t vertex_index[4];

  FASTPLY_GENERATE_OPERATORS(Face, vertex_index[0], vertex_index[1], vertex_index[2], vertex_index[3])
)

