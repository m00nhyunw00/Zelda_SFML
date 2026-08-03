# SFML 프로젝트 설정 가이드

이 문서는 Windows 환경에서 **Visual Studio 2022**와 **SFML 3.1.0 (64-bit)**을 사용하기 위한 프로젝트 설정 방법을 정리한 문서입니다.

## 개발 환경

- Windows 10/11
- Visual Studio 2022
- C++17
- SFML 3.1.0 64-bit
- x64 플랫폼

## SFML 사용 목적

SFML(Simple and Fast Multimedia Library)은 게임 제작 자체를 자동화하기 위한 도구라기보다, C++ 문법을 실제 게임 프로그래밍에 적용하기 위한 라이브러리입니다.

프로젝트를 통해 다음과 같은 내용을 학습하고 적용할 수 있습니다.

- C++ 객체지향 설계
- STL 활용
- 게임 루프 구현
- 충돌 처리
- 애니메이션
- 리소스 관리
- 디자인 패턴 적용

---

## 1. SFML 설치

SFML 3.1.0 64-bit 버전을 내려받은 뒤 다음 경로에 압축을 해제합니다.

```text
C:\SFML-3.1.0
```

정상적으로 설치되었다면 다음 폴더가 존재해야 합니다.

```text
C:\SFML-3.1.0
├─ bin
├─ include
└─ lib
```

> SFML을 다른 위치에 설치한 경우 아래 설정 경로도 실제 설치 위치에 맞게 변경해야 합니다.

---

## 2. Visual Studio 프로젝트 기본 설정

Visual Studio에서 프로젝트를 연 뒤 다음 순서로 이동합니다.

```text
프로젝트 우클릭
→ 속성(Properties)
```

프로젝트 속성 창 상단에서 다음과 같이 설정합니다.

```text
구성(Configuration): 모든 구성(All Configurations)
플랫폼(Platform): x64
```

---

## 3. C++17 설정

다음 메뉴로 이동합니다.

```text
구성 속성
└─ C/C++
   └─ 언어
      └─ C++ 언어 표준
```

값을 다음과 같이 설정합니다.

```text
ISO C++17 표준 (/std:c++17)
```

> `C/C++ → 전처리기 → 전처리기 정의`에는 별도의 값을 입력하지 않습니다.

---

## 4. 추가 포함 디렉터리 설정

다음 메뉴로 이동합니다.

```text
구성 속성
└─ C/C++
   └─ 일반
      └─ 추가 포함 디렉터리
```

다음 경로를 입력합니다.

```text
C:\SFML-3.1.0\include
```

---

## 5. 추가 라이브러리 디렉터리 설정

다음 메뉴로 이동합니다.

```text
구성 속성
└─ 링커
   └─ 일반
      └─ 추가 라이브러리 디렉터리
```

다음 경로를 입력합니다.

```text
C:\SFML-3.1.0\lib
```

---

## 6. 링커 입력 설정

Debug와 Release 구성은 사용하는 라이브러리 파일이 다르므로 각각 따로 설정해야 합니다.

### Release 구성

프로젝트 속성 창 상단의 구성을 `Release`로 변경한 뒤 다음 메뉴로 이동합니다.

```text
구성 속성
└─ 링커
   └─ 입력
      └─ 추가 종속성
```

다음 라이브러리를 입력합니다.

```text
sfml-graphics.lib
sfml-window.lib
sfml-system.lib
sfml-audio.lib
sfml-network.lib
```

### Debug 구성

프로젝트 속성 창 상단의 구성을 `Debug`로 변경한 뒤 다음 메뉴로 이동합니다.

```text
구성 속성
└─ 링커
   └─ 입력
      └─ 추가 종속성
```

다음 라이브러리를 입력합니다.

```text
sfml-graphics-d.lib
sfml-window-d.lib
sfml-system-d.lib
sfml-audio-d.lib
sfml-network-d.lib
```

Debug용 라이브러리는 파일명 뒤에 `-d`가 붙습니다.

---

## 7. DLL 파일 설정

프로그램 실행 시 SFML DLL 파일을 찾지 못하는 오류가 발생할 수 있습니다.

이 경우 다음 폴더 안의 DLL 파일을 복사합니다.

```text
C:\SFML-3.1.0\bin
```

복사한 파일을 프로젝트의 실행 파일이 생성되는 폴더에 붙여 넣습니다.

```text
프로젝트폴더\x64\Debug
프로젝트폴더\x64\Release
```

예시:

```text
Zelda_SFML
└─ x64
   ├─ Debug
   └─ Release
```

> 필요한 DLL을 모두 복사해도 되고, 프로젝트에서 사용하는 SFML 모듈의 DLL만 복사해도 됩니다.

---

## 8. 테스트 코드

설정이 완료되면 다음 코드를 실행하여 SFML이 정상적으로 동작하는지 확인합니다.

```cpp
#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({ 200, 200 }),
        "SFML works!"
    );

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}
```

초록색 원이 표시된 창이 정상적으로 실행되면 설정이 완료된 것입니다.

---

## 9. 자주 발생하는 오류

### `SFML/Graphics.hpp`를 찾을 수 없는 경우

다음 경로가 올바르게 설정되었는지 확인합니다.

```text
C/C++ → 일반 → 추가 포함 디렉터리
C:\SFML-3.1.0\include
```

### `LNK1104` 또는 `.lib` 파일을 찾을 수 없는 경우

다음 경로가 올바르게 설정되었는지 확인합니다.

```text
링커 → 일반 → 추가 라이브러리 디렉터리
C:\SFML-3.1.0\lib
```

또한 Debug 구성에서는 `-d.lib`, Release 구성에서는 일반 `.lib` 파일을 사용했는지 확인합니다.

### DLL을 찾을 수 없다는 오류가 발생하는 경우

다음 폴더의 DLL 파일을 실행 파일이 있는 `x64\Debug` 또는 `x64\Release` 폴더에 복사합니다.

```text
C:\SFML-3.1.0\bin
```

### x86/x64 관련 링커 오류가 발생하는 경우

프로젝트 플랫폼과 설치한 SFML 버전이 모두 x64인지 확인합니다.

```text
플랫폼: x64
SFML: 64-bit
```

---

## 참고

- SFML 공식 문서: [SFML and Visual Studio](https://www.sfml-dev.org/tutorials/3.1/getting-started/visual-studio/)
