#include "chunk_default_generator.h"
#include "chunk.h"
#include "voxel.h"
#include "color.h"

#include <cstdlib>
#include <cmath>

using minecpp::ChunkDefaultGenerator;
using minecpp::Chunk;

namespace
{
    constexpr int numOctaves = 7;
    constexpr double persistence = 0.5;

    constexpr int maxPrimeIndex = 10;
    constexpr int primeIndex = 0;

    constexpr int primes[maxPrimeIndex][3] = {
      { 995615039, 600173719, 701464987 },
      { 831731269, 162318869, 136250887 },
      { 174329291, 946737083, 245679977 },
      { 362489573, 795918041, 350777237 },
      { 457025711, 880830799, 909678923 },
      { 787070341, 177340217, 593320781 },
      { 405493717, 291031019, 391950901 },
      { 458904767, 676625681, 424452397 },
      { 531736441, 939683957, 810651871 },
      { 997169939, 842027887, 423882827 }
    };

    double Noise(int i, int x, int y) noexcept
    {
        int n = x + y * 57;
        n = (n << 13) ^ n;
        int a = primes[i][0], b = primes[i][1], c = primes[i][2];
        int t = (n * (n * n * a + b) + c) & 0x7fffffff;
        return 1.0 - (double)(t)/1073741824.0;
    }

    double SmoothedNoise(int i, int x, int y) noexcept
    {
        double corners = (Noise(i, x-1, y-1) + Noise(i, x+1, y-1) +
                          Noise(i, x-1, y+1) + Noise(i, x+1, y+1)) / 16,
             sides = (Noise(i, x-1, y) + Noise(i, x+1, y) + Noise(i, x, y-1) +
                      Noise(i, x, y+1)) / 8,
             center = Noise(i, x, y) / 4;
        return corners + sides + center;
    }

    double Interpolate(double a, double b, double x) noexcept
    {
        double ft = x * 3.1415927,
               f = (1 - std::cos(ft)) * 0.5;
        return  a*(1-f) + b*f;
    }

    double InterpolatedNoise(int i, double x, double y) noexcept
    {
        int integer_X = x;
        double fractional_X = x - integer_X;
        int integer_Y = y;
        double fractional_Y = y - integer_Y;

        double v1 = SmoothedNoise(i, integer_X, integer_Y),
               v2 = SmoothedNoise(i, integer_X + 1, integer_Y),
               v3 = SmoothedNoise(i, integer_X, integer_Y + 1),
               v4 = SmoothedNoise(i, integer_X + 1, integer_Y + 1),
               i1 = Interpolate(v1, v2, fractional_X),
               i2 = Interpolate(v3, v4, fractional_X);
        return Interpolate(i1, i2, fractional_Y);
    }

    double ValueNoise_2D(double x, double y) noexcept
    {
        double total = 0,
             frequency = std::pow(2, numOctaves),
             amplitude = 1;
        for (int i = 0; i < numOctaves; ++i)
        {
            frequency /= 2;
            amplitude *= persistence;
            total += InterpolatedNoise((primeIndex + i) % maxPrimeIndex, x / frequency, y / frequency) * amplitude;
        }
        return total / frequency;
    }
}

Chunk* ChunkDefaultGenerator::generate(const ChunkLocation& chunkLocation,
        ResourceContainer& resourceContainer) noexcept
{
    const int x = chunkLocation.getX() * constants::CHUNK_WIDTH;
    const int z = chunkLocation.getZ() * constants::CHUNK_WIDTH;

    const auto chunk = new Chunk{chunkLocation, resourceContainer};

    for (int i = 0; i < constants::CHUNK_WIDTH; ++i)
    {
        for (int j = 0; j < constants::CHUNK_WIDTH; ++j)
        {
            const int height = 100 + ::ValueNoise_2D(x + j, z + i) * constants::CHUNK_HEIGHT;

            for (int k = 0; k < height; ++k)
            {
                if (std::rand() % 150 == 1)
                {
                    chunk->updateVoxel({Chunk::getVoxelIndex(j, k, i), voxel::make(2, 
                        color::make(15 * (std::rand() % 2), 15 * (std::rand() % 2), 15 * (std::rand() % 2))) });
                }
                else
                {
                    chunk->updateVoxel({Chunk::getVoxelIndex(j, k, i), voxel::make(1, 
                        color::make(15 * height / 256, 15 * j / 128, 15 * i / 128)) });
                }
            }
        }
    }

    return chunk;
}

