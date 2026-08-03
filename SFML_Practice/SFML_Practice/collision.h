#include <SFML/Graphics.hpp>  // SFML 그래픽 기능 (창, 도형, 색상 등)
#include <iostream>           // 콘솔 출력용
#include <vector>             // vector 컨테이너 사용
#include <random>             // C++ 난수 생성기 사용

using namespace sf;
using namespace std;

// 플레이어 이동 속도
constexpr float MOVE_SPEED = 5.f;

// 생성할 적의 최대 개수
constexpr int MAX_ENEMY_COUNT = 20;

//--------------------------------------------------
// 랜덤 클래스
//--------------------------------------------------
// 기존 rand() 대신 <random> 기반 난수를 사용하는 유틸리티 클래스
// 더 현대적이고, 범위 지정이 직관적이며 품질도 더 좋음
class Random
{
public:
    // min ~ max 범위의 실수 난수를 반환
    static float Range(float min, float max)
    {
        // random_device: 시드(seed) 생성용
        static random_device rd;

        //rand()는 간단하지만 오래된 방식
        //mt19937는 품질이 좋고 예측 가능한 방식으로 제어 가능
// mt19937: 메르센 트위스터 알고리즘
        static mt19937 gen(rd());

        // 균등 분포 실수 난수 생성기
        uniform_real_distribution<float> dist(min, max);

        // 난수 반환
        return dist(gen);
    }
};

//--------------------------------------------------
// 적 클래스
//--------------------------------------------------
// 적을 하나의 객체로 묶어서 관리
// 이전 코드처럼 RectangleShape만 따로 관리하지 않고,
// "적의 모양 + 속도 + 동작"을 하나의 클래스로 캡슐화함
class Enemy
{
private:
    RectangleShape body; // 적의 외형(사각형)
    float speed;         // 적이 아래로 내려오는 속도

public:
    // 적 생성자
    Enemy()
    {
        // 적 크기를 랜덤하게 설정
        body.setSize({ Random::Range(10.f, 25.f), Random::Range(10.f, 25.f) });

        // 적 외곽선 색상을 빨간색으로 설정
        body.setOutlineColor(Color::Red);

        // 외곽선 두께 설정
        body.setOutlineThickness(2);

        // 적의 시작 위치를 화면 상단 쪽 랜덤 x 위치로 설정
        // 창 너비가 400이므로 대략 0~380 사이에서 생성
        body.setPosition({ Random::Range(0.f, 380.f), 0.f });

        // 적마다 서로 다른 낙하 속도를 갖도록 랜덤 지정
        speed = Random::Range(50.f, 150.f);
    }

    // 적 위치 업데이트
    // deltaTime을 곱해서 프레임 수와 관계없이 일정한 속도로 이동하도록 함
    void Update(float deltaTime)
    {
        // y축 아래 방향으로 이동
        body.move({ 0.f, speed * deltaTime });

        // 화면 아래로 벗어나면 다시 위쪽으로 올려서 재사용
        if (body.getPosition().y > 500)
        {
            body.setPosition({ Random::Range(0.f, 380.f), 0.f });
        }
    }

    // 적 그리기
    void Draw(RenderWindow& window) const
    {
        window.draw(body);
    }

    // 플레이어와 충돌했는지 검사
    bool Collision(const FloatRect& playerBounds) const
    {
        // getGlobalBounds() : 현재 위치/크기를 반영한 사각형 영역 반환
        // findIntersection(...) : 두 사각형이 겹치면 교차 영역 반환
        // has_value() : 실제로 겹쳤는지 여부 확인
        return body.getGlobalBounds().findIntersection(playerBounds).has_value();
    }
};

