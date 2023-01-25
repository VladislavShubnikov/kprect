// ------------------------------------------------------------------
// Knack-pack project. Place given set of rectangles
// into given container with minimum empty space
//
// december 2022.
// (c) Vlad
// ------------------------------------------------------------------

#include "palette.h"
#define rgb(r,g,b)      ((r<<16)|(g<<8)|b)

static const int s_numPalColors = 128;
static const uint32_t s_palette[s_numPalColors] =
{
    // 0
    rgb(120,180,90), rgb(128,0,0), rgb(0,128,0), rgb(128,128,0),
    rgb(0,0,128), rgb(128,0,128), rgb(0,128,128), rgb(192,192,192),
    rgb(128,128,128), rgb(255,0,0), rgb(0,255,0), rgb(255,255,0),
    rgb(0,0,255), rgb(255,0,255), rgb(0,255,255), rgb(255,20,20),
    // 1
    rgb(0, 0, 0), rgb(0, 0, 95), rgb(0, 0, 135), rgb(0, 0, 175),
    rgb(0, 0, 215), rgb(0, 0, 255), rgb(0, 95, 0), rgb(0, 95, 95),
    rgb(0, 95, 135), rgb(0, 95, 175), rgb(0, 95, 215), rgb(0, 95, 255),
    rgb(0, 135, 0), rgb(0, 135, 95), rgb(0, 135, 135), rgb(0, 135, 175),
    // 2
    rgb(0, 135, 215), rgb(0, 135, 255), rgb(0, 175, 0), rgb(0, 175, 95),
    rgb(0, 175, 135), rgb(0, 175, 175), rgb(0, 175, 215), rgb(0, 175, 255),
    rgb(0, 215, 0), rgb(0, 215, 95), rgb(0, 215, 135), rgb(0, 215, 175),
    // 3
    rgb(0, 215, 215), rgb(0, 215, 255), rgb(0, 255, 0), rgb(0, 255, 95),
    rgb(0, 255, 135), rgb(0, 255, 175), rgb(0, 255, 215), rgb(0, 255, 255),
    rgb(95, 0, 0), rgb(95, 0, 95), rgb(95, 0, 135), rgb(95, 0, 175),
    rgb(95, 0, 215), rgb(95, 0, 255), rgb(95, 95, 0), rgb(95, 95, 95),
    // 4
    rgb(95, 95, 135), rgb(95, 95, 175), rgb(95, 95, 215), rgb(95, 95, 255),
    rgb(95, 135, 0), rgb(95, 135, 95), rgb(95, 135, 135), rgb(95, 135, 175),
    rgb(95, 135, 215), rgb(95, 135, 255), rgb(95, 175, 0), rgb(95, 175, 95),
    rgb(95, 175, 135), rgb(95, 175, 175), rgb(95, 175, 215), rgb(95, 175, 255),
    // 5
    rgb(95, 215, 0), rgb(95, 215, 95), rgb(95, 215, 95), rgb(95, 215, 135),
    rgb(95, 215, 175), rgb(95, 215, 215), rgb(95, 215, 255), rgb(95, 255, 0),
    rgb(95, 255, 95), rgb(95, 255, 135), rgb(95, 255, 175), rgb(95, 255, 215),
    rgb(95, 255, 255), rgb(135, 0, 0), rgb(135, 0, 95), rgb(135, 0, 135),
    // 6
    rgb(135, 0, 175), rgb(135, 0, 215), rgb(135, 0, 255), rgb(135, 95, 0),
    rgb(135, 95, 95), rgb(135, 95, 135), rgb(135, 95, 175), rgb(135, 95, 215),
    rgb(135, 95, 255), rgb(135, 135, 0), rgb(135, 135, 95), rgb(135, 135, 135),
    rgb(135, 135, 175), rgb(135, 135, 215), rgb(135, 135, 255), rgb(135, 175, 0),
    // 7
    rgb(135, 175, 95), rgb(135, 175, 135), rgb(135, 175, 175), rgb(135, 175, 215),
    rgb(135, 175, 255), rgb(135, 215, 0), rgb(135, 215, 95), rgb(135, 215, 135),
    rgb(135, 215, 175), rgb(135, 215, 215), rgb(135, 215, 255), rgb(135, 255, 0),
    rgb(135, 255, 95), rgb(135, 255, 135), rgb(135, 255, 175), rgb(135, 255, 215)
};


void kp::Palette::getBackground(int index, uint32_t& r, uint32_t& g, uint32_t& b)
{
    const int i = index & (s_numPalColors - 1);
    uint32_t rgb = s_palette[i];
    b = rgb & 0xff;
    g = (rgb >> 8) & 0xff;
    r = (rgb >> 16) & 0xff;
}

void kp::Palette::getForeground(int index, uint32_t& r, uint32_t& g, uint32_t& b)
{
    const int i = index & (s_numPalColors - 1);
    uint32_t rgb = s_palette[i];
    b = rgb & 0xff;
    g = (rgb >> 8) & 0xff;
    r = (rgb >> 16) & 0xff;
    b = (b + 120) & 0xff;
    g = (r + 180) & 0xff;
    r = (g + 97) & 0xff;
}

