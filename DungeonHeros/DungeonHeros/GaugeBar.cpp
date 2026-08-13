#include "GaugeBar.h"

GaugeBar::GaugeBar(
    const sf::Vector2f& size,
    const sf::Vector2f& position,
    float maxValue)
{
    this->maxValue = maxValue;
    this->currentValue = maxValue;

    maxWidth = size.x;

    // 배경
    background.setSize(size);
    background.setOrigin({
        size.x / 2.f,
        size.y / 2.f
        });
    background.setPosition(position);

    background.setFillColor(
        sf::Color(50, 50, 50)
    );

    background.setOutlineThickness(2.f);

    background.setOutlineColor(
        sf::Color::White
    );

    // 실제 게이지
    gauge.setSize(size);
    gauge.setOrigin({
        size.x / 2.f,
        size.y / 2.f
        });
    gauge.setPosition(position);

    gauge.setFillColor(
        sf::Color::Green
    );
}

void GaugeBar::HandleEvent(const sf::Event& event, sf::RenderWindow& window)
{

}

void GaugeBar::Update(
    float deltaTime,
    sf::RenderWindow& window)
{
    (void)deltaTime;
    (void)window;
}

void GaugeBar::SetValue(float value)
{
    currentValue = value;

    if (currentValue < 0.f)
    {
        currentValue = 0.f;
    }

    if (currentValue > maxValue)
    {
        currentValue = maxValue;
    }

    if (maxValue <= 0.f)
    {
        return;
    }

    const float ratio = currentValue / maxValue;

    gauge.setSize({
        maxWidth * ratio,
        gauge.getSize().y
        });
}

void GaugeBar::SetMaxValue(float value)
{
    maxValue = value;

    SetValue(currentValue);
}

void GaugeBar::SetPosition(const sf::Vector2f& position)
{
    background.setPosition(position);
    gauge.setPosition(position);
}

void GaugeBar::SetGaugeColor(const sf::Color& color)
{
    gauge.setFillColor(color);
}

void GaugeBar::SetBackgroundColor(const sf::Color& color)
{
    background.setFillColor(color);
}

void GaugeBar::Render(sf::RenderWindow& window)
{
    if (!active)
    {
        return;
    }

    window.draw(background);
    window.draw(gauge);
}