#pragma once

#include <SFML/Graphics.hpp>

class Collider
{
private:
    // 실제 충돌 영역을 나타내는 사각형
    sf::RectangleShape body;

    // 소유 객체의 position으로부터 Collider를 얼마나 이동할지
    sf::Vector2f offset;

public:
    Collider();

    Collider(
        const sf::Vector2f& size,
        const sf::Vector2f& offset = { 0.f, 0.f }
    );

    // 충돌 영역의 크기 설정
    void SetSize(const sf::Vector2f& size);

    // 소유 객체 위치로부터의 상대 위치 설정
    void SetOffset(const sf::Vector2f& offset);

    // Collider의 실제 위치 갱신
    void SetPosition(const sf::Vector2f& position);

    // Creature 등의 중심 위치를 기준으로 Collider 위치 갱신
    void UpdatePosition(const sf::Vector2f& ownerPosition);

    // 현재 충돌 영역 반환
    sf::FloatRect GetBounds() const;

    // 다른 Collider와 충돌했는지 검사
    bool Collision(const Collider& other) const;

    // FloatRect와 충돌했는지 검사
    bool Collision(const sf::FloatRect& otherBounds) const;

    // 충돌 박스 확인을 위한 디버그 렌더링
    void Draw(sf::RenderWindow& window) const;
};