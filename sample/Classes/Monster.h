#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "cocos2d.h"

class Monster : public cocos2d::Sprite
{
public:
    // 创建怪物实例
    static Monster* create(const std::string& filename);

    // 初始化怪物
    bool init(const std::string& filename);

    // 设置和获取怪物的生命值
    void setHealth(int health);
    int getHealth() const;

	// 设置和获取怪物的防御值
    void setBlock(int block);
    int getBlock() const;

private:
    int _health;
    int _block;
};

#endif // __MONSTER_H__
