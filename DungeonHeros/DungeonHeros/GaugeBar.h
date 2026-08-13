#pragma once

#include "UI.h"
#include "SFML/Graphics.hpp"

class GaugeBar : public UI
{
private:
    sf::RectangleShape background;
    sf::RectangleShape gauge;

    float maxValue;
    float currentValue;

    float maxWidth;

public:
    GaugeBar(
        const sf::Vector2f& size,
        const sf::Vector2f& position,
        float maxValue
    );

    void HandleEvent(const sf::Event& event, sf::RenderWindow& window) override;

    void Update(float deltaTime,sf::RenderWindow& window) override;

    void Render(sf::RenderWindow& window) override;

    void SetValue(float value);
    void SetMaxValue(float value);

    void SetPosition(const sf::Vector2f& position);

    void SetGaugeColor(const sf::Color& color);

    void SetBackgroundColor(const sf::Color& color);
};