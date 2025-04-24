#ifndef __CARD_H__
#define __CARD_H__

#include "cocos2d.h"
#include "Effect.h"

// ����洢Ч����Ϣ�Ľṹ��
struct EffectInfo {
    Effect::Type type;       // Ч������
    int level;               // Ч���ȼ�
    int duration;            // Ч������ʱ��

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
        // ������Ӹ�������Ч��
    };

    // ��ӻ�ȡ����Ч���ķ���
    SpecialEffect getSpecialEffect() const;

    // �������Ч����ֵ��getter����鼸���Ƶ�
    int getSpecialEffectValue() const;

    // ��������Ч��
    void setSpecialEffect(SpecialEffect effect, int value = 1);

    Card(const std::string& name, Type type, int cost, const std::string& effect, const std::string& backgroundFile, int attack = 0, int defense = 0);
    ~Card();

    // ��ȡ��������
    const std::string& getName() const;

    // ��ȡ��������
    Type getType() const;

    // ��ȡ������������
    int getCost() const;

    // ��ȡ����Ч��
    const std::string& getEffect() const;

    // ��ȡ���ƹ�����
    int getAttack() const;

    // ��ȡ���Ʒ�����
    int getBlock() const;

    // ���ɴ��п���Ч���ı���ͼ��
    cocos2d::Sprite* createCardSprite();

    // ��ȡ���ƾ���
    cocos2d::Sprite* getSprite() const;

    // ���Ч�� - ���ڽ���Ч�����͡��ȼ���ʱ��
    void addEffect(Effect::Type type, int level, int duration = -1);

    // ��ȡЧ���б�
    const std::vector<EffectInfo>& getEffectInfos() const;

    // ��������ȡЧ������
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
    cocos2d::Sprite* _sprite; // ��ӿ��ƾ����Ա����
    std::vector<EffectInfo> _effectInfos; // ʹ���µĽṹ��洢Ч����Ϣ
    std::vector<std::pair<SpecialEffect, int>> _specialEffects;

};

#endif // __CARD_H__