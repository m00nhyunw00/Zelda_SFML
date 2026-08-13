#include "Entity.h"

Entity::Entity()
{
    sprite = nullptr;
    active = true;
}

Entity::~Entity()
{
    delete sprite;
    sprite = nullptr;
}