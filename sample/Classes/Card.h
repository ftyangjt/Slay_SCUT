#ifndef __CARD_H__
#define __CARD_H__

#include "cocos2d.h"
#include "Effect.h"

class Card
{
public:
    enum class Type
    {
        Attack,
        Skill,
        Power
    };

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

    // 添加效果
    void addEffect(std::shared_ptr<Effect> effect);

    // 获取效果列表
    const std::vector<std::shared_ptr<Effect>>& getEffects() const;

private:
    std::string _name;
    Type _type;
    int _cost;
    std::string _effect;
    std::string _backgroundFile;
    int _attack;
    int _block;
    cocos2d::Sprite* _sprite; // 添加卡牌精灵成员变量
    std::vector<std::shared_ptr<Effect>> _effects; // 添加效果列表
};

#endif // __CARD_H__
