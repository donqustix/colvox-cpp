#ifndef CONFIGBLOCK_H
#define CONFIGBLOCK_H

#include <string>
#include <map>

namespace minecpp
{
    struct ConfigBlock
    {
        std::map<std::string, std::string> data;
        std::map<std::string, ConfigBlock> blocks;
    };
}

#endif
