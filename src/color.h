#ifndef COLOR_H
#define COLOR_H

namespace minecpp
{
    namespace color
    {
        constexpr unsigned make(unsigned r, unsigned g, unsigned b) noexcept
        {
            return r | (g << 4) | (b << 8);
        }

        /*
         * 07 12 05 10
         * 07 00 05 10 & 0xF0FF                 0xFFF0 0xFF0F 0xF0FF 0x0FFF
         * 07 01 05 10 | 1 << 8
         * (color & (0xFFFF ^ (0x000F << (index << 2)))) | (component << (index << 2))
         */
        constexpr unsigned setComponent(unsigned color, unsigned index, unsigned value) noexcept
        {
            return (color & (0xFFFF ^ (0x000F << (index << 2)))) | (value << (index << 2));
        }

        constexpr unsigned getComponent(unsigned color, unsigned index) noexcept
        {
            return color >> (index << 2) & 15;
        }
    }
}

#endif
