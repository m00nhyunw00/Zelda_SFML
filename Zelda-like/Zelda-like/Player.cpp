#include "Player.h"
#include "CreatureType.h"

Player::Player(
	const std::string& name,
	PlayerType job,
	const PlayerData& data,
	const sf::Vector2f& position
) : Creature(CreatureType::PLAYER, data, position)
{
	this->userName = name;
	this->job = job;
	this->level = 1;
	this->skillName = data.skillName;
	this->skillDamage = data.skillDamage;
	this->maxSkillCooldown = data.maxSkillCooldown;
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

void Player::Heal(int amount)
{

}
