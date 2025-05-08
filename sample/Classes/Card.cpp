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

// ���Ч�� - ���ڽ���Ч�����͡��ȼ���ʱ��
void Card::addEffect(Effect::Type type, int level, int duration) {
    _effectInfos.push_back(EffectInfo(type, level, duration));
}

// ��ȡЧ����Ϣ�б�
const std::vector<EffectInfo>& Card::getEffectInfos() const {
    return _effectInfos;
}

// ��������ȡЧ������ - ���ݴ洢��Ч����Ϣ����ʵ�ʵ� Effect ����
std::vector<std::shared_ptr<Effect>> Card::createEffects() const {
    std::vector<std::shared_ptr<Effect>> effects;

    for (const auto& info : _effectInfos) {
        // ����Ч�����ʹ�����Ӧ�ľ���Ч������
        if (info.type == Effect::Type::Strength) {
            effects.push_back(std::make_shared<Buff>(info.type, info.level, info.duration));
        }
        else if (info.type == Effect::Type::Vulnerable) {
            effects.push_back(std::make_shared<Debuff>(info.type, info.level, info.duration));
        }
        // ������������͵�Ч����������������Ӹ����������֧
    }

    return effects;
}

// ��������Ч��
void Card::setSpecialEffect(SpecialEffect effect, int value) {
    // ����Ƿ�������ͬ���͵�Ч��
    for (auto& pair : _specialEffects) {
        if (pair.first == effect) {
            // ����У�����ֵ
            pair.second = value;
            return;
        }
    }
    // ���û�У������Ч��
    _specialEffects.emplace_back(effect, value);
}

// ��ȡ����Ч��ֵ
int Card::getSpecialEffectValue(SpecialEffect effect) const {
    for (const auto& pair : _specialEffects) {
        if (pair.first == effect) {
            return pair.second;
        }
    }
    return 0; // ���û���ҵ�Ч��������0
}

// �ж��Ƿ����ض�Ч��
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