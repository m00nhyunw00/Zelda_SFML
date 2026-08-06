#pragma once

#include "SFML/Graphics.hpp"

class SpriteUtil
{
public:
    // 규칙적인 배치의 Texture에서 원하는 부분의 행 번호와 열 번호를 반환하는 함수
    static sf::IntRect GetFrameRect(const sf::Texture& texture, const int& rows, const int& cols, const int& row, const int& col);

    // 원하는 Texture의 원하는 부분을 원하는 크기의 Sprite로 반환하는 함수
    static sf::Sprite CreateSprite(const sf::Texture& texture, const sf::IntRect& rect, sf::Vector2f scale);

    // 원하는 Sprite의 Origin을 Sprite의 중심으로 설정하는 함수
    static void SetSpriteOriginToCenter(sf::Sprite* sprite);
};

