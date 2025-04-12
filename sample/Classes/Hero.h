#ifndef __HERO_H__
#define __HERO_H__

#include "cocos2d.h"
#include "Card.h"
#include "Effect.h"

class Hero : public cocos2d::Sprite
{
public:
    // ��ӵ� Hero ��Ĺ�������
    static int getCurrentHealth(); // ��ȡ��ǰ����ֵ
    static void healHealth(int amount); // �ָ�ָ������������ֵ������������
    static const int MAX_HEALTH = 100; // �������ֵ

    // ��Hero.h����ӣ�
    static void resetHealth(); // ���ý���ֵ

    // ��������ʵ��
    static Hero* create(const std::string& filename);

    // ��ʼ������
    bool init(const std::string& filename);

    // ���úͻ�ȡ���ǵ�����ֵ
    void setHealth(int health);
    int getHealth() const;

	// ���úͻ�ȡ���ǵķ���ֵ
    void setBlock(int block);
    int getBlock() const;

    // ��������ӿ�
    void addCardToDeck(const Card& card);
    const std::vector<Card>& getDeck() const;
    void clearDeck();
    // ��ʼ��Ĭ�Ͽ���
    void createDefaultDeck();

    // Ч�������ӿ�
    void addEffect(std::shared_ptr<Effect> effect);
    const std::vector<std::shared_ptr<Effect>>& getEffects() const;
    void updateEffects(); // ����Ч�����Ƴ�����ʱ��Ϊ 0 ��Ч��

private:
    int _health; // ����ֵ
	int _block; // ����ֵ
	std::vector<Card> _deck; // ����
    std::vector<std::shared_ptr<Effect>> _effects;
};

#endif // __HERO_H__
