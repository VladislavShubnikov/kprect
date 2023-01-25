// ------------------------------------------------------------------
// Knack-pack project. Place given set of rectangles
// into given container with minimum empty space
//
// december 2022.
// (c) Vlad
// ------------------------------------------------------------------

#include "binplacement.h"
#include "bitmap.h"
#include "palette.h"

#include <cassert>
#include <iostream>
#include <sstream>


void kp::BinPlacement::init(int w, int h)
{
    m_w = w;
    m_h = h;
    for (int i = 0; i < w; i++)
    {
        m_heightMap.at(i) = 0;
    }
    m_bins.clear();
    m_curBin = 0;

    RectPlacement emptyPlacement;
    m_bins.push_back(emptyPlacement);
}

void kp::BinPlacement::allocateNewBin()
{
    // add current placement
    RectPlacement emptyPlacement;
    m_bins.push_back(emptyPlacement);

    // m_curPlacement.m_rects.clear();
    for (int i = 0; i < m_w; i++)
    {
        m_heightMap[i] = 0;
    }
    m_curBin++;
}

void kp::BinPlacement::log(Rect &container, const char *testName)
{
    const int W_BMP = container.m_w;
    const int H_BMP = container.m_h;

    const int numBins = (int) m_bins.size();
    for (int bin = 0; bin < numBins; bin++)
    {
        kp::Bitmap bitmap(W_BMP, H_BMP);
        if (!bitmap.create())
        {
            return;
        }
        RectPlacement rPlace = m_bins.at(bin);

        for (auto &rect: rPlace.m_rects)
        {
            // rectangle id
            const int id = rect.m_id;
            uint32_t r, g, b;
            Palette::getBackground(id, r, g, b);
            uint32_t colDraw = 0xFF000000 | (r << 16) | (g << 8) | b;
            Palette::getForeground(id, r, g, b);
            uint32_t colText = 0xFF000000 | (r << 16) | (g << 8) | b;


            const int xMin = rect.m_x;
            const int xMax = xMin + rect.m_w;
            const int yMin = rect.m_y;
            const int yMax = yMin + rect.m_h;

            bitmap.fillRect(xMin, yMin, rect.m_w, rect.m_h, colDraw);

            // rectangle shape as lines
            uint32_t col = 0xff000000;
            bitmap.drawLine(xMin, yMin, xMin, yMax, col);
            bitmap.drawLine(xMax, yMin, xMax, yMax, col);
            bitmap.drawLine(xMin, yMin, xMax, yMin, col);
            bitmap.drawLine(xMin, yMax, xMax, yMax, col);
            // diagonals
            bitmap.drawLine(xMin, yMin, xMax, yMax, col);
            bitmap.drawLine(xMin, yMax, xMax, yMin, col);

            // rectangle id to text
            char str[32];
            std::snprintf(str, 32, "%02d", id);
            const int minRectSize = (rect.m_w < rect.m_h) ? rect.m_w : rect.m_h;
            const int hTextHeight = (minRectSize / 2 >= 16) ? 16 : 9;
            bitmap.drawText(xMin + 2, yMin + 2, hTextHeight, str, colText);
        }// for all rects

        // prepare image file name
        std::ostringstream stream;
        stream << "dump_" << testName << "_" << bin << ".bmp";
        std::string str = stream.str();
        const char *cFileName = str.data();

        // save bitmap
        bitmap.writeToFile(cFileName);
    }// for b, all bins
}

bool kp::BinPlacement::tryPlaceRect(Rect &rect)
{
    bool isPossible = getPossiblePosition(rect);
    if (!isPossible)
    {
        allocateNewBin();
        isPossible = getPossiblePosition(rect);
        assert(isPossible);
    }
    // add possible pos
    //m_curPlacement.m_rects.push_back(rect);
    m_bins.at(m_curBin).m_rects.push_back(rect);

    // modify height map
    const int yNew = rect.m_y + rect.m_h;
    for (int x = rect.m_x; x < rect.m_x + rect.m_w; x++)
    {
        if (m_heightMap.at(x) < yNew)
        {
            m_heightMap.at(x) = yNew;
        }
    }// for x
    return true;
}

bool kp::BinPlacement::getPossiblePosition(Rect &rect)
{
    // try using height map
    Pos pLowest;
    pLowest.m_x = -1;
    pLowest.m_y = 1 << 30;
    for (int x = 0; x < m_w;)
    {
        // using current x, check on height map
        if (m_heightMap.at(x) + rect.m_h > m_h)
        {
            // not fit to container by height
            x++;
            continue;
        }
        int yPlace = m_heightMap.at(x);
        // check rest of rect width pixels with height map
        if (x + rect.m_w > m_w)
        {
            break;
        }
        bool isPossible = true;
        int xx = x + 1;
        for (; xx < x + rect.m_w; xx++)
        {
            if (m_heightMap.at(xx) > yPlace)
            {
                isPossible = false;
                break;
            }
        }
        if (!isPossible)
        {
            x = xx;
            if (x + 1 >= m_w)
            {
                break;
            }
            // skip the same height in height map
            yPlace = m_heightMap.at((std::size_t) x + 1);
            for (; x < m_w; x++)
            {
                if (m_heightMap.at(x) != yPlace)
                    break;
            }
            // search new start pos again
            continue;
        }
        // position (x, yPlace) is possible: check is it minimum
        if (yPlace < pLowest.m_y)
        {
            pLowest.m_y = yPlace;
            pLowest.m_x = x;
        }
        // skip for next x
        x++;
    }// for x, all in [0.. m_w]
    if (pLowest.m_x >= 0)
    {
        rect.m_x = pLowest.m_x;
        rect.m_y = pLowest.m_y;
        return true;
    }
    return false;
}
