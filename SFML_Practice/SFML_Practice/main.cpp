#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/Audio.hpp>

using namespace std;

//sf::Time StackTime;
//
//void updateGame(sf::Time t)
//{
//    StackTime += t;
//	cout << "       게임 갱신 중 : " << StackTime.asSeconds() << "초" << endl;
//}

int main()
{
	// 사운드 로드 ----------------------------------------------------------------------
    
    // 1
    sf::SoundBuffer buffer("Resources/1up.wav");

    // 2
    sf::SoundBuffer buffer2;
    if(buffer2.loadFromFile("Resources/1up.wav"))
    {
        cout << "사운드 로드 성공" << endl;
    }
    else
    {
        cout << "사운드 로드 실패" << endl;
	}

    // SFX / BGM 
    sf::Sound sound(buffer);
	sound.setPlayingOffset(sf::seconds(1.0f));  // 1초부터 재생
    sound.play();   // 재생
    //sound.pause();  // 일시정지
    //sound.stop();   // 정지

    sf::Music bgm("Resources/mario.wav");
	bgm.setLoopPoints({ sf::microseconds(40000), sf::seconds(4) }); // 루프 구간 설정
	bgm.play(); // 재생

	// 폰트 로드 ----------------------------------------------------------------------

    sf::Font font;
    if (!font.openFromFile("Resources/windows-bold.ttf"))
    {
        cout << "폰트 로드 실패" << endl;
    }
    else
    {
        cout << "폰트 로드 성공" << endl;
    }

    sf::Text text(font);

	text.setString("Hello SFML");
	text.setCharacterSize(30);
	text.setFillColor(sf::Color::Blue);
	text.setStyle(sf::Text::Bold | sf::Text::Italic);



	// 이미지 로드 ----------------------------------------------------------------------

    sf::Texture texture;
    if (!texture.loadFromFile("Resources/test.png", false, sf::IntRect({ 10,10 }, { 32,32 })))
    {
        // 로드되지 않으면
		cout << "이미지 로드 실패" << endl;
    }
    else
    {
		cout << "이미지 로드 성공" << endl;
    }

	texture.setSmooth(true); // 안티 알리아싱 스무스 : 텍스처마다 확인할 필요 있음

	sf::Sprite sprite(texture);
	sprite.setColor(sf::Color(0, 255, 0)); // 색상 변경 : R,G,B

	// 윈도우 생성 ----------------------------------------------------------------------

    sf::RenderWindow window(sf::VideoMode({ 500, 500 }), "Moon works!");

    // 원 그리기 ----------------------------------------------------------------------
    
    //sf::CircleShape shape(250);
    //shape.setFillColor(sf::Color::Red);

    // 각도 계산 ----------------------------------------------------------------------

	//sf::Angle angle1 = sf::degrees(90.f);
	//angle1 *= 2.0f;     // 180 degrees

	//sf::Angle angle2 = sf::radians(3.14159f);

    //bool equal = (angle1 == angle2);
    //bool inEqual = (angle1 != angle2);

    //cout << "Angle degree : " << angle1.asDegrees() << endl;
    //cout << "Angle1 == Angle2 : " << equal << endl;
    //cout << "Angle1 : " << angle1.asDegrees() << endl;

    // 시간 계산 ----------------------------------------------------------------------

    //sf::Time t1 = sf::microseconds(10000);
	//sf::Time t2 = sf::milliseconds(10);
	//sf::Time t3 = sf::seconds(0.01f);

    sf::Clock clock;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
				// Escape 키를 누르면 창을 닫음
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                {
                    window.close();
				}
                if (keyPressed->scancode == sf::Keyboard::Scancode::Left)
                {
                    cout << "왼쪽 방향키 입력" << endl;
				}
                if (keyPressed->scancode == sf::Keyboard::Scancode::Right)
                {
                    cout << "오른쪽 방향키 입력" << endl;
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::Up)
                {
                    cout << "위쪽 방향키 입력" << endl;
				}
                if (keyPressed->scancode == sf::Keyboard::Scancode::Down)
                {
                    cout << "아래쪽 방향키 입력" << endl;
                }
            }

            else if (const auto* MouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
            {
                switch(MouseWheelScrolled->wheel)
                {
                case sf::Mouse::Wheel::Vertical:
                    cout << "마우스 휠 수직 스크롤 : " << MouseWheelScrolled->delta << endl;
                    break;
                case sf::Mouse::Wheel::Horizontal:
                    cout << "마우스 휠 수평 스크롤 : " << MouseWheelScrolled->delta << endl;
                    break;
                default:
                    break;
				}

				cout << "Wheel Movement : " << MouseWheelScrolled->delta << endl;
				cout << "Mouse x : " << MouseWheelScrolled->position.x << endl;
				cout << "Mouse y : " << MouseWheelScrolled->position.y << endl;
			}
            //else if (const auto* LeftMouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
            //{
            //    if (LeftMouseButtonPressed->button == sf::Mouse::Button::Left)
            //    {
            //        cout << "마우스 왼쪽 버튼 클릭" << endl;
            //        cout << "Mouse x : " << LeftMouseButtonPressed->position.x << endl;
            //        cout << "Mouse y : " << LeftMouseButtonPressed->position.y << endl;
            //    }
            //}
            else if (const auto* MouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (MouseButtonPressed->button == sf::Mouse::Button::Left)
                {
                    cout << "왼쪽 마우스 버튼 클릭" << endl;
                    cout << "Mouse x : " << MouseButtonPressed->position.x << endl;
                    cout << "Mouse y : " << MouseButtonPressed->position.y << endl;
                }
                if (MouseButtonPressed->button == sf::Mouse::Button::Right)
                {
                    cout << "오른쪽 마우스 버튼 클릭" << endl;
                    cout << "Mouse x : " << MouseButtonPressed->position.x << endl;
                    cout << "Mouse y : " << MouseButtonPressed->position.y << endl;
                }
			}
        }

		//window.setSize({ 500, 500 });       // 창 크기 설정
		//window.setPosition({ 500, 100 });   // 창 위치 설정
		//window.setTitle("SFML");            // 창 제목 설정
		window.clear();                     // 창 초기화
		//window.draw(shape);                 // 원 그리기
        window.draw(sprite);                // 이미지 그리기
        window.draw(text);                  // 텍스트 그리기
		window.display();                   // 화면에 표시

		//sf::Time elapsed = clock.restart();
            
        //cout << "경과시간 : " << elapsed.asSeconds() << "초" << endl;
        //updateGame(elapsed);
    }
}