//--------------------------------------------------
// 메인
//--------------------------------------------------
int main()
{
    //----------------------------------------
    // 플레이어 생성
    //----------------------------------------
    // 플레이어를 15x15 크기의 사각형으로 생성
    RectangleShape player({ 15.f,15.f });

    // 플레이어 시작 위치 설정
    player.setPosition({ 150.f,400.f });

    // 외곽선 색상 초록색
    player.setOutlineColor(Color::Green);

    // 외곽선 두께
    player.setOutlineThickness(2);

    //----------------------------------------
    // 적 생성
    //----------------------------------------
    // Enemy 객체들을 저장할 벡터
    vector<Enemy> enemies;

    // 최대 적 개수만큼 생성해서 벡터에 추가
    for (int i = 0; i < MAX_ENEMY_COUNT; i++)
    {
        enemies.emplace_back();
    }

    //----------------------------------------
    // 창 생성
    //----------------------------------------
    // 400x500 크기의 게임 창 생성
    RenderWindow window(VideoMode({ 400,500 }), "Collision Test");

    // 프레임 제한 60
    window.setFramerateLimit(60);

    // 프레임 간 시간 측정용 시계
    Clock clock;

    //----------------------------------------
    // 게임 루프
    //----------------------------------------
    // 창이 열려 있는 동안 계속 반복
    while (window.isOpen())
    {
        // 지난 프레임 이후 흐른 시간을 초 단위로 가져오고 시계를 즉시 리셋
        // 예: 0.016초 정도면 약 60FPS
        float deltaTime = clock.restart().asSeconds();

        //-------------------------
        // 이벤트
        //-------------------------
        // 창 닫기 같은 시스템 이벤트 처리
        while (const auto event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
        }

        //-------------------------
        // 입력
        //-------------------------
        // 방향키 입력에 따라 플레이어 이동
        // 현재는 프레임당 고정량 이동 방식
        if (Keyboard::isKeyPressed(Keyboard::Key::Left))
            player.move({ -MOVE_SPEED,0 });

        if (Keyboard::isKeyPressed(Keyboard::Key::Right))
            player.move({ MOVE_SPEED,0 });

        if (Keyboard::isKeyPressed(Keyboard::Key::Up))
            player.move({ 0,-MOVE_SPEED });

        if (Keyboard::isKeyPressed(Keyboard::Key::Down))
            player.move({ 0,MOVE_SPEED });

        //-------------------------
        // 적 업데이트
        //-------------------------
        // 모든 적을 한 프레임씩 아래로 이동
        for (auto& enemy : enemies)
        {
            enemy.Update(deltaTime);
        }

        //-------------------------
        // 충돌 검사
        //-------------------------
        // size_t는 vector 인덱스에 맞는 타입이라 안전함
        for (size_t i = 0; i < enemies.size(); i++)
        {
            // 현재 적이 플레이어와 충돌했는지 검사
            if (enemies[i].Collision(player.getGlobalBounds()))
            {
                cout << i << "번째 적과 충돌!" << endl;

                // 충돌한 적 제거
                enemies.erase(enemies.begin() + i);

                // erase 후 뒤 원소들이 앞으로 당겨지므로
                // 다음 원소를 건너뛰지 않기 위해 i를 1 감소
                i--;
            }
        }

        //-------------------------
        // 그리기
        //-------------------------
        // 이전 프레임 화면 지우기
        window.clear();

        // 모든 적 그리기
        for (const auto& enemy : enemies)
        {
            enemy.Draw(window);
        }

        // 플레이어 그리기
        window.draw(player);

        // 이번 프레임 결과를 실제 화면에 출력
        window.display();

        //-------------------------
        // 종료
        //-------------------------
        // 적이 모두 사라지면 게임 클리어 후 종료
        if (enemies.empty())
        {
            cout << "게임 클리어!" << endl;
            window.close();
        }
    }
}

// 하단의 코드는 안좋은 예시

