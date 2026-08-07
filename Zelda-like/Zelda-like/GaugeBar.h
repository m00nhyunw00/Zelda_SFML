#pragma once

#include "UI.h"
#include "SFML/Graphics.hpp"

class GaugeBar : public UI
{
private:
    sf::RectangleShape background;
    sf::RectangleShape fill;

    float maxValue;
    float currentValue;

public:
    void Update(float deltaTime) override;
    void Render(sf::RenderWindow& window) override;

    void SetValue(float value);
    void SetMaxValue(float value);
};
