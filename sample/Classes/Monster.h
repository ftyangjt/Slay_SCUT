#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "cocos2d.h"
#include "Effect.h"
#include <unordered_map>
#include <functional>
#include <random>
#include "Hero.h"

// ���������Ϊ����
enum class MonsterActionType {
    ATTACK,     // ����
    DEFEND,     // ����
    BUFF,       // ���Լ�����Buff
    DEBUFF,     // ���������Debuff
    SPECIAL     // ������Ϊ
};

// ���������Ϊ�ṹ��
struct MonsterAction {
    MonsterActionType type;  // ��Ϊ����
    int value;               // ��Ϊֵ(����ֵ������ֵ��)
    Effect::Type effectType; // Ч������(����BUFF/DEBUFF)
    int effectLevel;         // Ч���ȼ�
    int effectDuration;      // Ч������ʱ��
    std::string description; // ��Ϊ����

    MonsterAction(
        MonsterActionType _type = MonsterActionType::ATTACK,
        int _value = 0,
        Effect::Type _effectType = Effect::Type::Strength,
        int _effectLevel = 0,
        int _effectDuration = 0,
        const std::string& _desc = ""
    ) : type(_type), value(_value), effectType(_effectType),
        effectLevel(_effectLevel), effectDuration(_effectDuration),
        description(_desc) {
    }
};

// ���������
class Monster : public cocos2d::Sprite
{
public:
    // �������������ڴ�������ʵ��
    static Monster* createFactory(const std::string& monsterClassName);

    // ������������ָ���Ƿ�ΪBoss��Ӣ��
    static Monster* createRandom(bool isBoss = false, bool isElite = false);

    // ��ʼ������
    virtual bool init();
    virtual bool initWithSprite(const std::string& spritePath);

    // ���úͻ�ȡ���������ֵ
    void setHealth(int health);
    int getHealth() const;

    // ���úͻ�ȡ����ķ���ֵ
    void setBlock(int block);
    int getBlock() const;

    // ���úͻ�ȡ����Ĺ�����
    void setAttackDamage(int damage);
    int getAttackDamage() const;


    // Ч�������ӿ�
    void addEffect(std::shared_ptr<Effect> effect);
    const std::vector<std::shared_ptr<Effect>>& getEffects() const;
    void updateEffects(); // ����Ч�����Ƴ�����ʱ��Ϊ 0 ��Ч��

    // ��ʼ���������Ϊģʽ - ����Ϊ���麯�����������ʵ��
    virtual void initActionPattern() = 0;

    // ��ȡ��ǰ�غϵ���Ϊ
    MonsterAction getCurrentAction();

    // ִ�е�ǰ�غϵ���Ϊ
    virtual void executeCurrentAction(Hero* target);

    // ׼����һ�غϵ���Ϊ(���ӻغϼ���)
    void prepareNextTurn();

    // ��ȡ��һ�غ���Ϊ������(����UI��ʾ)
    std::string getNextActionDescription() const;

    // �жϹ�������
    virtual bool isBoss() const { return false; }
    virtual bool isElite() const { return false; }

    // ��ȡ��������
    virtual std::string getMonsterName() const { return "Unknown Monster"; }

protected:
    int _health;
    int _block;
    int _attackDamage;
    std::vector<std::shared_ptr<Effect>> _effects;
    std::vector<MonsterAction> _actionPattern;  // ��Ϊģʽ�б�
    int _currentTurn = 0;                       // ��ǰ�غ���

    static std::mt19937 _rng; // �����������
};

// ��ͨ�������
class NormalMonster : public Monster
{
public:
    virtual bool init() override;
    bool isBoss() const override { return false; }
    bool isElite() const override { return false; }

protected:
    // ��ͨ���ﹲ�е�һЩ���Կ���������ʵ��
};

// ��Ӣ�������
class EliteMonster : public Monster
{
public:
    virtual bool init() override;
    bool isBoss() const override { return false; }
    bool isElite() const override { return true; }

protected:
    // ��Ӣ���ﹲ�е�һЩ���Կ���������ʵ��
};

// Boss�������
class BossMonster : public Monster
{
public:
    virtual bool init() override;
    bool isBoss() const override { return true; }
    bool isElite() const override { return false; }

protected:
    // Boss���ﹲ�е�һЩ���Կ���������ʵ��
};

// �������ͨ������
class SlimeMonster : public NormalMonster {
public:
    CREATE_FUNC(SlimeMonster);
    bool init() override;
    void initActionPattern() override;
    std::string getMonsterName() const override { return "Slime"; }
};

class GoblinMonster : public NormalMonster {
public:
    CREATE_FUNC(GoblinMonster);
    bool init() override;
    void initActionPattern() override;
    std::string getMonsterName() const override { return "Goblin"; }
};

class SnakeMonster : public NormalMonster {
public:
    CREATE_FUNC(SnakeMonster);
    bool init() override;
    void initActionPattern() override;
    std::string getMonsterName() const override { return "Snake"; }
};

// ����ľ�Ӣ������
class OrcMonster : public EliteMonster {
public:
    CREATE_FUNC(OrcMonster);
    bool init() override;
    void initActionPattern() override;
    std::string getMonsterName() const override { return "Orc"; }
};

class GolemMonster : public EliteMonster {
public:
    CREATE_FUNC(GolemMonster);
    bool init() override;
    void initActionPattern() override;
    std::string getMonsterName() const override { return "Golem"; }
};

// �����Boss������
class KnightBossMonster : public BossMonster {
public:
    CREATE_FUNC(KnightBossMonster);
    bool init() override;
    void initActionPattern() override;
    std::string getMonsterName() const override { return "Dark Knight"; }
};

class DragonBossMonster : public BossMonster {
public:
    CREATE_FUNC(DragonBossMonster);
    bool init() override;
    void initActionPattern() override;
    std::string getMonsterName() const override { return "Ancient Dragon"; }
};

#endif // __MONSTER_H__
