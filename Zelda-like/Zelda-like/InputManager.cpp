#include "InputManager.h"

InputManager::InputManager()
{
    spacePressed = false;
    ePressed = false;
    qPressed = false;
    iPressed = false;
    escPressed = false;
    enterPressed = false;
    num1Pressed = false;
    num2Pressed = false;
    num3Pressed = false;

    leftMouseClicked = false;
    mouseClickPosition = { 0, 0 };
}

InputManager::~InputManager()
{

}

InputManager& InputManager::GetInstance()
{
    static InputManager instance;

    return instance;
}

void InputManager::BeginFrame()
{
    spacePressed = false;
    ePressed = false;
    qPressed = false;
    iPressed = false;
    escPressed = false;
    enterPressed = false;
    num1Pressed = false;
    num2Pressed = false;
    num3Pressed = false;

    leftMouseClicked = false;
    mouseClickPosition = { 0, 0 };
}

void InputManager::HandleEvent(const sf::Event& event)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Space)
        {
            spacePressed = true;
        }
        else if (keyPressed->scancode == sf::Keyboard::Scancode::E)
        {
            ePressed = true;
        }
        else if (keyPressed->scancode == sf::Keyboard::Scancode::Q)
        {
            qPressed = true;
        }
        else if (keyPressed->scancode == sf::Keyboard::Scancode::I)
        {
            iPressed = true;
        }
        else if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
        {
            escPressed = true;
        }
        else if (keyPressed->scancode == sf::Keyboard::Scancode::Enter)
        {
            enterPressed = true;
        }
        else if (keyPressed->scancode == sf::Keyboard::Scancode::Num1)
        {
            num1Pressed = true;
        }
        else if (keyPressed->scancode == sf::Keyboard::Scancode::Num2)
        {
            num2Pressed = true;
        }
        else if (keyPressed->scancode == sf::Keyboard::Scancode::Num3)
        {
            num3Pressed = true;
        }
    }
    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mousePressed->button == sf::Mouse::Button::Left)
        {
            leftMouseClicked = true;
            mouseClickPosition = mousePressed->position;
        }
    }
}

sf::Vector2f InputManager::GetMoveDirection()
{
    sf::Vector2f direction(0.f, 0.f);

    // 위쪽 이동
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
    {
        direction.y -= 1.f;
    }

    // 아래쪽 이동
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
    {
        direction.y += 1.f;
    }

    // 왼쪽 이동
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        direction.x -= 1.f;
    }

    // 오른쪽 이동
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        direction.x += 1.f;
    }

    // 대각선 이동 속도 보정
    float length = std::sqrt(
        direction.x * direction.x +
        direction.y * direction.y
    );

    if (length > 0.f)
    {
        direction.x /= length;
        direction.y /= length;
    }

    return direction;
}

sf::Vector2i InputManager::GetMousePosition(sf::RenderWindow& window)
{
    return sf::Mouse::getPosition(window);
}


sf::Vector2i InputManager::GetMouseClickPosition()
{
    return mouseClickPosition;
}

sf::Vector2f InputManager::GetMouseClickWorldPosition(const sf::RenderWindow& window)
{
    return window.mapPixelToCoords(mouseClickPosition);
}

sf::Vector2f InputManager::GetMouseWorldPosition(
    const sf::RenderWindow& window)
{
    sf::Vector2i pixelPosition =
        sf::Mouse::getPosition(window);

    return window.mapPixelToCoords(pixelPosition);
}
