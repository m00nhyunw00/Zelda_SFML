#pragma once

#include <SFML/Graphics.hpp>

class Camera
{
private:
    sf::View view;

public:
    Camera(const sf::Vector2f& center, const sf::Vector2f& size);

    void Follow(const sf::Vector2f& targetPosition);

    const sf::View& GetView() const;
};