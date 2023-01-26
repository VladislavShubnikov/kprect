// ------------------------------------------------------------------
// Knack-pack project. Place given set of rectangles
// into given container with minimum empty space
//
// december 2022.
// (c) Vlad
// ------------------------------------------------------------------

#ifndef KNACK_PACK_RECT_H
#define KNACK_PACK_RECT_H

namespace kp
{

    /**
     * @brief Position in 2d , integer coordinates
     */

    class Pos
    {
    public:
        int m_x, m_y;

        Pos() : m_x(0), m_y(0) { }
        explicit Pos(int xx, int yy) :m_x(xx), m_y(yy) { }
    };

    /**
     * @brief 2d rectangle description with id, used to store
       several rectangles with the same size in set
     */
    class Rect
    {
    public:
        Rect() : m_w(0), m_h(0), m_x(0), m_y(0), m_id(0)
        {
        }
        Rect(const int ww, const int hh)
            : m_w(ww), m_h(hh), m_x(0), m_y(0), m_id(0)
        {
        }
        Rect(const int ww, const int hh, const int id)
            : m_w(ww), m_h(hh), m_x(0), m_y(0), m_id(id)
        {
        }
        bool isWidthGreater() const; // NOLINT
        void switchWH();

        int m_w, m_h;
        int m_x, m_y;
        int m_id;
    };

} // namespace kp

#endif// KNACK_PACK_RECT_H
