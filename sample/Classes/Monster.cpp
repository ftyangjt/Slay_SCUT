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

void Monster::addEffect(std::shared_ptr<Effect> effect) {
    if (!effect) {
        CCLOG("Error: Attempted to add a null effect.");
        return; // 如果传入的 effect 是空指针，直接返回
    }

    for (auto& existingEffect : _effects) {
        // 检查是否存在相同类型的效果
        if (existingEffect->getType() == effect->getType()) {
            if (effect->getType() == Effect::Type::Strength) {
                // 力量效果：叠加等级
                existingEffect->setLevel(existingEffect->getLevel() + effect->getLevel());
            }
            else if (effect->getType() == Effect::Type::Vulnerable) {
                // 易伤效果：时长叠加
                existingEffect->addRemainingTurns(effect->getRemainingTurns());
            }
            return; // 处理完成后直接返回
        }
    }

    // 如果没有找到相同类型的效果，则添加新的效果
    _effects.push_back(effect);
}

const std::vector<std::shared_ptr<Effect>>& Monster::getEffects() const
{
    return _effects;
}

void Monster::updateEffects() {
    for (auto it = _effects.begin(); it != _effects.end(); ) {
        (*it)->reduceTurn();
        if ((*it)->getRemainingTurns() == 0) {
            it = _effects.erase(it); // 移除持续时间为 0 的效果
        }
        else {
            ++it;
        }
    }
}