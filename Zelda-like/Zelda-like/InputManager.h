#pragma once

#include <SFML/Graphics.hpp>

class InputManager
{
private:
    bool spacePressed;
    bool ePressed;
    bool qPressed;
    bool iPressed;
    bool escPressed;
    bool enterPressed;
    bool num1Pressed;
    bool num2Pressed;
    bool num3Pressed;

    bool leftMouseClicked;
    sf::Vector2i mouseClickPosition;

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
    bool IsEscPressed() { return escPressed; }
    bool IsEnterPressed() { return enterPressed; }
    bool IsNum1Pressed() { return num1Pressed; }
    bool IsNum2Pressed() { return num2Pressed; }
    bool IsNum3Pressed() { return num3Pressed; }
    bool IsLeftMouseClicked() { return leftMouseClicked; }

    sf::Vector2i GetMouseClickPosition();
    sf::Vector2f GetMouseClickWorldPosition(const sf::RenderWindow& window);
    sf::Vector2i GetMousePosition(sf::RenderWindow& window);
    sf::Vector2f GetMouseWorldPosition(const sf::RenderWindow& window);
};