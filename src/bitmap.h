// ------------------------------------------------------------------
// Knack-pack project. Place given set of rectangles
// into given container with minimum empty space
//
// december 2022.
// (c) Vlad
// ------------------------------------------------------------------

#ifndef KNACK_PACK_BITMAP_H_
#define KNACK_PACK_BITMAP_H_

#include <cstdint>
#include <vector>

// NOLINT
// use external libpng (if exists in project)
// #define USE_PNG

namespace kp
{

/**
 * @brief Bitmap simple operations: drawing functions
 * and save to bmp format
 */
class Bitmap
{
public:
    /**
     * @brief Init structure with image size
     * Need to invoke create() later to actually allocate
     * image bits
     * @param w image width
     * @param h image height
     */
    Bitmap(int w, int h);
    ~Bitmap()
    {
        destroy();
    }

    void destroy();

    /**
     * @brief Allocate image pixels
     * @return true, if success. false - no memory
     */
    bool create();

    /**
     * @brief fill entire image with given rgb color
     * @param r r component of color to fill
     * @param g g component of color to fill
     * @param b b component of color to fill
     */
    void clear(uint8_t r, uint8_t g, uint8_t b);

    /**
     * @brief Write image into file with specified name
     * @param fileName name of file
     * @return false, if io operations failed
     */
    bool writeToFile(const char *fileName);

    /**
     * @brief render outline of rectangle
     * @param xMin x corner
     * @param yMin y corner
     * @param w rectangle width
     * @param h rectangle height
     * @param colorARGB color to draw
     */
    void drawRect(int xMin, int yMin, int w,
                  int h,
                  uint32_t colorARGB);

    /**
     * @brief render internal pixels of rectangle
     * @param xMin x corner
     * @param yMin y corner
     * @param w rectangle width
     * @param h rectangle height
     * @param colorARGB color to draw
     */
    void fillRect(int xMin, int yMin, int w,
        int h,
        uint32_t colorARGB);

    /**
     * @brief render internal pixels if corcle
     * @param xCenter circle center x coordinate
     * @param yCenter circle center y coordinate
     * @param radius circle radius
     * @param colorARGB color to draw
     */
    void drawCircle(int xCenter, int yCenter, int radius,
                    uint32_t colorARGB);
    /**
     * @brief render line
     * @param x0 start point coordinate x
     * @param y0 start point coordinate y
     * @param x1 end point coordinate x
     * @param y1 end point coordinate y
     * @param colorARGB color to draw
     */
    void drawLine(int x0, int y0, int x1, int y1,
                  uint32_t colorARGB);

    /**
     * @brief render text string
     * Actually only digits render is implemented
     * @param xMin min corner of text, x coordinate
     * @param yMin min corner of text, y coordinate
     * @param hLetter letter height in pixels
     * @param sText text string
     * @param colorARGB color to draw
     */
    void drawText(int xMin, int yMin, int hLetter, 
                    const char *sText, uint32_t colorARGB);

    /**
     * @brief get image width in pxiels
     * @return
     */
    int getWidth() const
    {
        return m_width;
    }

    /**
     * @brief get image height in pxiels
     * @return
     */
    int getHeight() const
    {
        return m_height;
    }

    /**
     * @brief get image pixels array
     * @return
     */
    uint32_t *getPixels()
    {
        return m_pixels.data();
    }

private:

#if defined(USE_PNG)
    bool writeToPng(const char *fileName);
#endif

    /**
     * @brief Estimate size of buffer to prepare BMP in memory
     * before writing to the file
     * @return size of memory in bytes
     */
    int getBufferSize() const;

    /**
     * @brief write BMP format into given memory
     * @param buffer given memory buffer
     * @param bufMaxSize size of memory buffer
     * @return 1, if correct. -1: logic fail
     */
    int writeToBuffer(uint8_t *buffer, int bufMaxSize);

    /**
     * @brief render single letter
     * Now only digits are implemented
     * @param cLetter Letter character, from '0' to '9'
     * @param xMin rectangle min x coordinate
     * @param yMin rectangle min y coordinate
     * @param xMax rectangle max x coordinate
     * @param yMax rectangle max y coordinate
     * @param colorARGB
     */
    void drawDigitLetter(char cLetter,
        int xMin, int yMin,
        int xMax, int yMax, uint32_t colorARGB);

    //! image width in pixels
    int         m_width = 0;
    //! image height in pixels
    int         m_height = 0;
    //! image itself as vector of ARGB's
    std::vector<uint32_t>   m_pixels;
};

} // namespace kp

#endif