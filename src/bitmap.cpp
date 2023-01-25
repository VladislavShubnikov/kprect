// ------------------------------------------------------------------
// Knack-pack project. Place given set of rectangles
// into given container with minimum empty space
//
// december 2022.
// (c) Vlad
// ------------------------------------------------------------------


//
// Simple bmp format utility: create image,
// draw lines, spheres, rectangles
// and save to *.bmp file
//

#include "bitmap.h"

#include <fstream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cassert>

#ifdef USE_PNG
#include <png.h>
#endif




// ************************************************************
// point struct for draw letters
// ************************************************************
namespace kp
{
    struct Point2d {
        int x, y;
    };
}

// ************************************************************

#if defined(__unix__) || defined(__linux__)
struct __attribute__((__packed__)) BmpFileHeader
#endif
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <pshpack2.h>
    struct BmpFileHeader
#endif
{
    uint16_t    m_sign = 0x4d42; // NOLINT
    uint32_t    m_fileSize = 0; // NOLINT
    uint32_t    m_zero = 0; // NOLINT
    uint32_t    m_offBits = 0; // NOLINT
};


#if defined(__unix__) || defined(__linux__)
struct __attribute__((__packed__)) BmpInfo
#endif
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    struct BmpInfo
#endif
{
    uint32_t    m_structSize = 0; // NOLINT
    uint32_t    m_width = 0; // NOLINT
    uint32_t    m_height = 0; // NOLINT
    uint16_t    m_planes = 1; // NOLINT
    uint16_t    m_bitCount = 32; // NOLINT
    uint32_t     m_compression = 0; // NOLINT
    uint32_t     m_sizeImageBytes = 0; // NOLINT
    uint32_t     m_xPixelsPerMeter = 0; // NOLINT
    uint32_t     m_yPixelsPerMeter = 0; // NOLINT
    uint32_t     m_colorsUsed = 0; // NOLINT
    uint32_t     m_colorsImportant = 0; // NOLINT
};

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <poppack.h>
#endif

kp::Bitmap::Bitmap(int w, int h)
{
    assert(w > 0);
    assert(h > 0);
    assert(w < 1024 * 8);
    assert(h < 1024 * 8);
    m_width =  w; m_height = h;
    // m_pixels = nullptr;
}


void kp::Bitmap::destroy()
{
    //delete[] m_pixels;
    //m_pixels = nullptr;
}
bool kp::Bitmap::create()
{
    //delete[] m_pixels;
    try {
        //m_pixels = new uint32_t[m_width * m_height];
        m_pixels.resize(m_width * m_height);
    } catch (std::bad_alloc& exc)
    {
        return false;
    }
    clear(255, 255, 255);
    return true;
}

void kp::Bitmap::clear(uint8_t r, uint8_t g, uint8_t b)
{
    assert(!m_pixels.empty());

    int n = m_width * m_height;
    uint32_t col = (uint32_t)b | ((uint32_t)g << 8) | ((uint32_t)r << 16) |
                   0xff000000;

    for (int i = 0; i < n; i++)
    {
        m_pixels[i] = col;
    }

}

