#include <SFML/Graphics.hpp>
#include <iostream>

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
		window.display();                   // 화면에 표시

		//sf::Time elapsed = clock.restart();
  //          
  //      cout << "경과시간 : " << elapsed.asSeconds() << "초" << endl;
  //      updateGame(elapsed);
    }
}

int test()
{
	cout << "::test() 함수 호출" << endl;
}