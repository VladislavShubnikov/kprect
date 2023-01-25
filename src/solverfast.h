// ------------------------------------------------------------------
// Knack-pack project. Place given set of rectangles
// into given container with minimum empty space
//
// december 2022.
// (c) Vlad
// ------------------------------------------------------------------

#ifndef KNACK_PACK_SOLVER_FAST_H
#define KNACK_PACK_SOLVER_FAST_H


#include "solverslow.h"

namespace kp
{

    class SolverFast : public SolverSlow
    {
    public:
        SolverFast(Rect &cont, std::vector<Rect> &list);
        int fillContainer(const char *testName) override;
    };

} // namespace kp

#endif
