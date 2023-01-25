// ------------------------------------------------------------------
// Knack-pack project. Place given set of rectangles
// into given container with minimum empty space
//
// december 2022.
// (c) Vlad
// ------------------------------------------------------------------

#ifndef KNACK_PACK_BIN_PLACEMENT_H_
#define KNACK_PACK_BIN_PLACEMENT_H_

#include "common.h"
#include "rectplacement.h"
#include <array>

namespace kp
{
    /**
     * @brief not effective rectangle placement
     */
    class BinPlacement
    {
    public:
        std::vector<RectPlacement> m_bins;
        int m_curBin = -1;
        //! width of container
        int m_w = 0;
        //! height of container
        int m_h = 0;
        //! height map
        std::array<int, kp::maxContainerWidth> m_heightMap = {};

        // methods

        void init(int w, int h);

        bool tryPlaceRect(Rect &rect);
        void allocateNewBin();
        bool getPossiblePosition(Rect &rect);

        void log(Rect &container, const char *testName);
    };

} // namespace kp
#endif
