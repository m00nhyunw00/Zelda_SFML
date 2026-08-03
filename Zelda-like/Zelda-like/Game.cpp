#include "Game.h"

Game::Game()
    : window(
        sf::VideoMode({ 1280, 720 }),
        "Zelda-like RPG"
    )
{
    sceneManager.Start(TITLE);   // 게임 시작 시 첫 Scene을 Title로 설정
}

void Game::Run()
{
    sf::Clock clock;             

    while (window.isOpen())      // 창이 열려있는 동안 게임 실행
    {
        float deltaTime = clock.restart().asSeconds();  // 이전 프레임부터 현재 프레임까지 걸린 시간

        // 발생한 모든 이벤트 처리
        while (const auto event = window.pollEvent())
        {
            // 창 닫기 버튼 클릭 시 게임 종료
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            // 현재 Scene에게 이벤트 전달
            sceneManager.HandleEvent(*event);
        }

        // 현재 Scene의 게임 로직 업데이트
        sceneManager.Update(deltaTime);

        // RequestSceneChange()에 의해서 sceneChangeRequested == true일 경우, Scene 변경
        // sceneChangeRequested == false일 경우, 변화 없이 바로 return
        sceneManager.ApplySceneChange();

        // 이전 프레임 화면 지우기
        window.clear(sf::Color::Black);

        // 현재 Scene 화면 출력
        sceneManager.Render(window);

        // 그린 내용을 실제 화면에 표시
        window.display();
    }
}