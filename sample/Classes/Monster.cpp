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

    // ��ʼ�����������
    _health = 100; // Ĭ������ֵ

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
        return; // �������� effect �ǿ�ָ�룬ֱ�ӷ���
    }

    for (auto& existingEffect : _effects) {
        // ����Ƿ������ͬ���͵�Ч��
        if (existingEffect->getType() == effect->getType()) {
            if (effect->getType() == Effect::Type::Strength) {
                // ����Ч�������ӵȼ�
                existingEffect->setLevel(existingEffect->getLevel() + effect->getLevel());
            }
            else if (effect->getType() == Effect::Type::Vulnerable) {
                // ����Ч����ʱ������
                existingEffect->addRemainingTurns(effect->getRemainingTurns());
            }
            return; // ������ɺ�ֱ�ӷ���
        }
    }

    // ���û���ҵ���ͬ���͵�Ч����������µ�Ч��
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
            it = _effects.erase(it); // �Ƴ�����ʱ��Ϊ 0 ��Ч��
        }
        else {
            ++it;
        }
    }
}