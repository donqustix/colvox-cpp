#ifndef PLAYER_H
#define PLAYER_H

#include <glm/vec3.hpp>

#include <memory>

namespace minecpp
{
    class Camera;

    class Player
    {
        std::unique_ptr<Camera> camera;

    public:
        explicit Player(const glm::vec3& position) noexcept;
        Player(Player&&) noexcept;
        ~Player();

        Player& operator=(Player&&) noexcept;

        void update(unsigned delta) noexcept;

        const auto& getCamera() const noexcept {return *camera;}
    };
}

#endif
