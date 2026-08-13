#include "SkillCooldownUI.h"

#include <cmath>
#include <string>

SkillCooldownUI::SkillCooldownUI(
    const sf::Font& font,
    const sf::Texture& iconTexture,
    const sf::Vector2f& size,
    const sf::Vector2f& position,
    float maxCooldown)
{
    this->maxCooldown = maxCooldown;

    currentCooldown = 0.f;


    // ---------------- 배경 ----------------

    background.setSize(size);

    background.setOrigin({
        size.x / 2.f,
        size.y / 2.f
        });

    background.setPosition(position);

    background.setFillColor(
        sf::Color(
            30,
            30,
            30
        )
    );

    background.setOutlineThickness(2.f);

    background.setOutlineColor(sf::Color::White);


    // ---------------- Skill Icon ----------------

    iconSprite = new sf::Sprite(iconTexture);

    const sf::Vector2u textureSize = iconTexture.getSize();


    // 테두리가 보이도록 아이콘을 사각형보다 조금 작게 설정
    const float iconWidth = size.x - 0.f;

    const float iconHeight = size.y - 0.f;


    iconSprite->setScale({
        iconWidth / static_cast<float>(textureSize.x),
        iconHeight / static_cast<float>(textureSize.y)
        });


    const sf::FloatRect iconBounds = iconSprite->getLocalBounds();


    iconSprite->setOrigin({
        iconBounds.position.x + iconBounds.size.x / 2.f,

        iconBounds.position.y + iconBounds.size.y / 2.f
        });


    iconSprite->setPosition(position
    );


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
        sf::Color(
            0,
            0,
            0,
            170
        )
    );


    // ---------------- 남은 시간 Text ----------------

    cooldownText = new sf::Text(font);

    cooldownText->setCharacterSize(24);

    cooldownText->setFillColor(sf::Color::White);

    cooldownText->setPosition(position);


    UpdateIconState();
}

SkillCooldownUI::~SkillCooldownUI()
{
    delete iconSprite;
    iconSprite = nullptr;

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


    // 아이콘 활성 / 비활성 상태 갱신
    UpdateIconState();


    if (maxCooldown <= 0.f)
    {
        cooldownOverlay.setSize({
            background.getSize().x,
            0.f
            });

        cooldownText->setString("");

        return;
    }


    const float ratio = currentCooldown / maxCooldown;


    const sf::Vector2f size = background.getSize();


    // 남아있는 쿨타임 비율만큼 검은 Overlay 표시
    cooldownOverlay.setSize({
        size.x,
        size.y * ratio
        });


    const sf::Vector2f backgroundPosition = background.getPosition();


    // 쿨타임이 감소할수록 위에서 아래 방향으로 Overlay가 줄어듦
    cooldownOverlay.setPosition({
        backgroundPosition.x - size.x / 2.f,
        backgroundPosition.y + size.y / 2.f - size.y * ratio
        });


    // ---------------- 남은 시간 Text ----------------

    if (currentCooldown > 0.f)
    {
        cooldownText->setString(std::to_string(static_cast<int>(std::ceil(currentCooldown))));


        const sf::FloatRect bounds =cooldownText->getLocalBounds();


        cooldownText->setOrigin({
            bounds.position.x +
                bounds.size.x / 2.f,

            bounds.position.y +
                bounds.size.y / 2.f
            });


        cooldownText->setPosition(background.getPosition());
    }
    else
    {
        cooldownText->setString("");
    }
}

void SkillCooldownUI::UpdateIconState()
{
    if (iconSprite == nullptr)
    {
        return;
    }


    // ---------------- 사용 가능 ----------------

    if (currentCooldown <= 0.f)
    {
        // 원래 색상 그대로 표시
        iconSprite->setColor(sf::Color::White);

        background.setOutlineColor(sf::Color::White);

        return;
    }


    // ---------------- 쿨타임 중 ----------------

    // 흑백에 가까운 비활성화 느낌
    iconSprite->setColor(
        sf::Color(
            100,
            100,
            100
        )
    );

    background.setOutlineColor(
        sf::Color(
            120,
            120,
            120
        )
    );
}

void SkillCooldownUI::HandleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    (void)event;

    (void)window;
}

void SkillCooldownUI::Update(float deltaTime, sf::RenderWindow& window)
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


    // 배경
    window.draw(background);


    // 아이콘
    if (iconSprite != nullptr)
    {
        window.draw(*iconSprite);
    }


    // 쿨타임 중에만 Overlay + 숫자 표시
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


    if (currentCooldown > maxCooldown)
    {
        currentCooldown = maxCooldown;
    }


    UpdateIconState();
}