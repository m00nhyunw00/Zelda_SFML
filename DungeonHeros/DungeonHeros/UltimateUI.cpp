#include "UltimateUI.h"

UltimateUI::UltimateUI(
    const sf::Texture& iconTexture,
    const sf::Vector2f& size,
    const sf::Vector2f& position,
    float maxGauge)
{
    this->maxGauge = maxGauge;

    currentGauge = 0.f;


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

    background.setOutlineColor(
        sf::Color(
            120,
            120,
            120
        )
    );


    // ---------------- Ultimate Icon ----------------

    iconSprite =new sf::Sprite(iconTexture);


    const sf::Vector2u textureSize = iconTexture.getSize();


    // 사각형 테두리가 보이도록 약간 작게
    const float iconWidth = size.x - 0.f;

    const float iconHeight = size.y - 0.f;


    iconSprite->setScale(
        { 
            iconWidth / static_cast<float>(textureSize.x),
            iconHeight / static_cast<float>(textureSize.y) 
        });


    const sf::FloatRect iconBounds = iconSprite->getLocalBounds();


    iconSprite->setOrigin({
        iconBounds.position.x + iconBounds.size.x / 2.f,
        iconBounds.position.y + iconBounds.size.y / 2.f
        });


    iconSprite->setPosition(position);


    UpdateIconState();
}

UltimateUI::~UltimateUI()
{
    delete iconSprite;

    iconSprite = nullptr;
}

void UltimateUI::SetGauge(float gauge)
{
    currentGauge = gauge;

    if (currentGauge < 0.f)
    {
        currentGauge = 0.f;
    }


    if (currentGauge > maxGauge)
    {
        currentGauge = maxGauge;
    }


    UpdateIconState();
}

void UltimateUI::SetMaxGauge(float gauge)
{
    maxGauge =gauge;

    if (currentGauge > maxGauge)
    {
        currentGauge = maxGauge;
    }


    UpdateIconState();
}

void UltimateUI::UpdateIconState()
{
    if (iconSprite == nullptr)
    {
        return;
    }


    const bool isReady = maxGauge > 0.f && currentGauge >= maxGauge;


    // ---------------- 궁극기 사용 가능 ----------------

    if (isReady)
    {
        // 원래 색상으로 복귀
        iconSprite->setColor(sf::Color::White);


        // 궁극기 사용 가능 상태 강조
        background.setOutlineColor(sf::Color::Yellow);

        return;
    }


    // ---------------- 궁극기 게이지 부족 ----------------

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

void UltimateUI::HandleEvent(const sf::Event& event,sf::RenderWindow& window)
{
    (void)event;

    (void)window;
}

void UltimateUI::Update(float deltaTime,sf::RenderWindow& window)
{
    (void)deltaTime;

    (void)window;
}

void UltimateUI::Render(sf::RenderWindow& window)
{
    if (!active)
    {
        return;
    }


    window.draw(background);


    if (iconSprite != nullptr)
    {
        window.draw(*iconSprite);
    }
}