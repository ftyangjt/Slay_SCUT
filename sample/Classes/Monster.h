#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "cocos2d.h"
#include "Effect.h"
#include <unordered_map>
#include <functional>
#include <random>

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
};

#endif // __MONSTER_H__
