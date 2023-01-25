// ------------------------------------------------------------------
// Knack-pack project. Place given set of rectangles
// into given container with minimum empty space
//
// december 2022.
// (c) Vlad
// ------------------------------------------------------------------

#ifndef KNACK_PACK_PALETTE_H_
#define KNACK_PACK_PALETTE_H_

#include <cstdint>
namespace kp
{
class Palette
{
public:
    static void getBackground(int index, uint32_t& r, uint32_t& g, uint32_t& b);
    static void getForeground(int index, uint32_t& r, uint32_t& g, uint32_t& b);
};

}

#endif
