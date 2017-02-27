#ifndef VOXEL_H
#define VOXEL_H

namespace minecpp
{
    namespace voxel
    {
        enum Types
        {
            AIR         =    0,
            DEFAULT     =    1,
            LIGHT       =    2,
            NUM_TYPES
        };

        constexpr unsigned getType(unsigned voxel) noexcept
        {
            return voxel & 15;
        }

        constexpr unsigned getColor(unsigned voxel) noexcept
        {
            return voxel >> 4;
        }

        constexpr unsigned make(unsigned type, unsigned color) noexcept
        {
            return type | (color << 4);
        }
    }
}

#endif
