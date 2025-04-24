#ifndef __CARD_H__
#define __CARD_H__

#include "cocos2d.h"
#include "Effect.h"

// 定义存储效果信息的结构体
struct EffectInfo {
    Effect::Type type;       // 效果类型
    int level;               // 效果等级
    int duration;            // 效果持续时间

    EffectInfo(Effect::Type t, int lvl, int dur) :
        type(t), level(lvl), duration(dur) {
    }
};

class Card
{
public:
    enum class Type
    {
        Attack,
        Skill,
        Power
    };

    enum class SpecialEffect {
        None,
        DrawCard,
        GainEnergy,
        DiscardCard,
        LoseHealth,
        // 可以添加更多特殊效果
    };

    // 添加获取特殊效果的方法
    SpecialEffect getSpecialEffect() const;

    // 添加特殊效果数值的getter，如抽几张牌等
    int getSpecialEffectValue() const;

    // 设置特殊效果
    void setSpecialEffect(SpecialEffect effect, int value = 1);

    Card(const std::string& name, Type type, int cost, const std::string& effect, const std::string& backgroundFile, int attack = 0, int defense = 0);
    ~Card();

    // 获取卡牌名称
    const std::string& getName() const;

    // 获取卡牌类型
    Type getType() const;

    // 获取卡牌能量消耗
    int getCost() const;

    // 获取卡牌效果
    const std::string& getEffect() const;

    // 获取卡牌攻击力
    int getAttack() const;

    // 获取卡牌防御力
    int getBlock() const;

    // 生成带有卡牌效果的背景图像
    cocos2d::Sprite* createCardSprite();

    // 获取卡牌精灵
    cocos2d::Sprite* getSprite() const;

    // 添加效果 - 现在接受效果类型、等级和时长
    void addEffect(Effect::Type type, int level, int duration = -1);

    // 获取效果列表
    const std::vector<EffectInfo>& getEffectInfos() const;

    // 创建并获取效果对象
    std::vector<std::shared_ptr<Effect>> createEffects() const;

    int getSpecialEffectValue(SpecialEffect effect) const;
    bool hasSpecialEffect(SpecialEffect effect) const;


private:
    std::string _name;
    Type _type;
    int _cost;
    std::string _effect;
    std::string _backgroundFile;
    int _attack;
    int _block;
    cocos2d::Sprite* _sprite; // 添加卡牌精灵成员变量
    std::vector<EffectInfo> _effectInfos; // 使用新的结构体存储效果信息
    std::vector<std::pair<SpecialEffect, int>> _specialEffects;

};

#endif // __CARD_H__