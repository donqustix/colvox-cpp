#ifndef CONFIG_H
#define CONFIG_H

#include "config_block.h"
#include "utility.h"

#include <algorithm>
#include <istream>

namespace minecpp
{
    class Config
    {
        ConfigBlock mainBlock;

        template<typename Char, typename Traits>
        void load(std::basic_istream<Char, Traits>& stream, ConfigBlock& block) noexcept
        {
            std::string line;
            while (std::getline(stream, line))
            {
                line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
                switch (line[0])
                {
                    case '!':
                    {
                        auto newBlockName = line.substr(1, line.length() - 1);
                        ConfigBlock newBlock;
                        load(stream, newBlock);
                        block.blocks.emplace(std::move(newBlockName), std::move(newBlock));
                        break;
                    }
                    case '/':
                        return;
                    case '*':
                        break;
                    default:
                    {
                        std::istringstream sstream{line};
                        std::string key, value;
                        std::getline(sstream, key, '=');
                        std::getline(sstream, value);
                        block.data.emplace(std::move(key), std::move(value));
                        break;
                    }
                }
            }
        }

        const ConfigBlock& findBlock(const std::string& route) const noexcept;

    public:
        static Config parseFile(const std::string& filepath);

        template<typename Char, typename Traits>
        explicit Config(std::basic_istream<Char, Traits>& stream) noexcept
        {
            load(stream, mainBlock);
        }

        template<typename T>
        T findValue(const std::string& key, const std::string& route) const noexcept
        {
            return utility::convert<T>(findBlock(route).data.find(key)->second);
        }

        template<typename T>
        T findValue(const std::string& key) const noexcept
        {
            return utility::convert<T>(findBlock("").data.find(key)->second);
        }

        const auto& getMainBlock() const noexcept {return mainBlock;}
    };

    template<>
    inline std::string Config::findValue(const std::string& key, const std::string& route) const noexcept
    {
        return findBlock(route).data.find(key)->second;
    }

    template<>
    inline std::string Config::findValue(const std::string& key) const noexcept
    {
        return findBlock("").data.find(key)->second;
    }
}

#endif
