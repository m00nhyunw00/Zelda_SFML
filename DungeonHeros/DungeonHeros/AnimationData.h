#pragma once

#include <string>

struct AnimationData
{
    std::string textureKey;  // ResourceManager에 등록된 Texture 키

    int totalRows;           // 스프라이트 시트 전체 행 개수
    int totalColumns;        // 스프라이트 시트 전체 열 개수
    int frameCount;          // 실제 재생할 프레임 개수

    float frameDuration;     // 프레임 하나가 유지되는 시간
};