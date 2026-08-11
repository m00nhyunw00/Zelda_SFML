#include "Monster.h"

Monster::Monster(
    MonsterType type,
    const MonsterData& data,
    const sf::Vector2f& position
) : Creature(CreatureType::MONSTER, data, position)
{
    this->type = type;
    this->color = data.color;
    this->exp = data.exp;
    this->maxAttackCooldown =data.attackCooldown;
    this->attackCooldown = 0.f;

    hpBar = new GaugeBar(
        { 50.f, 6.f },
        { position.x, position.y - 45.f },
        GetMaxHp()
    );

    monsterState = MonsterState::NONE_CHASE;

    target = nullptr;

    this->detectionRange = data.detectionRange;
    this->attackRange = data.attackRange;
}

Monster::~Monster()
{
    target = nullptr;

    delete hpBar;
    hpBar = nullptr;
}

sf::Vector2f Monster::AIMovement(float deltaTime)
{
    // 타겟이 없으면 움직이지 않음
    if (target == nullptr ||
        !target->IsActive())
    {
        monsterState = MonsterState::NONE_CHASE;
        animationState = CreatureState::RUN;

        return { 0.f, 0.f };
    }

    const sf::Vector2f targetPosition =
        target->GetPosition();

    // Monster → Player 방향 벡터
    sf::Vector2f direction =
    {
        targetPosition.x - position.x,
        targetPosition.y - position.y
    };

    // 직선 거리 계산
    const float distance =
        std::sqrt(
            direction.x * direction.x +
            direction.y * direction.y
        );

    // 감지 범위 밖
    if (distance > detectionRange)
    {
        monsterState = MonsterState::NONE_CHASE;

        return { 0.f, 0.f };
    }

    // 공격 범위
    if (distance <= attackRange)
    {
        monsterState = MonsterState::NONE_CHASE;

        if (distance > 0.f)
        {
            direction.x /= distance;
            direction.y /= distance;

            UpdateFacingDirection(direction);
        }

        if (canAttack &&
            animationState != CreatureState::ATTACK &&
            attackCooldown <= 0.f)
        {
            animationState = CreatureState::ATTACK;

            attackTriggered = true;

            attackCooldown = maxAttackCooldown;
        }

        return { 0.f, 0.f };
    }

    // 추격 상태
    monsterState = MonsterState::CHASE;

    if (distance > 0.f)
    {
        direction.x /= distance;
        direction.y /= distance;
    }

    UpdateFacingDirection(direction);

    // 이동 불가능 몬스터는 여기서만 이동 차단
    if (!canMove)
    {
        return { 0.f, 0.f };
    }

    Move(direction, deltaTime);

    return direction;
}

void Monster::HandleAnimation(const sf::Vector2f& direction, float deltaTime)
{
    // 공격 중이 아닐 때만 이동 여부로 상태 변경
    if (animationState != CreatureState::ATTACK)
    {
        if (direction.x != 0.f || direction.y != 0.f || !target->IsActive())
        {
            animationState = CreatureState::RUN;
        }
        else
        {
            animationState = CreatureState::IDLE;
        }
    }

    animation.Play(animationState,facingDirection);

    const bool animationFinished =
        animation.Update(*sprite, deltaTime);

    // 공격 애니메이션 한 사이클이 종료됨
    if (animationState == CreatureState::ATTACK && animationFinished)
    {
        if (direction.x != 0.f ||
            direction.y != 0.f)
        {
            animationState = CreatureState::RUN;
        }
        else
        {
            animationState = CreatureState::IDLE;
        }
    }
}

void Monster::UpdateLogic(float deltaTime)
{
    attackTriggered = false;

    if (attackCooldown > 0.f)
    {
        attackCooldown -= deltaTime;

        if (attackCooldown < 0.f)
        {
            attackCooldown = 0.f;
        }
    }

    const sf::Vector2f direction = AIMovement(deltaTime);

    HandleAnimation(direction, deltaTime);

    UpdateTypeLogic(deltaTime);

    hpBar->SetValue(GetHp());

    hpBar->SetPosition({ position.x,position.y - 45.f });
}

void Monster::Render(sf::RenderWindow& window)
{
    Creature::Render(window);

    if (hpBar != nullptr && showHpBar)
    {
        hpBar->Render(window);
    }
}

void Monster::AddPendingProjectile(Projectile* projectile)
{
    if (projectile == nullptr)
    {
        return;
    }

    pendingProjectiles.push_back(projectile);
}

std::vector<Projectile*> Monster::TakePendingProjectiles()
{
    std::vector<Projectile*> result = std::move(pendingProjectiles);

    pendingProjectiles.clear();

    return result;
}

void Monster::SetAttakRange(float range)
{
    attackRange = range;
}
