#pragma once

namespace Constants
{
    const unsigned int WINDOW_WIDTH = 1280;
    const unsigned int WINDOW_HEIGHT = 720;

    const float CENTER_X = WINDOW_WIDTH / 2.f;
    const float CENTER_Y = WINDOW_HEIGHT / 2.f;

    const float DEFAULT_SCALE = 3.0f;

    const float DEFAULT_COLLIDER_SIZE = 28.f;
    const float DEFAULT_ATTACK_RANGE = 48.f;

    const float WARRIOR_ATTACK_RANGE = 70;
    const float ARCHER_ATTACK_RANGE = 250.f;
    const float ARCHER_ANGLE = 90.f;
    const float MAGE_ATTACK_RANGE = 32.f;

    const int MIN_SPAWN_COUNT = 5;
    const int MAX_SPAWN_COUNT = 10;

    const float ROOM_SCALE = 0.7f;

    const int TOTAL_STAGE = 3;

    const int TRIPE_SHOT_SLOWRATE = 0.5;
    const int TRIPE_SHOT_SLOWDURATION = 1.0;
}