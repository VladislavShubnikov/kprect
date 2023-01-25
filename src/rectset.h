// ------------------------------------------------------------------
// Knack-pack project. Place given set of rectangles
// into given container with minimum empty space
//
// december 2022.
// (c) Vlad
// ------------------------------------------------------------------

#ifndef KNACK_PACK_RECT_SET_H
#define KNACK_PACK_RECT_SET_H

#include "common.h"
#include "rect.h"
#include "rectplacement.h"

#include <bitset>
#include <vector>

namespace kp
{

/**
  * @brief Horizontal segment description
  * Used to find place for next rectangle
  */
struct Segment {
    int m_start = 0;
    int m_size = 0;
    int m_height = 0;
};

/**
 * @brief Rectangle description
 * Can be rotated or not (two cases)
 */
struct RectDescr {
    int m_index = 0;
    bool m_orientNorm = true;
};

/**
 * @brief Strategy to place new rectangle into bin
 * Can be greedy (find largest possible rectangle to fit)
 * or random (just select any possible to fit)
 * Random and greedy strategy can be combined together
 */
enum Strategy
{
    MAX_GREEDY = 0,
    RANDOM = 1
};

/**
 * @brief Rectangle set with place new rectangle functionality
 *
 * This is a core functionality of the knack-pack algorithm
 */
class RectSet
{
public:
    //! Total result bins with rectangles
    std::vector<RectPlacement> m_bins;
    //! Index of current bin, where new rectangle can be placed
    int m_curBin = -1;
    //! Container width
    int m_w = 0;
    //! Container height
    int m_h = 0;
    //! Height map with width elements
    std::vector<int> m_heightMap;

    //! Storage for already placed rectangles
    std::vector<RectDescr> m_rectDescriptions;
    //! Current strategy for rectangle placement process
    Strategy m_strategy = Strategy::RANDOM;

    //! Number of placed rectangles
    int m_numDescriptions = 0;

    // methods

    /**
     * @brief Initialize with container dimensions and number of placed rectangles
     * @param w container width
     * @param h container height
     * @param numFigures Rectangles count
     */
    void init(int w, int h, int numFigures);

    /**
     * @brief Set current placement strategy for next rectangles
     * @param s Strategy type: @see Strategy
     */
    void setStrategy(Strategy s);

    /**
     * @brief Place first rectangle
     *
     * Update height map in special case
     * @param rectFirst Rectangle to place
     */
    void placeFirst(Rect &rectFirst);

    /**
     * @brief Select best fit figure from given set and current height map
     * @param figures source set of rectangles
     * @param used bit flags for each rectangle: used or not
     * @param rectOut Output rectangle
     * @return index of rectangle in source figures set
     */
    int selectBestFigure(std::vector<Rect> &figures, std::bitset<kp::maxFigures> &used, Rect &rectOut);

    /**
     * @brief Allocate new bin
     *
     * This function is invoked when current bin is completely filled
     * with rectangles. Initialize new bin will assign 0 to height map
     */
    void allocateNewBin();

    /**
     * @brief log ready placement to *.bmp file
     *
     * @param container container description
     * @param testName name will be used to build bitmap file name (and bin index too)
     */
    void log(Rect &container, const char *testName);

private:
    int getBestFigure(Segment &seg,
                      std::vector<Rect> &figures,
                      std::bitset<kp::maxFigures> &used,
                      bool &isOrientationNorm);
};

} // name space

#endif
