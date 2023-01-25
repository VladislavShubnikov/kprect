// ------------------------------------------------------------------
// Knack-pack project. Place given set of rectangles
// into given container with minimum empty space
//
// december 2022.
// (c) Vlad
// ------------------------------------------------------------------

#include "rect.h"

bool kp::Rect::isWidthGreater() const
{
    return m_w > m_h;
}

void kp::Rect::switchWH()
{
    const int t = m_w;
    m_w = m_h;
    m_h = t;
}