int kp::Bitmap::getBufferSize() const
{
    int sz = 0;
    sz += sizeof(BmpFileHeader);
    sz += sizeof(BmpInfo);
    sz += 4 * m_width * m_height;
    return sz;
}
int kp::Bitmap::writeToBuffer(uint8_t *buffer, int bufMaxSize)
{
    BmpFileHeader   head;
    BmpInfo         info;
    uint8_t        *dst = buffer;

    assert(!m_pixels.empty());

    head.m_sign = 0x4d42;
    head.m_zero = 0;
    head.m_offBits = sizeof(BmpFileHeader) + sizeof(BmpInfo);
    head.m_fileSize = sizeof(BmpFileHeader) + sizeof(BmpInfo) + 4 * m_width *
                                                                    m_height;

    // make clang tidy happy with unused struct fields
    assert(head.m_sign != 0);
    assert(head.m_zero == 0);
    assert(head.m_offBits != 0);
    assert(head.m_fileSize != 0);

    memcpy(dst, &head, sizeof(head));
    dst += sizeof(head);

    info.m_structSize = sizeof(BmpInfo);
    info.m_width = (uint32_t)m_width;
    info.m_height = (uint32_t)m_height;
    info.m_planes = 1;
    info.m_bitCount = 32;
    info.m_compression = 0;
    info.m_sizeImageBytes = (uint32_t)(m_width * m_height * 4);
    info.m_xPixelsPerMeter = 0;
    info.m_yPixelsPerMeter = 0;
    info.m_colorsUsed = 0;
    info.m_colorsImportant = 0;

    // make clang tidy happy with unused struct fields
    assert(info.m_structSize != 0);
    assert(info.m_width != 0);
    assert(info.m_height != 0);
    assert(info.m_planes != 0);
    assert(info.m_bitCount != 0);
    assert(info.m_compression == 0);
    assert(info.m_sizeImageBytes != 0);
    assert(info.m_xPixelsPerMeter == 0);
    assert(info.m_yPixelsPerMeter == 0);
    assert(info.m_colorsUsed == 0);
    assert(info.m_colorsImportant == 0);

    memcpy(dst, &info, sizeof(info));
    dst += sizeof(info);

    // write RGB bits^M
    //int numPixels = m_width * m_height;
    //int j = 0;
    auto* pix = (uint8_t *)m_pixels.data();
    // write with mirror image on height
    for (int y = 0; y < m_height; y++)
    {
        uint8_t *src = pix + (m_height - 1 - y) * m_width * 4;
        for (int x = 0; x < m_width; x++)
        {
            *dst++ = *src++;
            *dst++ = *src++;
            *dst++ = *src++;
            *dst++ = *src++;
        }
    }
    if (dst - buffer > bufMaxSize)
    {
        return -1;
    }

    return 1;
}
bool kp::Bitmap::writeToFile(const char *fileName)
{
    assert(!m_pixels.empty());

    #if defined(USE_PNG)
    if (strstr(fileName, ".png") != nullptr)
    {
        return writeToPng(fileName);
    }
    #endif
    const int bufSize = getBufferSize();
    std::vector<uint8_t> buf(bufSize, 0);
    uint8_t *data = buf.data();
    writeToBuffer(data, bufSize);

    std::ofstream fs(fileName, std::ios::out | std::ios::binary);
    fs.write((const char*)(data), bufSize);
    fs.close();

    return true;
}

#if defined(USE_PNG)
bool Bitmap::writeToPng(const char *fileName)
{
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr,
                                              nullptr,
                                              nullptr);
    if (png == nullptr)
    {
        return false;
    }
    png_infop info = png_create_info_struct(png);
    if (info == nullptr)
    {
        return false;
    }

    FILE *fp = ::std::fopen(fileName, "wb");
    if(fp == nullptr)
    {
        return false;
    }
    if (setjmp(png_jmpbuf(png))) abort();

    png_init_io(png, fp);
    // Output is 8bit depth, RGBA format.
    png_set_IHDR(
        png,
        info,
        m_width, m_height,
        8,
        PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png, info);

    // To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
    // Use png_set_filler().
    //png_set_filler(png, 0, PNG_FILLER_AFTER);

    //auto* rowPointers = new png_bytep[m_height];
    std::vector<png_bytep> rowPointers;
    rowPointers.reserve(m_height);

    for (int i = 0; i < m_height; i++)
    {
        rowPointers[i] = (png_bytep)(this->m_pixels + i * m_width);
    }

    // convert ARGB into ABGR
    const int numPixels = m_width  * m_height;
    for (int i = 0; i < numPixels; i++)
    {
        uint32_t val = m_pixels[i];
        uint32_t b = val & 0xff; val >>= 8;
        uint32_t g = val & 0xff; val >>= 8;
        uint32_t r = val & 0xff; val >>= 8;
        uint32_t a = val & 0xff;
        m_pixels[i]= (a << 24) | (b << 16) | (g << 8) | r;
    }

    png_write_image(png, rowPointers.data());
    png_write_end(png, NULL);

    // delete [] rowPointers;

    ::std::fclose(fp);

    png_destroy_write_struct(&png, &info);

    // convert back to ARGB
    for (int i = 0; i < numPixels; i++)
    {
        uint32_t val = m_pixels[i];
        uint32_t b = val & 0xff; val >>= 8;
        uint32_t g = val & 0xff; val >>= 8;
        uint32_t r = val & 0xff; val >>= 8;
        uint32_t a = val & 0xff;
        m_pixels[i]= (a << 24) | (b << 16) | (g << 8) | r;
    }

    return true;
}
#endif

