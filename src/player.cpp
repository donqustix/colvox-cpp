#include "player.h"
#include "camera.h"
#include "constants.h"

#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>

#include <glm/common.hpp>

using minecpp::Player;

Player::Player(const glm::vec3& position) noexcept :
    camera{std::make_unique<Camera>(position, glm::vec3{90.0F, 0.0F, 0.0F})}
{
}

Player::Player(Player&&) noexcept = default;

Player::~Player() = default;

Player& Player::operator=(Player&&) noexcept = default;

void Player::update(unsigned delta) noexcept
{
    const float movementSpeed = constants::PLAYER_MOVEMENT_SPEED * delta;
    const Camera::Basis& basis = camera->getBasis();

    glm::vec3& position = camera->getPosition();

    const Uint8* const keyboard = ::SDL_GetKeyboardState(nullptr);
    if (keyboard[SDL_SCANCODE_W])
        position += basis.forward * movementSpeed;
    else if (keyboard[SDL_SCANCODE_S])
        position -= basis.forward * movementSpeed;
    if (keyboard[SDL_SCANCODE_A])
        position -= basis.right * movementSpeed;
    else if (keyboard[SDL_SCANCODE_D])
        position += basis.right * movementSpeed;

    int mrx, mry;
    ::SDL_GetRelativeMouseState(&mrx, &mry);

    glm::vec3& rotation = camera->getRotation() += glm::vec3{mrx, -mry, 0.0F} * constants::PLAYER_ROTATION_SPEED;

    rotation.x = glm::mod(rotation.x, 360.0F);

    if (rotation.y > 89.0F)
        rotation.y = 89.0F;
    else if (rotation.y < -89.0F)
             rotation.y = -89.0F;

    camera->updateBasis();
}

