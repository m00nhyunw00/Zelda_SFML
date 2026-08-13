#include "Button.h"

#include "Button.h"
#include "InputManager.h"

Button::Button(
    const sf::Font& font,
    const std::string& textString,
    const sf::Vector2f& size,
    const sf::Vector2f& position
)
    : text(font)
{
    hovered = false;
    clicked = false;

    // ---------------- Body ----------------

    SetSize(size);

    SetPosition(position);
    SetFillColor(sf::Color(60, 60, 60));
    SetOutlineThickness(3.f);
    SetOutlineColor(sf::Color::White);

    // ---------------- Text ----------------

    SetText(textString);

}

void Button::HandleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    (void)event;

    if (!active)
    {
        clicked = false;
        return;
    }

    clicked = false;

    InputManager& input = InputManager::GetInstance();

    if (!input.IsLeftMouseClicked())
    {
        return;
    }

    const sf::Vector2f mousePosition = input.GetMouseUIPosition(window);

    if (body.getGlobalBounds().contains(mousePosition))
    {
        clicked = true;
    }
}

void Button::Update(float deltaTime, sf::RenderWindow& window)
{
    //(void)deltaTime;

    if (!active)
    {
        hovered = false;
        clicked = false;

        return;
    }

    InputManager& input = InputManager::GetInstance();

    const sf::Vector2f mousePosition = input.GetMouseUIPosition(window);

    hovered = body.getGlobalBounds().contains(mousePosition);

    clicked = hovered && input.IsLeftMouseClicked();

    // Hover È¿°ú
    if (hovered)
    {
        body.setOutlineColor(
            sf::Color::Magenta
        );
    }
    else
    {
        body.setOutlineColor(
            sf::Color::White
        );
    }
}

void Button::Render(sf::RenderWindow& window)
{
    if (!active)
    {
        return;
    }

    window.draw(body);
    window.draw(text);
}

void Button::SetText(const std::string& textString)
{
    text.setString(textString);

    sf::FloatRect bounds = text.getLocalBounds();

    text.setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
        });

    text.setPosition(
        body.getPosition()
    );
}

void Button::SetSize(const sf::Vector2f& size)
{
    body.setSize(size);

    body.setOrigin({
        size.x / 2.f,
        size.y / 2.f
        });
}

void Button::SetPosition(const sf::Vector2f& position)
{
    body.setPosition(position);
    text.setPosition(position);
}

void Button::SetFillColor(const sf::Color& color)
{
    body.setFillColor(color);
}

void Button::SetOutlineColor(const sf::Color& color)
{
    body.setOutlineColor(color);
}

void Button::SetOutlineThickness(float thickness)
{
    body.setOutlineThickness(thickness);
}