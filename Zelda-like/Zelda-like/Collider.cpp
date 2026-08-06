#include "Collider.h"

Collider::Collider()
    : offset({ 0.f, 0.f })
{
    body.setSize({ 0.f, 0.f });

    body.setFillColor(sf::Color::Transparent);
    body.setOutlineColor(sf::Color::Red);
    body.setOutlineThickness(1.f);
}

Collider::Collider(
    const sf::Vector2f& size,
    const sf::Vector2f& offset)
    : offset(offset)
{
    body.setSize(size);

    body.setOrigin({
        size.x / 2.f,
        size.y / 2.f
        });

    body.setFillColor(sf::Color::Transparent);
    body.setOutlineColor(sf::Color::Red);
    body.setOutlineThickness(1.f);
}

void Collider::SetSize(
    const sf::Vector2f& size)
{
    body.setSize(size);

    body.setOrigin({
        size.x / 2.f,
        size.y / 2.f
        });
}

void Collider::SetOffset(const sf::Vector2f& offset)
{
    this->offset = offset;
}

void Collider::SetPosition(
    const sf::Vector2f& position)
{
    body.setPosition(position);
}

void Collider::UpdatePosition(const sf::Vector2f& ownerPosition)
{
    const sf::Vector2f size = body.getSize();

    body.setPosition({
        ownerPosition.x + offset.x,
        ownerPosition.y + offset.y
        });
}

sf::FloatRect Collider::GetBounds() const
{
    return body.getGlobalBounds();
}

bool Collider::Collision(const Collider& other) const
{
    return body.getGlobalBounds().findIntersection(other.GetBounds()).has_value();
}

bool Collider::Collision(const sf::FloatRect& otherBounds) const
{
    return body.getGlobalBounds().findIntersection(otherBounds).has_value();
}

void Collider::Draw(sf::RenderWindow& window) const
{
    window.draw(body);
}