#include "Warrior.h"
#include "ResourceManager.h"
#include "PlayerType.h"
#include "SpriteUtil.h"
#include "Constants.h"
#include <iostream>

using namespace std;

Warrior::Warrior(
    const std::string& name,
    const PlayerData& data,
    const sf::Vector2f& startPosition
) : Player(name, PlayerType::MAGE, data, startPosition)
{
    sf::Texture* idleTexture = ResourceManager::GetInstance().GetTexture("Player_Idle");

    animation.SetOwnerType("WARRIOR");

    if (idleTexture == nullptr)
    {
        cerr << "Player_Idle texture not found." << endl;
        return;
    }

    sprite = new sf::Sprite(*idleTexture);

    sprite->setTextureRect(
        SpriteUtil::GetFrameRect(
            *idleTexture,
            3,
            4,
            0,
            0
        )
    );

    sprite->setScale({ Constants::DEFAULT_SCALE, Constants::DEFAULT_SCALE });
    sprite->setPosition(startPosition);

    attackCollider.SetSize({ Constants::DEFAULT_COLLIDER_SIZE * 2, Constants::WARRIOR_ATTACK_RANGE });
    attackCollider.SetOrigin({ Constants::DEFAULT_COLLIDER_SIZE, Constants::WARRIOR_ATTACK_RANGE });
}

void Warrior::UpdateJobLogic(float deltaTime)
{

}

void Warrior::Attack(Creature* target)
{
    int random = rand() % 100;

    if (target->GetEvasionRate() < random)	// 회피율에 따른 공격 성공
    {
        int randomDamageBoundary = (int)((float)GetDamage() * 0.2);
        int randomDamage = (rand() % randomDamageBoundary) - ((int)((float)randomDamageBoundary / 2));	// 기본 공격력의 +- 10%를 공격력으로 랜덤하게 설정

        int finalDamage = GetDamage() + randomDamage - target->GetDefence();		// 방어력과 공격력을 감안한 takeDamage()에 넘겨줄 최종 데미지

        int realDamage = target->TakeDamage(finalDamage);

        cout << "Warrior Attack" << endl;

    }
    else
    {
        cout << "Warrior Attack Failed" << endl;

    }
}

void Warrior::UseSkill(Creature* target)
{
    return;
}