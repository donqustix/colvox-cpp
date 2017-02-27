#ifndef CAMERA_H
#define CAMERA_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace minecpp
{
    class Camera
    {
    public:
        struct Basis
        {
            glm::vec3 right;
            glm::vec3 up;
            glm::vec3 forward;

            static Basis create(const glm::vec3& rotation) noexcept;
        };

    private:
        glm::vec3 position;
        glm::vec3 rotation;
        Basis basis;

    public:
        Camera(const glm::vec3& position, const glm::vec3& rotation) noexcept;

        void updateBasis() noexcept {basis = Basis::create(rotation);}

        glm::mat4 calculateViewMatrix() const noexcept;

        auto& getPosition() noexcept {return position;}
        auto& getRotation() noexcept {return rotation;}

        const auto& getPosition() const noexcept {return position;}
        const auto& getRotation() const noexcept {return rotation;}

        const auto& getBasis() const noexcept {return basis;}
    };
}

#endif
