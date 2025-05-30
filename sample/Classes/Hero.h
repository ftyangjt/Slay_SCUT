#ifndef __HERO_H__
#define __HERO_H__

#include "cocos2d.h"
#include "Card.h"
#include "Effect.h"

class Hero : public cocos2d::Sprite
{
public:
    // Hero.h ������
    static void increaseMaxHealth(int amount); // �����������ֵ����
    static int getMaxHealth(); // ��ȡ��ǰ�������ֵ����

    // �������
    static int getCurrentHealth(); // ��ȡ��ǰ����ֵ
    static void healHealth(int amount); // �ָ�ָ������������ֵ������������
    static const int MAX_HEALTH = 100; // �������ֵ
    static void resetHealth(); // ���ý���ֵ

    // ������
    static int getCoins(); // ��ȡ��ǰ�������
    static void setCoins(int coins); // ���ý������
    static void addCoins(int amount); // ���ӽ��
    static void resetCoins(); // ���ý��
    static const int INITIAL_COINS = 100; // ��ʼ�������

    // ״̬��ʾ��� - ���Ӿ�̬����
    static void updateStatusDisplayStatic(); // ��̬��������״̬��ʾ
    static void initStatusDisplay(); // ��ʼ��״̬��ʾ

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
    static const std::vector<Card>& getDeck();
    void clearDeck();
    // ��ʼ��Ĭ�Ͽ���
    void createDefaultDeck();

    // Ч�������ӿ�
    void addEffect(std::shared_ptr<Effect> effect);
    const std::vector<std::shared_ptr<Effect>>& getEffects() const;
    void updateEffects(); // ����Ч�����Ƴ�����ʱ��Ϊ 0 ��Ч��

    // ʵ������״̬��ʾ
    void updateStatusDisplay();

	// �жϿ����Ƿ��ѳ�ʼ��
    bool isDeckInitialized() const; // �жϿ����Ƿ��ѳ�ʼ��
   static void setDeckInitialized(bool initialized); // ���ÿ����ʼ��״̬

    static void clearDeckStatic() {
        _deck.clear();
    }

    static void addCardToDeckStatic(const Card& card) {
        _deck.push_back(card);
    }

private:
    int _health; // ����ֵ
    int _block; // ����ֵ
    static std::vector<Card> _deck; // ����
    std::vector<std::shared_ptr<Effect>> _effects; // Ч��
    cocos2d::Label* _statusLabel; // ״̬��ʾ��ǩ

    // ���Ӿ�̬��Ա
    static cocos2d::Label* s_statusLabel; // ��̬״̬��ʾ��ǩ
};

#endif // __HERO_H__
