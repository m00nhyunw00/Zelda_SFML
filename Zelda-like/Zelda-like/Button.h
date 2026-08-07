#pragma once

#include "UI.h"
#include "SFML/Graphics.hpp"

class Button : public UI
{
private:
    sf::RectangleShape body;
    sf::Text text;

    bool hovered;
    bool clicked;

public:
    void Update(float deltaTime) override;
    void Render(sf::RenderWindow& window) override;

    bool IsHovered() const;
    bool IsClicked() const;

    void SetText(...);
    void SetSize(...);
    void SetPosition(...);
};
