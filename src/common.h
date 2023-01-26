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

    //! Maximum possible figures (rectangles) in set
    const int maxFigures = 100; // NOLINT
    //! Maximum possible container width
    const int maxContainerWidth = 2400; // NOLINT

    /**
    * @brief Calculate factorial of given input
      @return factorial of x
     */
    uint64_t getFactorial(uint64_t x); // NOLINT

}
#endif

