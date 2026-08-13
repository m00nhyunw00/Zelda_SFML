#pragma once

#include "UI.h"
#include "SFML/Graphics.hpp"

class UltimateUI : public UI
{
private:
    sf::RectangleShape background;

    sf::Sprite* iconSprite = nullptr;

    float maxGauge;

    float currentGauge;

private:
    void UpdateIconState();

public:
    UltimateUI(
        const sf::Texture& iconTexture,
        const sf::Vector2f& size,
        const sf::Vector2f& position,
        float maxGauge
    );

    ~UltimateUI();

    void HandleEvent(const sf::Event& event, sf::RenderWindow& window) override;

    void Update(float deltaTime, sf::RenderWindow& window) override;

    void Render(sf::RenderWindow& window) override;

    void SetGauge(float gauge);

    void SetMaxGauge(float gauge);
};