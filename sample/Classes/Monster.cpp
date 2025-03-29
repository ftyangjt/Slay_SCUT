#include "Monster.h"

USING_NS_CC;

Monster* Monster::create(const std::string& filename)
{
    Monster* monster = new (std::nothrow) Monster();
    if (monster && monster->init(filename))
    {
        monster->autorelease();
        return monster;
    }
    CC_SAFE_DELETE(monster);
    return nullptr;
}

bool Monster::init(const std::string& filename)
{
    if (!Sprite::initWithFile(filename))
    {
        return false;
    }

    // 初始化怪物的属性
    _health = 100; // 默认生命值

    return true;
}

void Monster::setHealth(int health)
{
    _health = health;
}

int Monster::getHealth() const
{
    return _health;
}

void Monster::setBlock(int block) {
    _block = block;
}

int Monster::getBlock() const {
    return _block;
}

void Monster::addEffect(std::shared_ptr<Effect> effect)
{
    _effects.push_back(effect);
}

const std::vector<std::shared_ptr<Effect>>& Monster::getEffects() const
{
    return _effects;
}