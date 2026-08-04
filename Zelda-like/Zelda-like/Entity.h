#pragma once

#include "SFML/Graphics.hpp"

class Entity
{
protected:
    sf::Sprite* sprite;
    bool active;

public:
    Entity();
    virtual ~Entity();

    virtual void Update(
        float deltaTime,
        sf::RenderWindow& window
    ) = 0;

    virtual void Render(sf::RenderWindow& window) = 0;

    bool IsActive() const { return active; }
    void SetActive(bool value) { active = value; }
};

