// Effect.cpp
#include "Effect.h"

Effect::Effect(Type type, int level, int duration)
    : _type(type), _level(level), _remainingTurns(duration) {}

Effect::~Effect() {}

// ��ȡЧ������
Effect::Type Effect::getType() const {
    return _type;
}

// ��ȡЧ���ȼ�
int Effect::getLevel() const {
    return _level;
}

// ��ȡЧ��ʣ��غ���
int Effect::getRemainingTurns() const {
    return _remainingTurns;
}

// ����ʱ�����һ�غ�
void Effect::reduceTurn() {
    if (_remainingTurns > 0) {
        --_remainingTurns;
    }
}

// ����Ч���ȼ�
void Effect::setLevel(int level) {
    _level = level;
}

// ����ʣ��غ���
void Effect::addRemainingTurns(int turns) {
    if (_remainingTurns > 0) { // ���Ч�������޳���ʱ��
        _remainingTurns += turns;
    }
}

// Buff ���캯��
Buff::Buff(Type type, int level, int duration)
    : Effect(type, level, duration) {
}

std::string Buff::getDescription() const {
    switch (_type) {
    case Type::Strength:
        return "Increase attack by " + std::to_string(_level);
    default:
        return "Unknown buff";
    }
}

// Debuff ���캯��
Debuff::Debuff(Type type, int level, int duration)
    : Effect(type, level, duration) {
}

std::string Debuff::getDescription() const {
    switch (_type) {
    case Type::Vulnerable:
        return "Take 50% more damage";
    default:
        return "Unknown debuff";
    }
}