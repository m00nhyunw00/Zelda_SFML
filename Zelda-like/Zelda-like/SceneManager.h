#pragma once

#include <SFML/Graphics.hpp>

#include "Scene.h"
#include "SceneType.h"

class SceneManager
{
private:
    Scene* currentScene;                  // 현재 실행 중인 Scene 객체

    SceneType nextSceneType;              // 다음에 변경할 Scene 종류
    bool sceneChangeRequested;            // Scene 변경 요청 여부

private:
    Scene* CreateScene(SceneType type);   // Scene 종류에 맞는 객체 생성

public:
    SceneManager();
    ~SceneManager();

    void Start(SceneType firstScene);           // 게임 시작 시 첫 Scene 생성

    void RequestSceneChange(SceneType type);    // Scene 변경 요청
    void ApplySceneChange();                    // 요청된 Scene으로 실제 변경

    void HandleEvent(const sf::Event& event);   // 현재 Scene의 입력 처리
    void Update(float deltaTime);               // 현재 Scene의 게임 로직 업데이트
    void Render(sf::RenderWindow& window);      // 현재 Scene 화면 출력
};