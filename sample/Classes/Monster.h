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
    int value;              // 行为值(攻击值、防御值等)
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

// 怪物类型枚举
enum class MonsterType {
    SLIME,          // 史莱姆
    GOBLIN,         // 哥布林
    SKELETON,       // 骷髅
    BAT,            // 蝙蝠
    SNAKE,          // 蛇
    GHOST,          // 幽灵
    ORC,            // 兽人
    GOLEM,          // 魔像
    DRAGON,         // 龙
    BOSS_KNIGHT,    // Boss: 黑暗骑士
    BOSS_WIZARD,    // Boss: 邪恶法师
    BOSS_DRAGON     // Boss: 远古巨龙
};

// 怪物属性结构体
struct MonsterData {
    std::string name;           // 怪物名称
    std::string sprite;         // 怪物图像文件名
    int baseHealth;             // 基础生命值
    int baseAttackDamage;       // 基础攻击伤害
    int baseBlockAmount;        // 基础格挡值
    std::vector<Effect::Type> possibleEffects;  // 可能的效果类型
    bool isBoss;                // 是否为Boss
    bool isElite;               // 是否为精英怪

    // 构造函数
    MonsterData(
        const std::string& _name = "",
        const std::string& _sprite = "",
        int _health = 0,
        int _attack = 0,
        int _block = 0,
        const std::vector<Effect::Type>& _effects = {},
        bool _isBoss = false,
        bool _isElite = false
    ) : name(_name), sprite(_sprite), baseHealth(_health),
        baseAttackDamage(_attack), baseBlockAmount(_block),
        possibleEffects(_effects), isBoss(_isBoss), isElite(_isElite) {
    }
};

class Monster : public cocos2d::Sprite
{
public:
    // 创建怪物实例 - 添加新参数
    static Monster* create(const std::string& filename = "");

    // 使用指定类型创建怪物
    static Monster* createWithType(MonsterType type);

    // 随机创建怪物（可指定是否为Boss或精英）
    static Monster* createRandom(bool isBoss = false, bool isElite = false);

    // 初始化怪物
    bool init(const std::string& filename);
    bool initWithMonsterData(const MonsterData& data);

    // 设置和获取怪物的生命值
    void setHealth(int health);
    int getHealth() const;

    // 设置和获取怪物的防御值
    void setBlock(int block);
    int getBlock() const;

    // 设置和获取怪物的攻击力
    void setAttackDamage(int damage);
    int getAttackDamage() const;

    // 设置和获取怪物类型
    void setMonsterType(MonsterType type);
    MonsterType getMonsterType() const;

    // 获取怪物数据
    const MonsterData& getMonsterData() const;

    // 应用随机效果
    void applyRandomEffect();

    // 效果操作接口
    void addEffect(std::shared_ptr<Effect> effect);
    const std::vector<std::shared_ptr<Effect>>& getEffects() const;
    void updateEffects(); // 更新效果，移除持续时间为 0 的效果

    // 初始化怪物的行为模式
    void initActionPattern();

    // 获取当前回合的行为
    MonsterAction getCurrentAction();

    // 执行当前回合的行为
    void executeCurrentAction(Hero* target);

    // 准备下一回合的行为(增加回合计数)
    void prepareNextTurn();

    // 获取下一回合行为的描述(用于UI显示)
    std::string getNextActionDescription() const;

private:
    int _health;
    int _block;
    int _attackDamage;
    MonsterType _type;
    MonsterData _data;
    std::vector<std::shared_ptr<Effect>> _effects;

    // 怪物库 - 单例模式
    static std::unordered_map<MonsterType, MonsterData> _monsterLibrary;

    // 初始化怪物库
    static void initMonsterLibrary();

    // 随机数生成器
    static std::mt19937 _rng;


    std::vector<MonsterAction> _actionPattern;  // 行为模式列表
    int _currentTurn = 0;                      // 当前回合数(用于循环行为模式)
};

#endif // __MONSTER_H__
