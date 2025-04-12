// Effect.cpp
#include "Effect.h"

Effect::Effect(Type type, int level, int duration)
    : _type(type), _level(level), _remainingTurns(duration) {}

Effect::~Effect() {}

// 获取效果类型
Effect::Type Effect::getType() const {
    return _type;
}

// 获取效果等级
int Effect::getLevel() const {
    return _level;
}

// 获取效果剩余回合数
int Effect::getRemainingTurns() const {
    return _remainingTurns;
}

// 持续时间减少一回合
void Effect::reduceTurn() {
    if (_remainingTurns > 0) {
        --_remainingTurns;
    }
}

// 设置效果等级
void Effect::setLevel(int level) {
    _level = level;
}

// 增加剩余回合数
void Effect::addRemainingTurns(int turns) {
    if (_remainingTurns > 0) { // 如果效果是有限持续时间
        _remainingTurns += turns;
    }
}

// Buff 构造函数
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

// Debuff 构造函数
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