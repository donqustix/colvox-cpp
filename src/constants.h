#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace minecpp
{
    namespace constants
    {
        constexpr int BLOCK_SIZE       = 64;
        
        constexpr int CHUNK_WIDTH      = 16;
        constexpr int CHUNK_HEIGHT     = 256;

        constexpr int MAX_LIGHT_LEVEL = 15;

        constexpr float PLAYER_MOVEMENT_SPEED = 3.8F;
        constexpr float PLAYER_ROTATION_SPEED = 0.1F;
    }
}

#endif