// NOLINTNEXTLINE
void kp::Bitmap::drawRect(int xMin, int yMin, int w,
                      int h,
                      uint32_t colorARGB)
{
    assert(!m_pixels.empty());

    uint32_t c = colorARGB;
    uint32_t bSrc = (c >> 0) & 0xff;
    uint32_t gSrc = (c >> 8) & 0xff;
    uint32_t rSrc = (c >> 16) & 0xff;
    uint32_t aSrc = (c >> 24) & 0xff;
    const int xMax = xMin + w;
    const int yMax = yMin + h;

    for (int y = yMin; y < yMax; y++)
    {
        const int yOff = y * m_width;
        for (int x = xMin; x < xMax; x++)
        {
            if ((x < 0) || (y < 0) || (x >= m_width) || (y >= m_height))
            {
                continue;
            }

            uint32_t valDst = m_pixels[x + yOff];
            uint32_t bDst = (valDst >> 0) & 0xff;
            uint32_t gDst = (valDst >> 8) & 0xff;
            uint32_t rDst = (valDst >> 16) & 0xff;

            bDst = (bDst * (256 - aSrc) + bSrc * aSrc) >> 8;
            gDst = (gDst * (256 - aSrc) + gSrc * aSrc) >> 8;
            rDst = (rDst * (256 - aSrc) + rSrc * aSrc) >> 8;

            m_pixels[x + yOff] = 0xff000000 | (rDst << 16) | (gDst << 8) | bDst;
        } // for x
    } // for y
}

/**
 * Fill rectangle with internal pixels
 */
void kp::Bitmap::fillRect(int xMin, int yMin, int w,
    int h,
    uint32_t colorARGB)
{
    assert(!m_pixels.empty());

    const int xMax = xMin + w;
    const int yMax = yMin + h;

    for (int y = yMin; y < yMax; y++)
    {
        const int yOff = y * m_width;
        for (int x = xMin; x < xMax; x++)
        {
            if ((x < 0) || (y < 0) || (x >= m_width) || (y >= m_height))
            {
                continue;
            }
            m_pixels[x + yOff] = colorARGB;
        } // for x
    } // for y

}


// NOLINTNEXTLINE
void kp::Bitmap::drawCircle(int xCenter, int yCenter, int radius,
                uint32_t colorARGB)
{
    assert(!m_pixels.empty());

    int xMin = xCenter - radius;
    int xMax = xCenter + radius;
    int yMin = yCenter - radius;
    int yMax = yCenter + radius;
    int rd2 = radius * radius;

    uint32_t c = colorARGB;
    uint32_t bSrc = (c >> 0) & 0xff;
    uint32_t gSrc = (c >> 8) & 0xff;
    uint32_t rSrc = (c >> 16) & 0xff;
    uint32_t aSrc = (c >> 24) & 0xff;

    for (int y = yMin; y <= yMax; y++)
    {
        if (y < 0)
        {
            continue;
        }
        if (y >= m_height)
        {
            break;
        }
        const int yOff = y * m_width;
        for (int x = xMin; x <= xMax; x++)
        {
            if (x < 0)
            {
                continue;
            }
            if (x >= m_width)
            {
                break;
            }
            int dx = x - xCenter;
            int dy = y - yCenter;
            int dist2 = dx * dx + dy * dy;
            if (dist2 < rd2)
            {
                uint32_t valDst = m_pixels[x + yOff];
                uint32_t bDst = (valDst >> 0) & 0xff;
                uint32_t gDst = (valDst >> 8) & 0xff;
                uint32_t rDst = (valDst >> 16) & 0xff;

                bDst = (bDst * (256 - aSrc) + bSrc * aSrc) >> 8;
                gDst = (gDst * (256 - aSrc) + gSrc * aSrc) >> 8;
                rDst = (rDst * (256 - aSrc) + rSrc * aSrc) >> 8;

                m_pixels[x + yOff] = 0xff000000 | (rDst << 16) | (gDst << 8) | bDst;

            }
        } // for x
    } // for y
}
void kp::Bitmap::drawLine(int x0, int y0, int x1, int y1,
              uint32_t colorARGB)
{
    assert(!m_pixels.empty());

    const int accBits = 10;
    const int dx = x1 - x0, dy = y1 - y0;
    const int adx = ::std::abs(dx), ady = ::std::abs(dy);
    // line is actually point
    if ((adx == 0) && (ady == 0))
    {
        if ((x0 >= 0) && (y0 >= 0) && (x0 < m_width) && (y0 < m_height))
        {
            m_pixels[x0 + y0 * m_width] = colorARGB;
        }
        return;
    }
    if (adx >= ady)
    {
        // more similar to hor line
        int step = (dy << accBits) / adx;
        int yAcc = y0 << accBits;
        int x = x0;
        int stepLine = (dx > 0) ? +1: -1;
        for (int i = 0; i < adx; i++)
        {
            int y = yAcc >> accBits;
            // put point in x, y
            if ( (x >= 0) && (y >= 0) && (x < m_width) && (y < m_height))
            {
                m_pixels[x + y * m_width] = colorARGB;
            }
            x += stepLine;
            yAcc += step;
        }
    } // if hor line
    else
    {
        // more similar to vert line
        int step = (dx << accBits) / ady;
        int xAcc = x0 << accBits;
        int y = y0;
        int stepLine = (dy > 0) ? +1: -1;
        for (int i = 0; i < ady; i++)
        {
            int x = xAcc >> accBits;
            // put point in x, y
            if ( (x >= 0) && (y >= 0) && (x < m_width) && (y < m_height))
            {
                m_pixels[x + y * m_width] = colorARGB;
            }
            xAcc += step;
            y += stepLine;
        }

    }
}


