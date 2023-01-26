// ------------------------------------------------------------------
// Knack-pack project. Place given set of rectangles
// into given container with minimum empty space
//
// december 2022.
// (c) Vlad
// ------------------------------------------------------------------

#include "rectset.h"
#include "bitmap.h"
#include "palette.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <algorithm>


void kp::RectSet::init(int w, int h, int numFigures)
{
    m_w = w;
    m_h = h;
    m_heightMap.resize(w);
    for (int i = 0; i < w; i++)
    {
        m_heightMap.at(i) = 0;
    }
    m_bins.clear();
    m_curBin = 0;

    // need to reserve memory twice more than actual number of
    // rectangles: each rectangle can be placed in two rotations
    m_rectDescriptions.resize(numFigures * 2);

    RectPlacement   emptyPlacement;
    m_bins.push_back(emptyPlacement);
    m_numDescriptions = 0;
}

void kp::RectSet::allocateNewBin()
{
    kp::RectPlacement   emptyPlacement;
    m_bins.push_back(emptyPlacement);

    for (int i = 0; i < m_w; i++)
    {
        m_heightMap[i] = 0;
    }
    m_curBin++;
}

void kp::RectSet::log(Rect& container, const char *testName)
{
    const int numBins = static_cast<int>(m_bins.size());
    for (int bin = 0; bin < numBins; bin++)
    {
        kp::Bitmap bitmap(container.m_w, container.m_h);
        if (!bitmap.create())
        {
            return;
        }
        RectPlacement rPlace = m_bins.at(bin);

        for (auto& rect : rPlace.m_rects)
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

        }   // for all rects

        // prepare image file name
        std::ostringstream stream;
        stream << "dump_" << testName << "_" <<
            bin << ".bmp";
        std::string str = stream.str();
        const char *cFileName = str.data();

        // save bitmap
        bitmap.writeToFile(cFileName);
    }   // for b, all bins

}

void kp::RectSet::placeFirst(Rect& rectFirst)
{
    // place at (0,0)
    rectFirst.m_x = rectFirst.m_y = 0;
    const int hNew = rectFirst.m_h;
    for (int x = 0; x < rectFirst.m_w; x++)
    {
        m_heightMap.at(x) = hNew;
    } // for x
    // update bins
    m_bins.at(m_curBin).m_rects.push_back(rectFirst);
}

/*
 * Convert height map into array of segments
 * with start, length and height
 *
 */
static std::vector<kp::Segment>
getHeightMapParts(std::vector<int>& heightMap, const int w)
{
    std::vector<kp::Segment> segments;
    int hPrev = heightMap.at(0);
    int xStart = 0;
    int x = 1;
    for (; x < w; x++)
    {
        int h = heightMap.at(x);
        if (h != hPrev)
        {
            // generate line from xStart to (x - 1)
            kp::Segment seg;
            seg.m_start = xStart;
            seg.m_size = x - xStart;
            seg.m_height = hPrev;
            segments.push_back(seg);
            xStart = x;
            hPrev = h;
        }
    } // for x, all width

    // add last part
    kp::Segment seg;
    seg.m_start = xStart;
    seg.m_size = x - xStart;
    seg.m_height = hPrev;

    segments.push_back(seg);
    return segments;
}

/*
 * Get best figure (rect) that can be placed on th given segment
 */
