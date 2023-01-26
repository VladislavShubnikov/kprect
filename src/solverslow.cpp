// ------------------------------------------------------------------
// Knack-pack project. Place given set of rectangles
// into given container with minimum empty space
//
// december 2022.
// (c) Vlad
// ------------------------------------------------------------------

#include <iostream>
#include <chrono>
#include <random>
#include <cassert>

#include "solverslow.h"
#include "bitmap.h"

//! Need to print all indices permutations
//#define PRINT_PERMUTATION

namespace kp
{
    /**
     * @brief Indices vector permutation.
     * Used in total permutations generator
     */
    struct VecPerm
    {
        int                 m_start;
        std::vector<int>    m_indices;
        VecPerm(int s, std::vector<int>& vec)
        {
            m_start = s; m_indices = vec;
        }
    };
}

kp::SolverSlow::SolverSlow(Rect& cont, std::vector<Rect>& list)
{
    m_container = cont;
    m_figures = list;
    m_timeFillSeconds = 0.0;
    const int numFigs = static_cast<int>(list.size());
    if (numFigs > kp::maxFigures)
    {
        std::cout << "Error: too much figures in set = " << numFigs <<
            " > " << kp::maxFigures << std::endl;
    }
}
kp::SolverSlow::SolverSlow()
{
    m_container.m_w = m_container.m_h = 0;
    m_figures.clear();
    m_timeFillSeconds = 0.0;
}

void kp::SolverSlow::printTaskConditions() const
{
    std::cout << "Task conditions: " << std::endl;
    std::cout << m_container.m_w << "," << m_container.m_h << std::endl;
    std::cout << m_figures.size() << std::endl;
    for (const auto& r: m_figures)
    {
        std::cout << r.m_w << "," << r.m_h << std::endl;
    }
}

int kp::SolverSlow::getNumIterations() const
{
    return m_numIterations;
}
double kp::SolverSlow::getTimeFillSeconds() const
{
    return m_timeFillSeconds;
}


int kp::SolverSlow::getMinContainersForSet()
{
    const int numFigs = (int)m_figures.size();
    std::vector<int> indices(numFigs, 0);
    for (int i = 0; i < numFigs; i++)
        indices[i] = i;
    // num figs
    std::cout << "Num figures = " << numFigs << std::endl;

    // measure time
    int numPerm = 0;
    auto startTime = std::chrono::system_clock::now();
    {
        numPerm = getAllPermutations(indices);
    }
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedSeconds = endTime - startTime;
    double t = (elapsedSeconds.count() * (1));
    std::cout << "time elapsed = " << t << " seconds" << std::endl;

    return numPerm;
}

int kp::SolverSlow::getAllPermutations(std::vector<int>& indices)
{
    const int n = static_cast<int>(indices.size());
    int numPerm = 0;

    std::stack<VecPerm> stack;
    stack.push(VecPerm(0, indices));
    while (!stack.empty())
    {
        VecPerm desc = stack.top();
        stack.pop();
        if (desc.m_start >= n - 1)
        {
            #if defined(PRINT_PERMUTATION)
            // print out
            std::cout << "permutation: " << std::endl;
            for (int k = 0; k < n; k++)
            {
                std::cout << desc.m_indices[k] << ",";
            }
            std::cout << std::endl;
            #endif
            // do smth with indices vector (n elements)
            numPerm++;

        }
        else
        {
            for (int i = desc.m_start; i < n; i++)
            {
                // swap elements i, iStart
                {
                    int temp = desc.m_indices[i];
                    desc.m_indices[i] = desc.m_indices[desc.m_start];
                    desc.m_indices[desc.m_start] = temp;
                }
                VecPerm vecNew(desc.m_start + 1, desc.m_indices);
                stack.push(vecNew);
                // swap back
                {
                    int temp = desc.m_indices[i];
                    desc.m_indices[i] = desc.m_indices[desc.m_start];
                    desc.m_indices[desc.m_start] = temp;
                }
            } // for i

        }
    }
    return numPerm;
}

