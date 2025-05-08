// Card.cpp
#include "Card.h"

USING_NS_CC;

Card::Card(const std::string& name, Type type, int cost, const std::string& effect, const std::string& backgroundFile, int attack, int block)
    : _name(name), _type(type), _cost(cost), _effect(effect), _backgroundFile(backgroundFile), _attack(attack), _block(block), _sprite(nullptr)
{
}

Card::~Card()
{

}

const std::string& Card::getName() const
{
    return _name;
}

Card::Type Card::getType() const
{
    return _type;
}

int Card::getCost() const
{
    return _cost;
}

const std::string& Card::getEffect() const
{
    return _effect;
}

int Card::getAttack() const
{
    return _attack;
}

int Card::getBlock() const
{
    return _block;
}

cocos2d::Sprite* Card::createCardSprite()
{
    _sprite = Sprite::create(_backgroundFile);
    return _sprite;
}

cocos2d::Sprite* Card::getSprite() const
{
    return _sprite;
}

// 添加效果 - 现在接受效果类型、等级和时长
void Card::addEffect(Effect::Type type, int level, int duration) {
    _effectInfos.push_back(EffectInfo(type, level, duration));
}

// 获取效果信息列表
const std::vector<EffectInfo>& Card::getEffectInfos() const {
    return _effectInfos;
}

// 创建并获取效果对象 - 根据存储的效果信息创建实际的 Effect 对象
std::vector<std::shared_ptr<Effect>> Card::createEffects() const {
    std::vector<std::shared_ptr<Effect>> effects;

    for (const auto& info : _effectInfos) {
        // 根据效果类型创建对应的具体效果对象
        if (info.type == Effect::Type::Strength) {
            effects.push_back(std::make_shared<Buff>(info.type, info.level, info.duration));
        }
        else if (info.type == Effect::Type::Vulnerable) {
            effects.push_back(std::make_shared<Debuff>(info.type, info.level, info.duration));
        }
        // 如果有其他类型的效果，可以在这里添加更多的条件分支
    }

    return effects;
}

// 设置特殊效果
void Card::setSpecialEffect(SpecialEffect effect, int value) {
    // 检查是否已有相同类型的效果
    for (auto& pair : _specialEffects) {
        if (pair.first == effect) {
            // 如果有，更新值
            pair.second = value;
            return;
        }
    }
    // 如果没有，添加新效果
    _specialEffects.emplace_back(effect, value);
}

// 获取特殊效果值
int Card::getSpecialEffectValue(SpecialEffect effect) const {
    for (const auto& pair : _specialEffects) {
        if (pair.first == effect) {
            return pair.second;
        }
    }
    return 0; // 如果没有找到效果，返回0
}

// 判断是否有特定效果
bool Card::hasSpecialEffect(SpecialEffect effect) const {
    for (const auto& pair : _specialEffects) {
        if (pair.first == effect) {
            return true;
        }
    }
    return false;
}

bool Card::isPlayable() const {
    return _isPlayable;
}

void Card::setPlayable(bool playable) {
    _isPlayable = playable;
}