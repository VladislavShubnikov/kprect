// ------------------------------------------------------------------
// Knack-pack project. Place given set of rectangles
// into given container with minimum empty space
//
// december 2022.
// (c) Vlad
// ------------------------------------------------------------------

#include "common.h"


uint64_t kp::getFactorial(uint64_t v) // NOLINT
{
    uint64_t res = 1;
    for (uint64_t i = 2; i < v; i++)
    {
        res = res * i;
    }
    return res;
}

