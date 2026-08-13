#pragma once

#include "UI.h"
#include "SFML/Graphics.hpp"

class SkillCooldownUI : public UI
{
private:
    sf::RectangleShape background;

    sf::RectangleShape cooldownOverlay;

    sf::Sprite* iconSprite = nullptr;

    sf::Text* cooldownText = nullptr;

    float maxCooldown;

    float currentCooldown;

private:
    void UpdateIconState();

public:
    SkillCooldownUI(
        const sf::Font& font,
        const sf::Texture& iconTexture,
        const sf::Vector2f& size,
        const sf::Vector2f& position,
        float maxCooldown
    );

    ~SkillCooldownUI();

    void HandleEvent(const sf::Event& event,sf::RenderWindow& window) override;

    void Update(float deltaTime,sf::RenderWindow& window) override;

    void Render(sf::RenderWindow& window) override;

    void SetCooldown(float cooldown);

    void SetMaxCooldown(float cooldown);
};