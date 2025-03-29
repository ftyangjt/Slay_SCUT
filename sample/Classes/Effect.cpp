// Effect.cpp
#include "Effect.h"

Effect::Effect(Type type, int level)
    : _type(type), _level(level) {
}

Effect::~Effect() {}

Effect::Type Effect::getType() const {
    return _type;
}

int Effect::getLevel() const {
    return _level;
}

Buff::Buff(Type type, int level)
    : Effect(type, level) {
}

std::string Buff::getDescription() const {
    switch (_type) {
    case Type::Strength:
        return "Increase attack by " + std::to_string(_level);
    default:
        return "Unknown buff";
    }
}

Debuff::Debuff(Type type, int level)
    : Effect(type, level) {
}

std::string Debuff::getDescription() const {
    switch (_type) {
    case Type::Vulnerable:
        return "Take 50% more damage";
    default:
        return "Unknown debuff";
    }
}