// 0 1 2 3 4 5 6 7 8 9

// each symbol is initially drawn in 47 * 64 rectangle
static kp::Point2d arr0[] =
{
    {29, 1}, {18 , 1}, {14, 3}, {10, 7}, {6, 14}, {5, 20},
    {5, 42}, {7, 50}, {9, 55}, {13, 59}, {16, 61}, {19, 62},
    {29, 62}, {34, 59}, {38, 56}, {40, 52}, {42, 46}, {43, 42},
    {43, 20}, {41, 14}, {39, 9}, {36, 5}, {33, 3}, {29, 1}
};
static const int num0 = sizeof(arr0) / sizeof(kp::Point2d);
static kp::Point2d arr1[] =
{
    {10, 15}, {16, 11}, {22, 6}, {25, 2},
    {27, 1}, {31, 1}, {31, 62}
};
static const int num1 = sizeof(arr1) / sizeof(kp::Point2d);

static kp::Point2d arr2[] =
{
    {5, 18}, {6, 12}, {11, 5}, {14, 3}, {19, 1},
    {29, 1}, {36, 4}, {41, 9}, {42, 13}, {42, 22},
    {40, 27}, {36, 33}, {25, 43}, {4, 62}, {43, 62}
};
static const int num2 = sizeof(arr2) / sizeof(kp::Point2d);
static kp::Point2d arr3[] =
{
    {6, 16}, {8, 8}, {13, 3}, {18, 1}, {28, 1},
    {36, 5}, {39, 9}, {40, 12}, {40, 19},
    {38, 32}, {27, 27},
    {38, 30}, {42, 35}, {44, 39}, {44, 47},
    {42, 52}, {37, 58}, {29, 62}, {19, 62},
    {14, 60}, {10, 57}, {6, 51}, {5, 46}
};
static const int num3 = sizeof(arr3) / sizeof(kp::Point2d);
static kp::Point2d arr4[] =
{
    {42, 47}, {5, 47}, {5, 39}, {32, 1}, {37, 1}, {37, 62}
};
static const int num4 = sizeof(arr4) / sizeof(kp::Point2d);
static kp::Point2d arr5[] =
{
    {40, 2}, {11, 2}, {7, 25}, {20, 22}, {26, 21},
    {31, 22}, {37, 25}, {41, 30}, {43, 36}, {43, 46},
    {41, 52}, {37, 57}, {31, 61}, {28, 62},
    {18, 62}, {12, 59}, {7, 54}, {5, 50}, {5, 46}
};
static const int num5 = sizeof(arr5) / sizeof(kp::Point2d);
static kp::Point2d arr6[] =
{
    {43, 16}, {40, 8}, {35, 3}, {30, 1}, {21, 2},
    {14, 4}, {10, 8}, {6, 16}, {4, 24}, {4, 43},
    {6, 50}, {9, 56}, {12, 59}, {20, 62}, {31, 62},
    {36, 60}, {41, 54}, {44, 47}, {44, 38},
    {41, 31}, {37, 27}, {31, 24}, {24, 23}, 
    {17, 24}, {11, 27}, {4, 35}
};
static const int num6 = sizeof(arr6) / sizeof(kp::Point2d);
static kp::Point2d arr7[] =
{
    {6, 2}, {44, 2}, {44, 8}, 
    {39, 16}, {33, 26}, {29, 35},
    {26, 43}, {24, 51}, {23, 62}
};
static const int num7 = sizeof(arr7) / sizeof(kp::Point2d);
static kp::Point2d arr8[] =
{
    {27, 1}, {17, 1}, {11, 4}, {7, 8}, {6, 12},
    {6, 20}, {8, 24}, 
    {23, 28}, 
    {9, 31}, {5, 36},
    {4, 39}, {4, 48}, {7, 55}, {11, 59}, {18, 62},
    {27, 62}, {34, 59}, {38, 55}, {41, 48}, {41, 39},
    {38, 33}, {33, 29}, 
    {25, 28}, 
    {35, 25}, {38, 22},
    {40, 19}, {40, 12}, {37, 7}, {32, 3}, {27, 1}
};
static const int num8 = sizeof(arr8) / sizeof(kp::Point2d);
static kp::Point2d arr9[] =
{
    {44, 27}, {37, 36}, {28, 40}, {20, 40},
    {14, 37}, {9, 33}, {6, 28}, {6, 17}, {8, 9},
    {12, 5}, {16, 2}, {20, 1}, {29, 1},
    {35, 4}, {39, 8}, {43, 15}, {44, 20},
    {44, 40}, {42, 49}, {37, 57}, {31, 61},
    {28, 62}, {18, 62}, {13, 59}, {9, 56}, 
    {7, 51}, {6, 47}
};
static const int num9 = sizeof(arr9) / sizeof(kp::Point2d);




