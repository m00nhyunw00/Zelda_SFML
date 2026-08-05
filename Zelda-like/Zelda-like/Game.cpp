#include "Game.h"
#include "Constants.h"
#include "ResourceManager.h"
#include "DataManager.h"

Game::Game()
    : window(
        sf::VideoMode({
            Constants::WINDOW_WIDTH,
            Constants::WINDOW_HEIGHT
            }),
        "Zelda-like RPG"
    ),
    entityManager(),
    sceneManager(&entityManager)
{
    ResourceManager& resourceManager = ResourceManager::GetInstance();

    resourceManager.LoadFont("MainFont", "Assets/Fonts/windows-bold.ttf");                      // 폰트 업로드

    resourceManager.LoadTexture("Player_Idle", "Assets/Characters/Character_Idle.png");         // Idle 텍스쳐 업로드
    resourceManager.LoadTexture("Player_Run", "Assets/Characters/Character_Run.png");           // Run 텍스쳐 업로드
    resourceManager.LoadTexture("Warrior", "Assets/Characters/Character_AttackSword1.png");     // 전사 공격 텍스쳐 업로드
    resourceManager.LoadTexture("Archer", "Assets/Characters/Character_AttackBow1.png");        // 궁수 공격 텍스쳐 업로드
    resourceManager.LoadTexture("Staff_3", "Assets/Icons/Staff_3.png");                         // 레벨3 지팡이 텍스쳐 업로드

    DataManager::GetInstance().LoadPlayerData("Data/PlayerData.json");
    DataManager::GetInstance().LoadAnimationData("Data/AnimationData.json");

    sceneManager.Start(TITLE);
}

void Game::Run()
{
    sf::Clock clock;     

    InputManager& input = InputManager::GetInstance();

    while (window.isOpen())      // 창이 열려있는 동안 게임 실행
    {
        float deltaTime = clock.restart().asSeconds();  // 이전 프레임부터 현재 프레임까지 걸린 시간

        input.BeginFrame();

        // 발생한 모든 이벤트 처리
        while (const auto event = window.pollEvent())
        {
            // 창 닫기 버튼 클릭 시 게임 종료
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            input.HandleEvent(*event);
            sceneManager.HandleEvent(*event, window);
        }

        // 현재 Scene의 게임 로직 업데이트
        sceneManager.Update(deltaTime, window);

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