//#include <SFML/Graphics.hpp>  // 도형, 창, 색상 등 그래픽 관련 기능
//#include <SFML/Audio.hpp>     // 오디오 관련 기능 (현재 코드에서는 실제로 사용 안 함)
//#include <iostream>           // cout, endl 사용을 위한 표준 입출력
//using namespace std;          // std::cout, std::vector 등을 std:: 없이 사용
//using namespace sf;           // sf::RenderWindow, sf::RectangleShape 등을 sf:: 없이 사용
//
//#define MOVE_PIXEL 5.f        // 플레이어가 한 번 이동할 때 움직이는 거리
//#define MAX_ENEMY_COUNT 20    // 생성할 적 상자의 최대 개수
//
//// 1부터 max 사이의 랜덤 실수를 반환하는 함수
//float rand_number(int max)
//{
//	float num = (float)(rand() % max + 1);
//
//	return num;
//}
//
//class collision
//{
//
//};
//
//int main()
//{
//	Clock clock;               // 프레임 간 경과 시간을 측정하기 위한 시계
//	float interval = 0;        // 누적 시간 저장용 변수
//	float rectx_p = 0, recty_p = 0;   // 플레이어 좌표용(현재는 사용되지 않음)
//	float rectx_n = 50, recty_n = 50; // 적 시작 위치 관련 값 (실제로는 recty_n만 사용됨)
//
//	// 난수 생성을 위한 시드 설정
//	srand((unsigned int)time(NULL));
//
//	// 플레이어 사각형 생성 (크기: 15 x 15)
//	RectangleShape rect_player(Vector2f(15.f, 15.f));
//
//	// 적 사각형들을 저장할 벡터
//	vector<RectangleShape> enemies;
//
//	// 적 사각형 하나를 임시로 만들고, 나중에 벡터에 복사해서 넣음
//	RectangleShape enemy_rect;
//
//	// 플레이어 크기 설정
//	rect_player.setSize(Vector2f(15.f, 15.f));
//
//	// 플레이어 외곽선 색상을 초록색으로 설정
//	rect_player.setOutlineColor(Color::Green);
//
//	// 플레이어 외곽선 두께 설정
//	rect_player.setOutlineThickness(2);
//
//	// 플레이어 시작 위치 설정
//	rect_player.setPosition(Vector2f(150.f, 400.f));
//
//	// 적 상자 MAX_ENEMY_COUNT개 생성
//	for (int i = 0; i < MAX_ENEMY_COUNT; i++)
//	{
//		// 적 크기를 랜덤하게 설정
//		enemy_rect.setSize(Vector2(rand_number(25), rand_number(25)));
//
//		// 적 외곽선 색상을 빨간색으로 설정
//		enemy_rect.setOutlineColor(Color::Red);
//
//		// 적 외곽선 두께 설정
//		enemy_rect.setOutlineThickness(5);
//
//		// 적 위치 설정 (x=500, y=50)
//		enemy_rect.setPosition(Vector2(500.0f, recty_n));
//
//		// 현재 적의 위치를 가져옴
//		Vector2f rpos = enemy_rect.getPosition();
//
//		// 콘솔에 적 위치 출력
//		cout << "Enemy " << i << " position: (" << rpos.x << ", " << rpos.y << ")" << endl;
//
//		// 벡터에 적 추가
//		enemies.push_back(enemy_rect);
//	}
//
//	// 게임 시작 메시지 출력
//	cout << "게임 시작" << endl;
//
//	// 게임 창 생성 (가로 400, 세로 500)
//	RenderWindow app(VideoMode({ 400, 500 }), "DDong!");
//
//	// 최대 프레임을 60으로 제한
//	app.setFramerateLimit(60);
//
//	// 창이 열려 있는 동안 게임 루프 반복
//	while (app.isOpen())
//	{
//		// 지난 프레임부터 현재까지 흐른 시간(초)을 구함
//		float time = clock.getElapsedTime().asSeconds();
//
//		// 시계를 다시 0으로 초기화해서 다음 프레임 시간 측정 준비
//		clock.restart();
//
//		// 누적 시간에 현재 프레임 시간 더하기
//		interval += time;
//
//		// 이벤트가 있는 동안 반복해서 처리
//		while (const optional event = app.pollEvent())
//		{
//			// 창 닫기 이벤트가 발생하면
//			if (event->is<Event::Closed>())
//			{
//				// 창 닫기
//				app.close();
//
//				// 콘솔에 종료 메시지 출력
//				cout << "게임 종료" << endl;
//			}
//
//			// 왼쪽 방향키를 누르고 있으면 플레이어를 왼쪽으로 이동
//			if (Keyboard::isKeyPressed(Keyboard::Key::Left))
//			{
//				rect_player.move(Vector2f(-MOVE_PIXEL, 0));
//			}
//
//			// 오른쪽 방향키를 누르고 있으면 플레이어를 오른쪽으로 이동
//			if (Keyboard::isKeyPressed(Keyboard::Key::Right))
//			{
//				rect_player.move(Vector2f(MOVE_PIXEL, 0));
//			}
//
//			// 위쪽 방향키를 누르고 있으면 플레이어를 위로 이동
//			if (Keyboard::isKeyPressed(Keyboard::Key::Up))
//			{
//				rect_player.move(Vector2f(0, -MOVE_PIXEL));
//			}
//
//			// 아래쪽 방향키를 누르고 있으면 플레이어를 아래로 이동
//			if (Keyboard::isKeyPressed(Keyboard::Key::Down))
//			{
//				rect_player.move(Vector2f(0, MOVE_PIXEL));
//			}
//		}
//
//		// 화면을 검은색으로 지움
//		app.clear(Color::Black);
//
//		// interval이 1초 단위로 나뉘는지 확인하는 조건
//		// 현재 코드에서는 거의 항상 참이 되기 쉬움
//		if ((int)interval % 1 == 0)
//		{
//			vector<RectangleShape>::iterator iter;
//
//			// 모든 적에 대해 반복
//			for (iter = enemies.begin(); iter != enemies.end(); iter++)
//			{
//				// 적을 아래 방향으로 랜덤 속도로 이동
//				(*iter).move(Vector2f(0, rand_number(3)));
//
//				// 현재 적 위치 가져오기
//				Vector2f pos = (*iter).getPosition();
//
//				// 적이 화면 아래로 벗어나면
//				if (pos.y > 500)
//					// 다시 위쪽 근처로 보내고 x 위치는 랜덤으로 재배치
//					(*iter).setPosition(Vector2f(rand_number(400), recty_n));
//			}
//		}
//
//		// 플레이어와 적 상자의 충돌 검사
//		for (int i = 0; i < enemies.size(); i++)
//		{
//			// 적의 경계와 플레이어 경계가 겹치면 충돌로 판단
//			if ((enemies[i].getGlobalBounds()).findIntersection(rect_player.getGlobalBounds()))
//			{
//				// 충돌한 적 제거
//				enemies.erase(enemies.begin() + i);
//
//				// 몇 번째 상자와 충돌했는지 출력
//				cout << i << "번째 상자와 충돌발생!" << endl;
//			}
//		}
//
//		// 모든 적 상자를 화면에 그림
//		vector<RectangleShape>::iterator iter;
//		for (iter = enemies.begin(); iter != enemies.end(); iter++)
//			app.draw(*iter);
//
//		// 플레이어 상자를 화면에 그림
//		app.draw(rect_player);
//
//		// 지금까지 그린 내용을 실제 화면에 표시
//		app.display();
//
//		// 적이 모두 사라졌다면 게임 종료
//		if (enemies.size() == 0)
//		{
//			cout << "충돌 시험이 끝났습니다. 프로그램을 종료합니다." << endl;
//			app.close();
//			break;
//		}
//	}
//
//	return 0;
//}
//
