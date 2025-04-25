#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "cocos2d.h"
#include "Effect.h"
#include <unordered_map>
#include <functional>
#include <random>
#include "Hero.h"
// ���������Ϊ����
enum class MonsterActionType {
    ATTACK,     // ����
    DEFEND,     // ����
    BUFF,       // ���Լ�����Buff
    DEBUFF,     // ���������Debuff
    SPECIAL     // ������Ϊ
};

// ���������Ϊ�ṹ��
struct MonsterAction {
    MonsterActionType type;  // ��Ϊ����
    int value;              // ��Ϊֵ(����ֵ������ֵ��)
    Effect::Type effectType; // Ч������(����BUFF/DEBUFF)
    int effectLevel;         // Ч���ȼ�
    int effectDuration;      // Ч������ʱ��
    std::string description; // ��Ϊ����

    MonsterAction(
        MonsterActionType _type = MonsterActionType::ATTACK,
        int _value = 0,
        Effect::Type _effectType = Effect::Type::Strength,
        int _effectLevel = 0,
        int _effectDuration = 0,
        const std::string& _desc = ""
    ) : type(_type), value(_value), effectType(_effectType),
        effectLevel(_effectLevel), effectDuration(_effectDuration),
        description(_desc) {
    }
};

// ��������ö��
enum class MonsterType {
    SLIME,          // ʷ��ķ
    GOBLIN,         // �粼��
    SKELETON,       // ����
    BAT,            // ����
    SNAKE,          // ��
    GHOST,          // ����
    ORC,            // ����
    GOLEM,          // ħ��
    DRAGON,         // ��
    BOSS_KNIGHT,    // Boss: �ڰ���ʿ
    BOSS_WIZARD,    // Boss: а��ʦ
    BOSS_DRAGON     // Boss: Զ�ž���
};

// �������Խṹ��
struct MonsterData {
    std::string name;           // ��������
    std::string sprite;         // ����ͼ���ļ���
    int baseHealth;             // ��������ֵ
    int baseAttackDamage;       // ���������˺�
    int baseBlockAmount;        // ������ֵ
    std::vector<Effect::Type> possibleEffects;  // ���ܵ�Ч������
    bool isBoss;                // �Ƿ�ΪBoss
    bool isElite;               // �Ƿ�Ϊ��Ӣ��

    // ���캯��
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
    // ��������ʵ�� - ����²���
    static Monster* create(const std::string& filename = "");

    // ʹ��ָ�����ʹ�������
    static Monster* createWithType(MonsterType type);

    // ������������ָ���Ƿ�ΪBoss��Ӣ��
    static Monster* createRandom(bool isBoss = false, bool isElite = false);

    // ��ʼ������
    bool init(const std::string& filename);
    bool initWithMonsterData(const MonsterData& data);

    // ���úͻ�ȡ���������ֵ
    void setHealth(int health);
    int getHealth() const;

    // ���úͻ�ȡ����ķ���ֵ
    void setBlock(int block);
    int getBlock() const;

    // ���úͻ�ȡ����Ĺ�����
    void setAttackDamage(int damage);
    int getAttackDamage() const;

    // ���úͻ�ȡ��������
    void setMonsterType(MonsterType type);
    MonsterType getMonsterType() const;

    // ��ȡ��������
    const MonsterData& getMonsterData() const;

    // Ӧ�����Ч��
    void applyRandomEffect();

    // Ч�������ӿ�
    void addEffect(std::shared_ptr<Effect> effect);
    const std::vector<std::shared_ptr<Effect>>& getEffects() const;
    void updateEffects(); // ����Ч�����Ƴ�����ʱ��Ϊ 0 ��Ч��

    // ��ʼ���������Ϊģʽ
    void initActionPattern();

    // ��ȡ��ǰ�غϵ���Ϊ
    MonsterAction getCurrentAction();

    // ִ�е�ǰ�غϵ���Ϊ
    void executeCurrentAction(Hero* target);

    // ׼����һ�غϵ���Ϊ(���ӻغϼ���)
    void prepareNextTurn();

    // ��ȡ��һ�غ���Ϊ������(����UI��ʾ)
    std::string getNextActionDescription() const;

private:
    int _health;
    int _block;
    int _attackDamage;
    MonsterType _type;
    MonsterData _data;
    std::vector<std::shared_ptr<Effect>> _effects;

    // ����� - ����ģʽ
    static std::unordered_map<MonsterType, MonsterData> _monsterLibrary;

    // ��ʼ�������
    static void initMonsterLibrary();

    // �����������
    static std::mt19937 _rng;


    std::vector<MonsterAction> _actionPattern;  // ��Ϊģʽ�б�
    int _currentTurn = 0;                      // ��ǰ�غ���(����ѭ����Ϊģʽ)
};

#endif // __MONSTER_H__
