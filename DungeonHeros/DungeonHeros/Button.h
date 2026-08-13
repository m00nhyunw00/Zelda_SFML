#pragma once

#include "UI.h"
#include <string>

class Button : public UI
{
private:
    sf::RectangleShape body;
    sf::Text text;

    bool hovered;
    bool clicked;

public:
    Button(
        const sf::Font& font,
        const std::string& text,
        const sf::Vector2f& size,
        const sf::Vector2f& position
    );

    void HandleEvent(const sf::Event& event,sf::RenderWindow& window) override;

    void Update(float deltaTime, sf::RenderWindow& window) override; 

    void Render(sf::RenderWindow& window) override;

    // Getter
    bool IsHovered() const { return hovered; }

    bool IsClicked() const { return clicked; }

    // Setter
    void SetText(const std::string& text);

    void SetSize(const sf::Vector2f& size);

    void SetPosition(const sf::Vector2f& position);

    void SetFillColor(const sf::Color& color);

    void SetOutlineColor(const sf::Color& color);

    void SetOutlineThickness(float thickness);
};