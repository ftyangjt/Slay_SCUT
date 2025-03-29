// Effect.h
#ifndef EFFECT_H
#define EFFECT_H

#include <string>

class Effect {
public:
    enum class Type {
        Strength,  // а╕а©
        Vulnerable // рвик
    };

    Effect(Type type, int level);
    virtual ~Effect();

    Type getType() const;
    int getLevel() const;
    virtual std::string getDescription() const = 0;

protected:
    Type _type;
    int _level;
};

class Buff : public Effect {
public:
    Buff(Type type, int level);
    std::string getDescription() const override;
};

class Debuff : public Effect {
public:
    Debuff(Type type, int level);
    std::string getDescription() const override;
};

#endif // EFFECT_H