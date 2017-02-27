#include "resource_loader.h"
#include "config.h"
#include "resource_container.h"
#include "shader_program.h"

using minecpp::ResourceLoader;

ResourceLoader::ResourceLoader(ResourceContainer& resourceContainer) noexcept :
    resourceContainer{&resourceContainer}
{
}

void ResourceLoader::load(const Config& config)
{
    for (const auto& p : config.getMainBlock().blocks)
    {
        if (p.first == "shaders")
        {
            for (const auto& sp : p.second.blocks)
            {
                const auto shaderProgram = resourceContainer->load<ShaderProgram>(sp.first + "_shader");
                shaderProgram->loadShader(utility::readFile(sp.second.data.find("vs")->second), GL_VERTEX_SHADER);
                shaderProgram->loadShader(utility::readFile(sp.second.data.find("fs")->second), GL_FRAGMENT_SHADER);
                shaderProgram->link();
            }
        }
    }
}

