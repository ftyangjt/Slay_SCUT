// Effect.h
#ifndef EFFECT_H
#define EFFECT_H

#include <string>

class Effect {
public:
    enum class Type {
        Strength,  // 力量
        Vulnerable // 易伤
    };

    Effect(Type type, int level, int duration = -1); // 默认持续时间为 -1，表示永久
    virtual ~Effect();

    Type getType() const;
    int getLevel() const;
    int getRemainingTurns() const; // 获取剩余回合数
    void reduceTurn();             // 减少一回合

    // 新增方法
    void setLevel(int level);           // 设置效果等级
    void addRemainingTurns(int turns);  // 增加剩余回合数

    virtual std::string getDescription() const = 0;

protected:
    Type _type;
    int _level;
    int _remainingTurns; // 剩余回合数
};

class Buff : public Effect {
public:
    Buff(Type type, int level, int duration = -1); // 添加持续时间参数，默认值为 -1
    std::string getDescription() const override;
};

class Debuff : public Effect {
public:
    Debuff(Type type, int level, int duration = -1); // 添加持续时间参数，默认值为 -1
    std::string getDescription() const override;
};
#endif // EFFECT_H