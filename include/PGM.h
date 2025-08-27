#ifndef OUTPUTFORMAT_H
#define OUTPUTFORMAT_H

#include "Vec3.h"
#include <cassert>
#include <cmath>
#include <fstream>

namespace PGM
{
    constexpr int max_colour{255};
    constexpr int max_colour_plus{max_colour + 1};

    inline double linearToGamma(const double linear)
    {
        if (linear == 0)
        {
            return 0;
        }
        return std::sqrt(linear);
    }

    inline int thresholdColour(const double colour)
    {
        return std::min(max_colour, static_cast<int>(max_colour_plus * colour));
    }

    inline void writeHeader(std::ofstream& file, const int width, const int height)
    {
        file << "P3\n" << width << " " << height << "\n" << max_colour << "\n";
    }

    inline void writeRGBTriple(std::ofstream& file, const Vec3& rgb)
    {
        assert(rgb[0] <= 1.0 && rgb[1] <= 1.0 && rgb[2] <= 1.0);
        assert(rgb[0] >= 0.0 && rgb[1] >= 0.0 && rgb[2] >= 0.0);
        file << thresholdColour(linearToGamma(rgb[0])) << " "
                  << thresholdColour(linearToGamma(rgb[1])) << " "
                  << thresholdColour(linearToGamma(rgb[2])) << "\n";
    }
}

#endif //OUTPUTFORMAT_H
