#include "SpriteUtil.h"

sf::IntRect SpriteUtil::GetFrameRect(const sf::Texture& texture, int rows, int cols, int row, int col)
{
    sf::Vector2u size = texture.getSize();

    int width = size.x / cols;
    int height = size.y / rows;

    return sf::IntRect(
        { col * width, row * height },
        { width, height }
    );
}