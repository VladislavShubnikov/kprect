// ------------------------------------------------------------------
// Knack-pack project. Place given set of rectangles
// into given container with minimum empty space
//
// december 2022.
// (c) Vlad
// ------------------------------------------------------------------

#include "solverfast.h"

#include <iostream>
#include <chrono>
#include <cassert>


kp::SolverFast::SolverFast(Rect& cont, std::vector<Rect>& list)
{
    m_container = cont;
    m_figures = list;
    m_numPermutations = 0;
    const int numFigs = static_cast<int>(list.size());
    if (numFigs > kp::maxFigures)
    {
        std::cout << "Error: too much figures in set = " << numFigs <<
            " > " << kp::maxFigures << std::endl;
    }

}

// ************************************************************
// Optimized fast fill (greedy search)
// ************************************************************
int kp::SolverFast::fillContainer(const char *testName)
{
    const int numFigures = static_cast<int>(m_figures.size());
    //std::mt19937    gen(36256);

    srand(8162); // NOLINT

    // pessimistic
    m_bestNumBins = 1 << 30;

    // get start time
    auto startTime = std::chrono::system_clock::now();

    m_numIterations = 0;

    const int numVals = numFigures + 1;
    uint64_t numMax = kp::getFactorial(numVals);
    const int numAttempts = (numMax < 4096) ?
        (int)numMax : (numVals * numVals);
    for (int a = 0; a < numAttempts; a++)
    {
        m_numIterations++;

        RectSet         set;
        set.init(m_container.m_w, m_container.m_h, numFigures);

        if (a < numFigures)
            set.setStrategy(Strategy::MAX_GREEDY);
        else
            set.setStrategy(Strategy::RANDOM);

        std::bitset<kp::maxFigures> used;
        used.reset();


        // select 1st random figure
        //const int ind = static_cast<int>(gen() % numFigures);
        const int ind = a % numFigures;

        Rect rectFirst = m_figures.at(ind);
        used.set(ind, true);
        set.placeFirst(rectFirst);

        // one by one, place all figures
        for (int f = 1; f < numFigures; f++)
        {
            Rect r;
            int indSelected = set.selectBestFigure(m_figures, used, r);
            if (indSelected < 0)
            {
                set.allocateNewBin();
                indSelected = set.selectBestFigure(m_figures, used, r);
                assert(indSelected >= 0);
            }
            //used.set(indSelected, true);
        } // for f, all figures

        // update best solution
        if (set.m_bins.size() < m_bestNumBins)
        {
            m_bestNumBins = set.m_bins.size();
            m_bestRectSet = set;
        }

    } // for a. all attempts



    // get time
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedSeconds = endTime - startTime;
    m_timeFillSeconds = (elapsedSeconds.count() * (1));

    // print best set
    m_bestRectSet.log(m_container, testName);

    return 0;
}
