#include "Game.h"
#include "Constants.h"
#include "ResourceManager.h"
#include "DataManager.h"
#include "SaveManager.h"
#include "SFML/Graphics.hpp"

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
    window.setKeyRepeatEnabled(false);  // KeyPressed 이벤트의 자동 반복 방지

    ResourceManager& resourceManager = ResourceManager::GetInstance();
    DataManager& dataManager = DataManager::GetInstance();

    resourceManager.LoadFont("MainFont", "Assets/Fonts/windows-bold.ttf");                      // 폰트 업로드

    resourceManager.LoadTexture("Player_Idle", "Assets/Characters/Character_Idle.png");             // 플레이어 Idle 텍스쳐 업로드
    resourceManager.LoadTexture("Player_Run", "Assets/Characters/Character_Run.png");               // 플레이어 Run 텍스쳐 업로드
    resourceManager.LoadTexture("Warrior", "Assets/Characters/Character_AttackSword1.png");         // 전사 Attack 텍스쳐 업로드
    resourceManager.LoadTexture("Archer", "Assets/Characters/Character_AttackBow1.png");            // 궁수 Attack 텍스쳐 업로드
    resourceManager.LoadTexture("Staff_3", "Assets/Icons/Staff_3.png");                             // 레벨3 지팡이 텍스쳐 업로드
    resourceManager.LoadTexture("Arrow", "Assets/Effects/Effect_Arrow.png");                        // 화살 텍스쳐 업로드
    resourceManager.LoadTexture("Fireball", "Assets/Effects/Effect_Fireball_Loop.png");             // 파이어볼 텍스쳐 업로드
    resourceManager.LoadTexture("LichFireball", "Assets/Effects/Effect_LichFireball_Loop.png");     // 리치 파이어볼 텍스쳐 업로드
    resourceManager.LoadTexture("SlimeSpike", "Assets/Effects/Effect_Slime_Spike.png");             // 자이언트 슬라임 스파이크 텍스쳐 업로드

    resourceManager.LoadTexture("Slime_Idle", "Assets/Characters/Enemy_Slime/Enemy_Slime_Idle.png");        // 슬라임 Idle 텍스쳐 업로드
    resourceManager.LoadTexture("Slime_Run", "Assets/Characters/Enemy_Slime/Enemy_Slime_Walk.png");         // 슬라임 Run 텍스쳐 업로드
    resourceManager.LoadTexture("Slime_Attack", "Assets/Characters/Enemy_Slime/Enemy_Slime_Attack.png");    // 슬라임 Attack 텍스쳐 업로드

    resourceManager.LoadTexture("Cacto_Idle", "Assets/Characters/Enemy_Cacto/Enemy_Cacto_Idle.png");        // 선인장 Idle 텍스쳐 업로드
    resourceManager.LoadTexture("Cacto_Run", "Assets/Characters/Enemy_Cacto/Enemy_Cacto_Walk.png");         // 선인장 Run 텍스쳐 업로드
    resourceManager.LoadTexture("Cacto_Attack", "Assets/Characters/Enemy_Cacto/Enemy_Cacto_Attack.png");    // 선인장 Attack 텍스쳐 업로드

    resourceManager.LoadTexture("Skeleton_Idle", "Assets/Characters/Enemy_Skel/Enemy_Skel_Idle.png");        // 스켈레톤 Idle 텍스쳐 업로드
    resourceManager.LoadTexture("Skeleton_Run", "Assets/Characters/Enemy_Skel/Enemy_Skel_Walk.png");         // 스켈레톤 Run 텍스쳐 업로드
    resourceManager.LoadTexture("Skeleton_Attack", "Assets/Characters/Enemy_Skel/Enemy_Skel_Attack.png");    // 스켈레톤 Attack 텍스쳐 업로드

    resourceManager.LoadTexture("Lich_Idle", "Assets/Characters/Enemy_Lich/Enemy_Lich_Idle.png");        // 리치 Idle 텍스쳐 업로드
    resourceManager.LoadTexture("Lich_Run", "Assets/Characters/Enemy_Lich/Enemy_Lich_Walk.png");         // 리치 Run 텍스쳐 업로드
    resourceManager.LoadTexture("Lich_Attack", "Assets/Characters/Enemy_Lich/Enemy_Lich_Attack.png");    // 리치 Attack 텍스쳐 업로드

    resourceManager.LoadTexture("GiantSlime_Idle", "Assets/Characters/Enemy_Boss_BigSlime/Enemy_Boss_BigSlime_Idle.png");        // 자이언트 슬라임 Idle 텍스쳐 업로드
    resourceManager.LoadTexture("GiantSlime_Run", "Assets/Characters/Enemy_Boss_BigSlime/Enemy_Boss_BigSlime_Walk.png");         // 자이언트 슬라임 Run 텍스쳐 업로드
    resourceManager.LoadTexture("GiantSlime_Attack", "Assets/Characters/Enemy_Boss_BigSlime/Enemy_Boss_BigSlime_Attack1.png");    // 자이언트 슬라임 Attack 텍스쳐 업로드
    resourceManager.LoadTexture("GiantSlime_Spawn", "Assets/Characters/Enemy_Boss_BigSlime/Enemy_Boss_BigSlime_Spawn.png");      // 자이언트 슬라임 Spawn 텍스쳐 업로드
    resourceManager.LoadTexture("GiantSlime_Dead", "Assets/Characters/Enemy_Boss_BigSlime/Enemy_Boss_BigSlime_Death.png");       // 자이언트 슬라임 Dead 텍스쳐 업로드
        
    resourceManager.LoadTexture("Home", "Assets/Tileset_House.png");                             // 집 타일셋 텍스쳐 업로드
    resourceManager.LoadTexture("Indoor", "Assets/Objects/Tileset_Objects_Indoors1.png");        // 실내 바닥 타일셋 텍스쳐 업로드
    resourceManager.LoadTexture("Dungeon", "Assets/Tileset_Dungeon.png");                        // 던전 타일셋 텍스쳐 업로드
    resourceManager.LoadTexture("BossRoom", "Assets/Tileset_BossRoom.png");                      // 던전 타일셋 텍스쳐 업로드

    dataManager.LoadPlayerData("Data/PlayerData.json");
    dataManager.LoadMonsterData("Data/MonsterData.json");
    dataManager.LoadPlayerLevelData("Data/PlayerLevelData.json");
    dataManager.LoadMonsterLevelData("Data/MonsterLevelData.json");
    dataManager.LoadAnimationData("Data/AnimationData.json");
    dataManager.LoadMonsterSpawnData("Data/MonsterSpawnData.json");
    
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