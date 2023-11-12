#ifndef WAVE_WRITER_H
#define WAVE_WRITER_H

#include "sampler.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <climits>
#include <iosfwd>
 
class WAVWriter
{
    public:
        static void writeSamplesToBinaryStream(Sampler* sampler, std::ofstream* wavStream);
};

#endif
