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
        //! Storage to keep all arranged rectangles
        std::vector<RectPlacement> m_bins;
        //! Current bin, increasing number with iterations
        int m_curBin = -1;
        //! width of container
        int m_w = 0;
        //! height of container
        int m_h = 0;
        //! height map
        std::array<int, kp::maxContainerWidth> m_heightMap = {};

        // methods

        /**
         * @brief Initialize containers
         */
        void init(int w, int h);

        /**
         * @brief Attempt to place new given rectangle
         * @return true , if arranged successfully
         */
        bool tryPlaceRect(Rect &rect);


        /**
         * @brief Allocate new container and initialize it.
         */
        void allocateNewBin();

        /**
         * @brief Calculate possible position for rectangle in container
         * @return true , if arranged successfully
         */
        bool getPossiblePosition(Rect &rect);

        /**
         * @brief Create bmp files with arranged rectangles
         * @param container Container
         * @param testName Name of test
         */
        void log(Rect &container, const char *testName);
    };

} // namespace kp
#endif
