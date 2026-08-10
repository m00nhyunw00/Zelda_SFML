#include "SkillCooldownUI.h"

#include <cmath>

SkillCooldownUI::SkillCooldownUI(
    const sf::Font& font,
    const sf::Vector2f& size,
    const sf::Vector2f& position,
    float maxCooldown)
{
    this->maxCooldown = maxCooldown;
    this->currentCooldown = 0.f;

    // ---------------- 배경 ----------------

    background.setSize(size);

    background.setOrigin({
        size.x / 2.f,
        size.y / 2.f
        });

    background.setPosition(position);

    background.setFillColor(sf::Color(70, 70, 70));

    background.setOutlineThickness(2.f);
    background.setOutlineColor(sf::Color::White);


    // ---------------- 쿨타임 Overlay ----------------

    cooldownOverlay.setSize({
        size.x,
        0.f
        });

    cooldownOverlay.setPosition({
        position.x - size.x / 2.f,
        position.y + size.y / 2.f
        });

    cooldownOverlay.setFillColor(
        sf::Color(0, 0, 0, 170)
    );


    // ---------------- 남은 시간 Text ----------------

    cooldownText = new sf::Text(font);

    cooldownText->setCharacterSize(24);
    cooldownText->setFillColor(sf::Color::White);

    cooldownText->setPosition(position);
}

SkillCooldownUI::~SkillCooldownUI()
{
    delete cooldownText;
    cooldownText = nullptr;
}

void SkillCooldownUI::SetCooldown(float cooldown)
{
    currentCooldown = cooldown;

    if (currentCooldown < 0.f)
    {
        currentCooldown = 0.f;
    }

    if (currentCooldown > maxCooldown)
    {
        currentCooldown = maxCooldown;
    }

    if (maxCooldown <= 0.f)
    {
        return;
    }

    const float ratio = currentCooldown / maxCooldown;

    const sf::Vector2f size = background.getSize();

    // 남은 쿨타임 비율만큼 검은색 영역 표시
    cooldownOverlay.setSize({
        size.x,
        size.y * ratio
        });

    const sf::Vector2f backgroundPosition = background.getPosition();

    // 아래에서 위로 차도록
    cooldownOverlay.setPosition({
        backgroundPosition.x - size.x / 2.f,
        backgroundPosition.y + size.y / 2.f -
            size.y * ratio
        });

    if (currentCooldown > 0.f)
    {
        cooldownText->setString(
            std::to_string(
                static_cast<int>(
                    std::ceil(currentCooldown)
                    )
            )
        );

        const sf::FloatRect bounds =cooldownText->getLocalBounds();

        cooldownText->setOrigin(
            {
                bounds.position.x + bounds.size.x / 2.f,
                bounds.position.y + bounds.size.y / 2.f
            });

        cooldownText->setPosition(background.getPosition());
    }
    else
    {
        cooldownText->setString("");
    }
}

void SkillCooldownUI::HandleEvent(
    const sf::Event& event,
    sf::RenderWindow& window)
{
    (void)event;
    (void)window;
}

void SkillCooldownUI::Update(
    float deltaTime,
    sf::RenderWindow& window)
{
    (void)deltaTime;
    (void)window;
}

void SkillCooldownUI::Render(
    sf::RenderWindow& window)
{
    if (!active)
    {
        return;
    }

    window.draw(background);

    if (currentCooldown > 0.f)
    {
        window.draw(cooldownOverlay);

        if (cooldownText != nullptr)
        {
            window.draw(*cooldownText);
        }
    }
}

void SkillCooldownUI::SetMaxCooldown(float cooldown)
{
    maxCooldown = cooldown;
}