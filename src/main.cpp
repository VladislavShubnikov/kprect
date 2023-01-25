// ------------------------------------------------------------------
// Knack-pack project. Place given set of rectangles
// into given container with minimum empty space
//
// december 2022.
// (c) Vlad
// ------------------------------------------------------------------


#include <iostream>
#include <vector>
#include <random>

#include "rect.h"
#include "solverslow.h"
#include "solverfast.h"


//#define SOLVE_SLOW
#define SOLVE_FAST

// ******************************************************

static void testTwoBins()
{
    kp::Rect    container(300, 200);
    std::vector<kp::Rect>   figures = {
            kp::Rect(190, 50, 0),
            kp::Rect(190, 50, 1),
            kp::Rect(250, 90, 2),
            kp::Rect(250, 90, 3),
            kp::Rect(250, 90, 4),
            kp::Rect(250, 90, 5),
    };

    #if defined(SOLVE_SLOW)
    kp::SolverSlow solver(container, figures);
    const char *nameSolver = "test_slow_TwoBins";
    #endif
    #if defined(SOLVE_FAST)
    kp::SolverFast solver(container, figures);
    const char *nameSolver = "test_fast_TwoBins";
    #endif

    // print task
    std::cout << ">>>>>>>>>>>>>>>>" << std::endl;
    solver.printTaskConditions();
    std::cout << "<<<<<<<<<<<<<<<<" << std::endl;

    // try to fill 
    solver.fillContainer(nameSolver);

    std::cout << "num iterations = " << solver.getNumIterations() <<
        std::endl;
    std::cout << "time fill sec = " << solver.getTimeFillSeconds() << std::endl;
}

static void testSingle5Rects()
{
    //
    //  +-------+------------+
    //  |       |     C      |
    //  |       |            |
    //  |   A   +----+-------+
    //  |       | E  |       |
    //  |-------+----+       |
    //  |      D     |   B   |
    //  |            |       |
    //  +------------+-------+
    //
    //
    //
    kp::Rect    container(800, 590);
    std::vector<kp::Rect>   figures = {
            kp::Rect(500, 234, 0), // D
            kp::Rect(300, 354, 1), // B
            kp::Rect(300, 354, 2), // A
            kp::Rect(200, 120, 3), // E
            kp::Rect(500, 234, 4), // C
    };
    #if defined(SOLVE_SLOW)
    kp::SolverSlow solver(container, figures);
    const char *nameSolver = "test_slow_Single5Rects";
    #endif
    #if defined(SOLVE_FAST)
    kp::SolverFast solver(container, figures);
    const char *nameSolver = "test_fast_Single5Rects";
    #endif

    // print task
    std::cout << ">>>>>>>>>>>>>>>>" << std::endl;
    solver.printTaskConditions();
    std::cout << "<<<<<<<<<<<<<<<<" << std::endl;

    // try to fill 
    solver.fillContainer(nameSolver);

    std::cout << "num iterations = " << solver.getNumIterations() <<
        std::endl;
    std::cout << "time fill sec = " << solver.getTimeFillSeconds() << std::endl;
}

/**
 * TODO: Special test, generated from single 24*16 int matrix
 * Process: merge some random neighbour cells
 *
 */
static bool hasNonFilled(const int* matrix, const int numCols = 24, const int numRows = 16)
{
    for (int i = 0; i < numCols * numRows; i++)
    {
        if (matrix[i] == -1)
            return true;
    }
    return false;
}
static bool findRect(const int* matrix, const int numCols, const int numRows,
    const int wRect, const int hRect, int& xRes, int& yRes)
{
    for (int y = 0; y <= numRows - hRect; y++)
    {
        for (int x = 0; x <= numCols - wRect; x++)
        {
            bool isVacant = true;
            for (int yy = 0; (yy < hRect) && isVacant; yy++)
            {
                for (int xx = 0; xx < wRect; xx++)
                {
                    const int xc = x + xx, yc = y + yy;
                    if ((xx >= numCols) || (yy >= numRows))
                    {
                        isVacant = false;
                        break;
                    }
                    if (matrix[xc + yc * numCols] != -1)
                    {
                        isVacant = false; break;
                    }
                }
            } // for yy
            if (isVacant)
            {
                xRes = x; yRes = y;
                return true;
            }
        } // for x
    } // for y
    return false;
}
static void markFilled(int* matrix, const int numCols,
    const int xRect, const int yRect, const int wRect, const int hRect)
{
    // numRows;
    for (int y = yRect; y < yRect + hRect; y++)
    {
        for (int x = xRect; x < xRect + wRect; x++)
        {
            matrix[x + y * numCols] = 1;
        }
    }
}

static void testGen()
{
    const int numCols = 24;
    const int numRows = 16;
    int matrix[numCols * numRows];
    std::vector<kp::Rect>   figures;
    const int numPixelsInUnit = 24;

    // init
    for (int& elem: matrix)
    {
        elem = -1;
    }
    std::mt19937 gen(0x8736); // NOLINT
    int indexRect = 0;
    while (hasNonFilled(matrix, numCols, numRows))
    {
        int wRect = 1 + (int)(gen() % 6);
        int hRect = 1 + (int)(gen() % 6);
        if (figures.size() > 64)
        {
            wRect = hRect = 1;
        }
        int x, y;
        if (findRect(matrix, numCols, numRows, wRect, hRect, x, y))
        {
            kp::Rect r;
            r.m_x = x * numPixelsInUnit;
            r.m_y = y * numPixelsInUnit;
            r.m_w = wRect * numPixelsInUnit;
            r.m_h = hRect * numPixelsInUnit;
            r.m_id = indexRect;
            indexRect++;
            figures.push_back(r);
            markFilled(matrix, numCols, x, y, wRect, hRect);
        }
    }
    kp::Rect container;
    container.m_w = numCols * numPixelsInUnit;
    container.m_h = numRows * numPixelsInUnit;

    #if defined(SOLVE_SLOW)
    kp::SolverSlow solver(container, figures);
    const char *nameSolver   = "test_slow_Gen";
    const char *nameExpected = "test_slow_Gen_expected";
    #endif
    #if defined(SOLVE_FAST)
    kp::SolverFast solver(container, figures);
    const char *nameSolver = "test_fast_Gen";
    const char *nameExpected = "test_fast_Gen_expected";
    #endif


    // print expected
    {
        kp::BinPlacement placement;
        kp::RectPlacement rectPlacement;
        for (kp::Rect& r: figures)
        {
            rectPlacement.m_rects.push_back(r);
        }
        placement.m_bins.push_back(rectPlacement);
        placement.log(container, nameExpected);
    }

    std::cout << ">>>>>>>>>>>>>>>>" << std::endl;
    solver.printTaskConditions();
    std::cout << "<<<<<<<<<<<<<<<<" << std::endl;

    // try to fill 
    solver.fillContainer(nameSolver);

    std::cout << "num iterations = " << solver.getNumIterations() <<
        std::endl;
    std::cout << "time fill sec = " << solver.getTimeFillSeconds() << std::endl;

}

int main()
{
    testSingle5Rects();
    testGen();
    testTwoBins();
    return 0;
}