/*
void kp::SolverSlow::getAllPermutations(int iStart, int n, std::vector<int>& indices)
{
    if (iStart >= n - 1)
    {
#if defined(PRINT_PERMUTATION)
        // print out
        std::cout << "permutation: "<< std::endl;
        for (int k = 0; k < n; k++)
        {
            std::cout << indices[k] << ",";
        }
        std::cout << std::endl;
#endif
        // do smth with indices vector (n elements)
        m_numPermutations++;
        return;
    }
    // not finished
    for (int i = iStart; i < n; i++)
    {
        // swap elements i, iStart
        {
            int temp = indices[i];
            indices[i] = indices[iStart];
            indices[iStart] = temp;
        }
        getAllPermutations(iStart + 1, n, indices);
        // swap back
        {
            int temp = indices[i];
            indices[i] = indices[iStart];
            indices[iStart] = temp;
        }
    } // for i
}
*/

int kp::SolverSlow::fillContainer(const char *testName)
{
    // select one-by-one all figures and place them into bins
    // with minimum empty spaces
    const int numFigures = (int)m_figures.size();

    // indices of figures and rotation flags
    std::vector<int> indices;
    std::vector<bool> rotated;
    for (int i = 0; i < numFigures; i++)
    {
        indices.push_back(i);
        rotated.push_back(false);
    }

    auto startTime = std::chrono::system_clock::now();

    // init result with pessimism
    m_bestNumBins = 1 << 30;

    // try to place original
    //tryToPlace(indices, rotated);

    // perform shuffles of init figures
    std::mt19937    gen(0x9372); // NOLINT

    int numAttempts;
    uint64_t unumAtt = kp::getFactorial((uint64_t)numFigures + 1);
    const uint64_t nFig1 = (uint64_t)numFigures + 1;
    uint64_t unumApprox = nFig1 * nFig1 * nFig1 * 2L;
    if (unumAtt > unumApprox)
    {
        numAttempts = (int)unumApprox;
        assert(numAttempts > 0);
    }
    else
        numAttempts = (int)unumAtt;
    // !!!!!!!!
    // numAttempts = 1;

    for (int a = 0; a < numAttempts; a++)
    {
        m_numIterations++;

        // shuffle indices randomly
        for (int k = 0; k < 4; k++)
        {
            const int ia = (int)(gen() % numFigures);
            int ib = (int)(gen() % numFigures);
            while (ib == ia)
            {
                ib = (int)(gen() % numFigures);
            }
            const int t = indices[ia];
            indices[ia] = indices[ib];
            indices[ib] = t;
        } // for k

        // new random rotations
        for (int i = 0; i < numFigures; i++)
        {
            rotated.at(i) = ((gen() & 1) != 0);
        }

        tryToPlace(indices, rotated);

    }   // for a. all attempts

    // get time
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedSeconds = endTime - startTime;
    m_timeFillSeconds = (elapsedSeconds.count() * (1));

    // print best set
    m_bestPlacement.log(m_container, testName);

    return 0;
}

bool kp::SolverSlow::tryToPlace(std::vector<int>& indices, std::vector<bool>& rotated)
{
    // init placement
    BinPlacement    placement;
    placement.init(m_container.m_w, m_container.m_h);

    bool placeComplete = true;
    const int numFigures = (int)indices.size();
    for (int f = 0; f < numFigures; f++)
    {
        const int indFig = indices.at(f);
        Rect r = m_figures.at(indFig);
        if ((r.m_w > m_container.m_w) && (r.m_h > m_container.m_w))
        {
            placeComplete = false;
            continue;
        }
        // try 2 rotations
        if (rotated.at(indFig))
        {
            r.switchWH();
            // can be implacable after rotation
            if (r.m_h > m_container.m_h)
            {
                r.switchWH();
            }
        }
        placement.tryPlaceRect(r);
    } // for f, all figures

    // modify best placement
    if (placement.m_bins.size() < m_bestNumBins)
    {
        m_bestNumBins = (int)placement.m_bins.size();
        m_bestPlacement = placement;
    } // if
    return placeComplete;
}

