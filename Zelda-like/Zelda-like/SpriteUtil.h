#pragma once

#include "SFML/Graphics.hpp"

class SpriteUtil
{
public:
    static sf::IntRect GetFrameRect(const sf::Texture& texture, int rows, int cols, int row, int col);
};

