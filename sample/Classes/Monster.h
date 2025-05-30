#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "cocos2d.h"
#include "Effect.h"
#include <unordered_map>
#include <functional>
#include <random>
#include "Hero.h"

// 定义怪物行为类型
enum class MonsterActionType {
    ATTACK,     // 攻击
    DEFEND,     // 防御
    BUFF,       // 给自己增加Buff
    DEBUFF,     // 给玩家增加Debuff
    SPECIAL     // 特殊行为
};

// 定义怪物行为结构体
struct MonsterAction {
    MonsterActionType type;  // 行为类型
    int value;               // 行为值(攻击值、防御值等)
    Effect::Type effectType; // 效果类型(用于BUFF/DEBUFF)
    int effectLevel;         // 效果等级
    int effectDuration;      // 效果持续时间
    std::string description; // 行为描述

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

// 怪物基础类
class Monster : public cocos2d::Sprite
{
public:
    // 工厂方法，用于创建怪物实例
    static Monster* createFactory(const std::string& monsterClassName);

    // 随机创建怪物（可指定是否为Boss或精英）
    static Monster* createRandom(bool isBoss = false, bool isElite = false);

    // 初始化怪物
    virtual bool init();
    virtual bool initWithSprite(const std::string& spritePath);

    // 设置和获取怪物的生命值
    void setHealth(int health);
    int getHealth() const;

    // 设置和获取怪物的防御值
    void setBlock(int block);
    int getBlock() const;

    // 设置和获取怪物的攻击力
    void setAttackDamage(int damage);
    int getAttackDamage() const;


    // 效果操作接口
    void addEffect(std::shared_ptr<Effect> effect);
    const std::vector<std::shared_ptr<Effect>>& getEffects() const;
    void updateEffects(); // 更新效果，移除持续时间为 0 的效果

    // 初始化怪物的行为模式 - 设置为纯虚函数，子类必须实现
    virtual void initActionPattern() = 0;

    // 获取当前回合的行为
    MonsterAction getCurrentAction();

    // 执行当前回合的行为
    virtual void executeCurrentAction(Hero* target);

    // 准备下一回合的行为(增加回合计数)
    void prepareNextTurn();

    // 获取下一回合行为的描述(用于UI显示)
    std::string getNextActionDescription() const;

    // 判断怪物类型
    virtual bool isBoss() const { return false; }
    virtual bool isElite() const { return false; }

    // 获取怪物名称
    virtual std::string getMonsterName() const { return "Unknown Monster"; }

protected:
    int _health;
    int _block;
    int _attackDamage;
    std::vector<std::shared_ptr<Effect>> _effects;
    std::vector<MonsterAction> _actionPattern;  // 行为模式列表
    int _currentTurn = 0;                       // 当前回合数

    static std::mt19937 _rng; // 随机数生成器
};

// 普通怪物基类
class NormalMonster : public Monster
{
public:
    virtual bool init() override;
    bool isBoss() const override { return false; }
    bool isElite() const override { return false; }

protected:
    // 普通怪物共有的一些特性可以在这里实现
};

// 精英怪物基类
class EliteMonster : public Monster
{
public:
    virtual bool init() override;
    bool isBoss() const override { return false; }
    bool isElite() const override { return true; }

protected:
    // 精英怪物共有的一些特性可以在这里实现
};

// Boss怪物基类
class BossMonster : public Monster
{
public:
    virtual bool init() override;
    bool isBoss() const override { return true; }
    bool isElite() const override { return false; }

protected:
    // Boss怪物共有的一些特性可以在这里实现
};

// 具体的普通怪物类
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

// 具体的精英怪物类
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

// 具体的Boss怪物类
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
