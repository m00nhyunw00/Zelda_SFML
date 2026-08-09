#pragma once

#include "SFML/Graphics.hpp"

class UI
{
protected:
    bool active = true;

public:
    virtual ~UI() = default;

    virtual void HandleEvent(const sf::Event& event, sf::RenderWindow& window) = 0;

    virtual void Update(float deltaTime, sf::RenderWindow& window) = 0;

    virtual void Render(sf::RenderWindow& window) = 0;

    bool IsActive() const { return active; }

    void SetActive(bool active) { this->active = active; }
};