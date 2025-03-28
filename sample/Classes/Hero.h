#ifndef __HERO_H__
#define __HERO_H__

#include "cocos2d.h"
#include <vector>
#include "Card.h" // 假设 Card 类已经定义

class Hero : public cocos2d::Sprite
{
public:
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
    const std::vector<Card>& getDeck() const;
    void clearDeck();

private:
    int _health; // 生命值
	int _block; // 防御值
	std::vector<Card> _deck; // 卡组

    // 初始化默认卡组
    void createDefaultDeck();
};

#endif // __HERO_H__
