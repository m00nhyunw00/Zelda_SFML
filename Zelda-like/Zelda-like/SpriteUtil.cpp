#include "SpriteUtil.h"

sf::IntRect SpriteUtil::GetFrameRect(const sf::Texture& texture, const int& rows, const int& cols, const int& row, const int& col)
{
    sf::Vector2u size = texture.getSize();

    int width = size.x / cols;
    int height = size.y / rows;

    return sf::IntRect(
        { col * width, row * height },
        { width, height }
    );
}
sf::Sprite SpriteUtil::CreateSprite(const sf::Texture& texture, const sf::IntRect& rect, sf::Vector2f scale)
{
    sf::Sprite sprite(texture);

    sprite.setTextureRect(rect);
    sprite.setScale(scale);

    return sprite;
}

void SpriteUtil::SetSpriteOriginToCenter(sf::Sprite* sprite)
{
    sf::FloatRect bounds = sprite->getLocalBounds();

    sprite->setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
        });
}