static kp::Point2d *arrDigits[] =
{
    arr0, arr1, arr2, arr3, arr4, 
    arr5, arr6, arr7, arr8, arr9
};
static const int numPointsDigits[] =
{
    num0, num1, num2, num3, num4, 
    num5, num6, num7, num8, num9
};


/**
 * Very simple font for drawing digits
 *
 */
// NOLINTNEXTLINE
void kp::Bitmap::drawText(int xMin, int yMin, int hLetter, 
    const char *sText, uint32_t colorARGB)
{
    // make width a bit narrow
    int wLetter = hLetter * 64 / 47;
    wLetter = (int)(wLetter * 0.7);
    const int numLetters = static_cast<int>(std::strlen(sText));
    for (int i = 0; i < numLetters; i++)
    {
        char cLetter = sText[i];
        int x = xMin + i * wLetter;
        drawDigitLetter(cLetter, 
            x, yMin, 
            x + wLetter, yMin + hLetter, colorARGB);
    }
}
void kp::Bitmap::drawDigitLetter(char cLetter,
    int xMin, int yMin,
    int xMax, int yMax, uint32_t colorARGB)
{
    const int wDst = xMax - xMin, hDst = yMax - yMin;

    assert(cLetter >= '0');
    assert(cLetter <= '9');
    const Point2d *points = arrDigits[cLetter - '0'];
    const int numPoints = numPointsDigits[cLetter - '0'];
    for (int i = 1; i < numPoints; i++)
    {
        Point2d pA = points[i - 1], pB = points[i];
        float x = (float)pA.x / 47.0f;
        float y = (float)pA.y / 64.0f;
        pA.x = (int)((float)xMin + (float)wDst * x);
        pA.y = (int)((float)yMin + (float)hDst * y);
        x = (float)pB.x / 47.0f;
        y = (float)pB.y / 64.0f;
        pB.x = (int)((float)xMin + (float)wDst * x);
        pB.y = (int)((float)yMin + (float)hDst * y);
        drawLine(pA.x, pA.y, pB.x, pB.y, colorARGB);
    }
}

