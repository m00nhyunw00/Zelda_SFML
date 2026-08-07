#include "Slime.h"
#include "ResourceManager.h"
#include "MonsterType.h"
#include "MonsterColor.h"
#include "SpriteUtil.h"
#include "Constants.h"
#include <iostream>

using namespace std;

Slime::Slime(
    const MonsterColor& color,
    const MonsterData& data,
    const sf::Vector2f& startPosition
) : Monster(MonsterType::SLIME, data, startPosition)
{
    sf::Texture* idleTexture = ResourceManager::GetInstance().GetTexture("Slime_Idle");

    animation.SetOwnerType("SLIME");

    if (idleTexture == nullptr)
    {
        cerr << "Slime_Idle texture not found." << endl;
        return;
    }

    sprite = new sf::Sprite(*idleTexture);

    sprite->setTextureRect(
        SpriteUtil::GetFrameRect(
            *idleTexture,
            1,
            6,
            0,
            0
        )
    );

    sprite->setScale({ Constants::DEFAULT_SCALE, Constants::DEFAULT_SCALE });
    sprite->setPosition(startPosition);

    attackCollider.SetSize({ Constants::DEFAULT_COLLIDER_SIZE, data.attackRange });
    attackCollider.SetOrigin({ Constants::DEFAULT_COLLIDER_SIZE / 2, data.attackRange });
}

void Slime::UpdateTypeLogic(float deltaTime)
{

}

void Slime::Attack(Creature* target)
{
    int random = rand() % 100;

    if (target->GetEvasionRate() < random)	// 회피율에 따른 공격 성공
    {
        int randomDamageBoundary = (int)((float)GetDamage() * 0.2);
        int randomDamage = (rand() % randomDamageBoundary) - ((int)((float)randomDamageBoundary / 2));	// 기본 공격력의 +- 10%를 공격력으로 랜덤하게 설정

        int finalDamage = GetDamage() + randomDamage - target->GetDefence();		// 방어력과 공격력을 감안한 takeDamage()에 넘겨줄 최종 데미지

        int realDamage = target->TakeDamage(finalDamage);

        cout << "Slime Attack" << endl;
    }
    else
    {
        cout << "Slime Attack Failed" << endl;
    }
}