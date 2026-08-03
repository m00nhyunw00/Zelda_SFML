#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Scene
{
public:
	virtual ~Scene() = default;

	virtual void HandleEvent(const sf::Event& event) = 0;	// 사용자 입력 처리 함수
	virtual void Update(float deltaTime) = 0;				// 게임 로직 계산 함수
	virtual void Render(sf::RenderWindow& window) = 0;		// 렌더링 함수
};

