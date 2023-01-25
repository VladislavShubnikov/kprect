// ------------------------------------------------------------------
// Knack-pack project. Place given set of rectangles
// into given container with minimum empty space
//
// december 2022.
// (c) Vlad
// ------------------------------------------------------------------

#ifndef KNACK_PACK_SOLVER_SLOW_H
#define KNACK_PACK_SOLVER_SLOW_H


#include "binplacement.h"
#include "rectset.h"

#include <stack>
#include <vector>


namespace kp
{

    class SolverSlow
    {
    public:
        SolverSlow();
        SolverSlow(Rect &cont, std::vector<Rect> &list);
        int getMinContainersForSet();

        virtual int fillContainer(const char *testName);

        int getNumIterations() const;
        double getTimeFillSeconds() const;
        void printTaskConditions() const;

    protected:
        Rect m_container;
        std::vector<Rect> m_figures;
        int m_numPermutations = 0;
        int m_index = 0;
        int m_numIterations = 0;
        double m_timeFillSeconds = 0.0;

        // min number of bins
        std::size_t m_bestNumBins = 0;
        BinPlacement m_bestPlacement;
        RectSet m_bestRectSet;

    private:
        void getAllPermutations(int iStart, int n, std::vector<int> &indices);

        bool tryToPlace(std::vector<int> &indices, std::vector<bool> &rotated);
    };

} // namespace kp

#endif// KNACK_PACK_SOLVER_H
