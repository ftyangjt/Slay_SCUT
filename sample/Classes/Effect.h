// Effect.h
#ifndef EFFECT_H
#define EFFECT_H

#include <string>

class Effect {
public:
    enum class Type {
        Strength,  // ����
        Vulnerable // ����
    };

    Effect(Type type, int level, int duration = -1); // Ĭ�ϳ���ʱ��Ϊ -1����ʾ����
    virtual ~Effect();

    Type getType() const;
    int getLevel() const;
    int getRemainingTurns() const; // ��ȡʣ��غ���
    void reduceTurn();             // ����һ�غ�

    // ��������
    void setLevel(int level);           // ����Ч���ȼ�
    void addRemainingTurns(int turns);  // ����ʣ��غ���

    virtual std::string getDescription() const = 0;

protected:
    Type _type;
    int _level;
    int _remainingTurns; // ʣ��غ���
};

class Buff : public Effect {
public:
    Buff(Type type, int level, int duration = -1); // ��ӳ���ʱ�������Ĭ��ֵΪ -1
    std::string getDescription() const override;
};

class Debuff : public Effect {
public:
    Debuff(Type type, int level, int duration = -1); // ��ӳ���ʱ�������Ĭ��ֵΪ -1
    std::string getDescription() const override;
};
#endif // EFFECT_H