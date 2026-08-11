#pragma once

#include <SFML/Graphics.hpp>
#include "AnimationState.h"

class InputManager
{
private:
    bool spacePressed;
    bool ePressed;
    bool qPressed;
    bool iPressed;
    bool fPressed;
    bool escPressed;
    bool enterPressed;
    bool num1Pressed;
    bool num2Pressed;
    bool num3Pressed;

    bool leftMouseClicked;
    sf::Vector2i mouseClickPosition;    // 마우스를 클릭한 위치
    Direction lastPressedDirection;     // 마지막으로 보고 있던 방향

private:
    InputManager();
    ~InputManager();

public:
    static InputManager& GetInstance();

    void BeginFrame();
    void HandleEvent(const sf::Event& event);

    sf::Vector2f GetMoveDirection();

    bool IsSpacePressed() { return spacePressed; }
    bool IsEPressed() { return ePressed; }
    bool IsQPressed() { return qPressed; }
    bool IsIPressed() { return iPressed; }
    bool IsFPressed() { return fPressed; }
    bool IsEscPressed() { return escPressed; }
    bool IsEnterPressed() { return enterPressed; }
    bool IsNum1Pressed() { return num1Pressed; }
    bool IsNum2Pressed() { return num2Pressed; }
    bool IsNum3Pressed() { return num3Pressed; }
    bool IsLeftMouseClicked() { return leftMouseClicked; }
    Direction GetLastPressedDirection() const { return lastPressedDirection; }
    sf::Vector2i GetMouseClickPosition();
    sf::Vector2f GetMouseClickWorldPosition(const sf::RenderWindow& window);
    sf::Vector2i GetMousePosition(sf::RenderWindow& window);
    sf::Vector2f GetMouseWorldPosition(const sf::RenderWindow& window);
    sf::Vector2f GetMouseUIPosition(const sf::RenderWindow& window);
};