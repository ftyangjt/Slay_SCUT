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

void Card::addEffect(std::shared_ptr<Effect> effect)
{
    _effects.push_back(effect);
}

const std::vector<std::shared_ptr<Effect>>& Card::getEffects() const
{
    return _effects;
}