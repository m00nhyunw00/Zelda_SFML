#include "Camera.h"

Camera::Camera(
    const sf::Vector2f& center,
    const sf::Vector2f& size)
{
    view.setCenter(center);
    view.setSize(size);
}

void Camera::Follow(const sf::Vector2f& targetPosition)
{
    view.setCenter(targetPosition);
}

const sf::View& Camera::GetView() const
{
    return view;
}