// ------------------------------------------------------------------
// Knack-pack project. Place given set of rectangles
// into given container with minimum empty space
//
// december 2022.
// (c) Vlad
// ------------------------------------------------------------------

#ifndef KNACK_PACK_COMMON_H_
#define KNACK_PACK_COMMON_H_

#include <cstdint>

namespace kp
{

    const int maxFigures = 100; // NOLINT
    const int maxContainerWidth = 2400; // NOLINT

    uint64_t getFactorial(uint64_t v); // NOLINT

}
#endif

