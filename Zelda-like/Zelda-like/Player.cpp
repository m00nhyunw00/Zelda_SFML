#include "Player.h"
#include "CreatureType.h"

Player::Player(
	std::string name, 
	PlayerType job, 
	int maxHp,
	int defence,
	int damage,
	int evasionRate,
	std::string skillName, 
	int skillDamage, 
	float maxSkillCooldown 
) : Creature(CreatureType::PLAYER, maxHp, defence, damage, evasionRate)
{
	this->userName = name;
	this->job = job;
	this->level = 1;
	this->skillName = skillName;
	this->skillDamage = skillDamage;
	this->maxSkillCooldown = maxSkillCooldown;
	this->skillCooldown = 0;
	this->maxExp = 100;
	this->currentExp = 0;
}

void Player::IncreaseMaxHp(int amount)
{

}

void Player::IncreaseDamage(int amount)
{

}

void Player::IncreaseSkillDamage(int amount)
{

}

void Player::AddExp(int exp)
{

}

void Player::LevelUp()
{

}

void Player::IncreaseStats(int maxHpAmount, int damageAmount, int skillDamageAmount)
{

}

void Player::UseSkill(Creature* target)
{

}

void Player::Heal(int amount)
{

}

void Player::Attack(Creature* target)
{

}

void Player::UpdateLogic(float deltaTime)
{

}