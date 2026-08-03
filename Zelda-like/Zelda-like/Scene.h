#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Scene
{
public:
	virtual ~Scene() = default;

	virtual void HandleEvent(const sf::Event& event) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(sf::RenderWindow& window) = 0;
};

