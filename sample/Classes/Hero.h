#ifndef __HERO_H__
#define __HERO_H__

#include "cocos2d.h"
#include "Card.h"
#include "Effect.h"

class Hero : public cocos2d::Sprite
{
public:
    // Hero.h 中添加
    static void increaseMaxHealth(int amount); // 增加最大生命值上限
    static int getMaxHealth(); // 获取当前最大生命值上限

    // 健康相关
    static int getCurrentHealth(); // 获取当前健康值
    static void healHealth(int amount); // 恢复指定数量的生命值，不超过上限
    static const int MAX_HEALTH = 100; // 最大生命值
    static void resetHealth(); // 重置健康值

    // 金币相关
    static int getCoins(); // 获取当前金币数量
    static void setCoins(int coins); // 设置金币数量
    static void addCoins(int amount); // 增加金币
    static void resetCoins(); // 重置金币
    static const int INITIAL_COINS = 100; // 初始金币数量

    // 状态显示相关 - 添加静态方法
    static void updateStatusDisplayStatic(); // 静态方法更新状态显示
    static void initStatusDisplay(); // 初始化状态显示

    // 创建主角实例
    static Hero* create(const std::string& filename);

    // 初始化主角
    bool init(const std::string& filename);

    // 设置和获取主角的生命值
    void setHealth(int health);
    int getHealth() const;

    // 设置和获取主角的防御值
    void setBlock(int block);
    int getBlock() const;

    // 卡组操作接口
    void addCardToDeck(const Card& card);
    static const std::vector<Card>& getDeck();
    void clearDeck();
    // 初始化默认卡组
    void createDefaultDeck();

    // 效果操作接口
    void addEffect(std::shared_ptr<Effect> effect);
    const std::vector<std::shared_ptr<Effect>>& getEffects() const;
    void updateEffects(); // 更新效果，移除持续时间为 0 的效果

    // 实例方法状态显示
    void updateStatusDisplay();

	// 判断卡组是否已初始化
    bool isDeckInitialized() const; // 判断卡组是否已初始化
   static void setDeckInitialized(bool initialized); // 设置卡组初始化状态

    static void clearDeckStatic() {
        _deck.clear();
    }

    static void addCardToDeckStatic(const Card& card) {
        _deck.push_back(card);
    }

private:
    int _health; // 生命值
    int _block; // 防御值
    static std::vector<Card> _deck; // 卡组
    std::vector<std::shared_ptr<Effect>> _effects; // 效果
    cocos2d::Label* _statusLabel; // 状态显示标签

    // 添加静态成员
    static cocos2d::Label* s_statusLabel; // 静态状态显示标签
};

#endif // __HERO_H__
