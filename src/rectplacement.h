// ------------------------------------------------------------------
// Knack-pack project. Place given set of rectangles
// into given container with minimum empty space
//
// december 2022.
// (c) Vlad
// ------------------------------------------------------------------

#ifndef KNACK_PACK_RECT_PLACEMENT_H_
#define KNACK_PACK_RECT_PLACEMENT_H_

#include "rect.h"
#include <vector>

namespace kp
{
    /**
     * @brief Just container to store placed rectangles
     */
    class RectPlacement
    {
    public:
        //! places rects
        std::vector<Rect> m_rects;
    };

} // namespace kp

#endif