int kp::RectSet::getBestFigure(Segment& seg,
    std::vector<Rect>& figures,
    std::bitset<kp::maxFigures>& used,
    bool& isOrientationNorm
)
{
    const int numFigures = static_cast<int>(figures.size());
    const int lenFit = seg.m_size;
    const bool isLastSegment = (seg.m_start + seg.m_size >= m_w);

    int     lenBest = 0;
    int     indexBest = -1;
    bool    orientationBestNorm = true;

    bool perfectMatch = false;
    m_numDescriptions = 0;

    for (int f = 0; f < numFigures; f++)
    {
        // check figure not used
        if (used.test(f))
        {
            continue;
        }
        Rect rect = figures.at(f);
        int lenFig = rect.m_w;
        int heiFig = rect.m_h;
        if ((lenFig <= lenFit) && (seg.m_height + heiFig <= m_h))
        {
            // add to descriptions
            RectDescr descr;
            descr.m_index = f;
            descr.m_orientNorm = true;
            m_rectDescriptions.at(m_numDescriptions) = descr;
            m_numDescriptions++;
            // update best matches
            if (lenFig > lenBest)
            {
                lenBest = lenFig;
                indexBest = f;
                orientationBestNorm = true;
                if (lenFig == lenFit)
                {
                    perfectMatch = true;
                }
            }
        }
        lenFig = rect.m_h;
        heiFig = rect.m_w;
        if ((lenFig <= lenFit) && (seg.m_height + heiFig <= m_h))
        {
            // add to descriptions
            RectDescr descr;
            descr.m_index = f;
            descr.m_orientNorm = false;
            m_rectDescriptions.at(m_numDescriptions) = descr;
            m_numDescriptions++;

            // update best matches
            if (lenFig > lenBest)
            {
                lenBest = lenFig;
                indexBest = f;
                orientationBestNorm = false;
                if (lenFig == lenFit)
                {
                    perfectMatch = true;
                }

            }
        }
    } // for f , all figures
    // Best figure is now ready: need to place it into segment seg.
    if (indexBest < 0)
    {
        return -1;
    }

    if (m_strategy == Strategy::RANDOM)
    {
        // only last segment with perfect match
        // will return the best figure
        if (isLastSegment && perfectMatch)
        {
            isOrientationNorm = orientationBestNorm;
            return indexBest;
        }
        // no perfect match found OR not last segment
        assert(m_numDescriptions > 0);
        const int indexArr = (int)(rand() % m_numDescriptions); // NOLINT
        int indexRect = m_rectDescriptions.at(indexArr).m_index;
        isOrientationNorm = m_rectDescriptions.at(indexArr).m_orientNorm;
        return indexRect;
    }
    if (m_strategy == Strategy::MAX_GREEDY)
    {
        if (!perfectMatch)
        {
            assert(m_numDescriptions > 0);
            const int indexArr = (int)(rand() % m_numDescriptions); // NOLINT
            isOrientationNorm = m_rectDescriptions.at(indexArr).m_orientNorm;
            int indexRect = m_rectDescriptions.at(indexArr).m_index;
            return indexRect;
        }
        isOrientationNorm = orientationBestNorm;
        return indexBest;
    }
    return -1;
}

void kp::RectSet::setStrategy(Strategy s)
{
    m_strategy = s;
}

int kp::RectSet::selectBestFigure(
                                std::vector<Rect>& figures, 
                                std::bitset<kp::maxFigures>& used, 
                                Rect& rectOut)
{
    std::vector<Segment> segments = 
        getHeightMapParts(m_heightMap, m_w);
    
    // sort segments: first will be lowest
    std::sort(segments.begin(), segments.end(),
        [](const auto& lhs, const auto& rhs)
        {
            bool less = lhs.m_height < rhs.m_height;
            return less;
        }
            );
    // const int numFigures = static_cast<int>(figures.size());
    // get lowest segment and select best figure to place
    Segment seg = segments.at(0);

    bool isOrientNorm = true;
    int indexBestFig = getBestFigure(seg, figures, used, isOrientNorm);
    if (indexBestFig >= 0)
    {
        // mark selected figure is used now
        used.set(indexBestFig, true);
        // update height map
        Rect rect = figures.at(indexBestFig);
        if (!isOrientNorm)
        {
            rect.switchWH();
        }
        for (int x = seg.m_start; x < seg.m_start + rect.m_w; x++)
        {
            m_heightMap.at(x) = seg.m_height + rect.m_h;
        }
        rectOut = rect;
        rectOut.m_x = seg.m_start;
        rectOut.m_y = seg.m_height;

        // update bins
        m_bins.at(m_curBin).m_rects.push_back(rectOut);
        return indexBestFig;
    }
    return -1;
}
