#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

using minecpp::Camera;

Camera::Basis Camera::Basis::create(const glm::vec3& rotation) noexcept
{
    const auto xCos = glm::cos(glm::radians(rotation.x));
    const auto xSin = glm::sin(glm::radians(rotation.x));

    const auto yCos = glm::cos(glm::radians(rotation.y));
    const auto ySin = glm::sin(glm::radians(rotation.y));

    const glm::vec3 forward{xCos * yCos, ySin, xSin * yCos};
    const glm::vec3 right{glm::cross(forward, {0.0F, 1.0F, 0.0F})};
    const glm::vec3 up   {glm::cross(right, forward)};

    return {glm::normalize(right), glm::normalize(up), glm::normalize(forward)};
}

Camera::Camera(const glm::vec3& position, const glm::vec3& rotation) noexcept :
    position{position}, rotation{rotation}, basis{Basis::create(rotation)}
{
}

glm::mat4 Camera::calculateViewMatrix() const noexcept
{
    return glm::lookAt(position, position + basis.forward, basis.up);
}

