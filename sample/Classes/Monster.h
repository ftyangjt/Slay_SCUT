#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "cocos2d.h"
#include "Effect.h"

class Monster : public cocos2d::Sprite
{
public:
    // ��������ʵ��
    static Monster* create(const std::string& filename);

    // ��ʼ������
    bool init(const std::string& filename);

    // ���úͻ�ȡ���������ֵ
    void setHealth(int health);
    int getHealth() const;

	// ���úͻ�ȡ����ķ���ֵ
    void setBlock(int block);
    int getBlock() const;

	// Ч�������ӿ�
    void addEffect(std::shared_ptr<Effect> effect);
    const std::vector<std::shared_ptr<Effect>>& getEffects() const;
    void updateEffects(); // ����Ч�����Ƴ�����ʱ��Ϊ 0 ��Ч��

private:
    int _health;
    int _block;
    std::vector<std::shared_ptr<Effect>> _effects;
};

#endif // __MONSTER_H__
