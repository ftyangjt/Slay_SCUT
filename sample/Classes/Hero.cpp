#include "Hero.h"

USING_NS_CC;

// ���һ����̬�������洢Ӣ�۵Ľ���ֵ
static int heroHealth = 100; // Ĭ������ֵΪ100

// ��������ʵ��
Hero* Hero::create(const std::string& filename)
{
    Hero* hero = new (std::nothrow) Hero();
    if (hero && hero->init(filename))
    {
        hero->autorelease();
        return hero;
    }
    CC_SAFE_DELETE(hero);
    return nullptr;
}

// ��ȡ��ǰ����ֵ
int Hero::getCurrentHealth()
{
    return heroHealth;
}

// �ָ�ָ������������ֵ������������
void Hero::healHealth(int amount)
{
    heroHealth = std::min(heroHealth + amount, MAX_HEALTH);
}

// ��ʼ������
bool Hero::init(const std::string& filename)
{
    if (!Sprite::initWithFile(filename))
    {
        return false;
    }

    // ʹ�ñ��������ֵ��ʼ������
    _health = heroHealth;

    // ����Ĭ�ϵĿ���
    createDefaultDeck();

    return true;
}

// �������ǵ�����ֵ
void Hero::setHealth(int health)
{
    _health = health;
    // ͬʱ���¾�̬����
    heroHealth = health;
}

// ��ȡ���ǵ�����ֵ
int Hero::getHealth() const
{
    return _health;
}

// ���ý���ֵ����������Ϸ���¿�ʼʱ���ã�
void Hero::resetHealth()
{
    heroHealth = 100;  // ֻ�����þ�̬����
    // ɾ���� _health ������
}

// �������ǵķ���ֵ
void Hero::setBlock(int block) {
    _block = block;
}

// ��ȡ���ǵķ���ֵ
int Hero::getBlock() const {
    return _block;
}

// ��ӿ��Ƶ�����
void Hero::addCardToDeck(const Card& card)
{
    _deck.push_back(card);
}

// ��ȡ����
const std::vector<Card>& Hero::getDeck() const
{
    return _deck;
}

// ��տ���
void Hero::clearDeck()
{
    _deck.clear();
}

// ��ʼ��Ĭ�Ͽ���
void Hero::createDefaultDeck()
{
    // ����ʾ������һ����ʼ���飬���忨�����Ը���ʵ���������
    clearDeck();

    // ���ʾ�����ƣ����� Card ���캯����Card(name, type, cost, effect, background, attack, defense)��
    addCardToDeck(Card("Strike", Card::Type::Attack, 1, "Deal 6 damage", "cardBackground.jpg", 6, 0));
    addCardToDeck(Card("Strike", Card::Type::Attack, 1, "Deal 6 damage", "cardBackground.jpg", 6, 0));
    addCardToDeck(Card("Strike", Card::Type::Attack, 1, "Deal 6 damage", "cardBackground.jpg", 6, 0));
    addCardToDeck(Card("Defend", Card::Type::Skill, 1, "Gain 5 Block", "cardBackground.jpg", 0, 5));
    addCardToDeck(Card("Defend", Card::Type::Skill, 1, "Gain 5 Block", "cardBackground.jpg", 0, 5));
    addCardToDeck(Card("Defend", Card::Type::Skill, 1, "Gain 5 Block", "cardBackground.jpg", 0, 5));

    // ���� Bash ���Ʋ��������Ч��
    Card bashCard("Bash", Card::Type::Attack, 2, "Deal 8 damage and apply Vulnerable", "cardBackground.jpg", 8, 0);
    // ʹ���·������Ч����ֱ��ָ��Ч�����͡��ȼ��ͳ���ʱ��
    bashCard.addEffect(Effect::Type::Vulnerable, 1, 3); // 1�����ˣ�����3�غ�
    addCardToDeck(bashCard);

    Card strengthCard("Strength", Card::Type::Power, 1, "Gain 2 Strength", "cardBackground.jpg");
    strengthCard.addEffect(Effect::Type::Strength, 2, -1); // 2������������ʱ��Ϊ����
    addCardToDeck(strengthCard);
}

//  ���Ч��
void Hero::addEffect(std::shared_ptr<Effect> effect) {
    if (!effect) {
        CCLOG("Error: Attempted to add a null effect.");
        return; // �������� effect �ǿ�ָ�룬ֱ�ӷ���
    }

    for (auto& existingEffect : _effects) {
        // ����Ƿ������ͬ���͵�Ч��
        if (existingEffect->getType() == effect->getType()) {
            if (effect->getType() == Effect::Type::Strength) {
                // ����Ч�������ӵȼ�
                existingEffect->setLevel(existingEffect->getLevel() + effect->getLevel());
            }
            else if (effect->getType() == Effect::Type::Vulnerable) {
                // ����Ч����ʱ������
                existingEffect->addRemainingTurns(effect->getRemainingTurns());
            }
            return; // ������ɺ�ֱ�ӷ���
        }
    }

    // ���û���ҵ���ͬ���͵�Ч����������µ�Ч��
    _effects.push_back(effect);
}

// ��ȡӵ�е�Ч��
const std::vector<std::shared_ptr<Effect>>& Hero::getEffects() const
{
    return _effects;
}

// ����Ч�����Ƴ�����ʱ��Ϊ 0 ��Ч��
void Hero::updateEffects() {
    for (auto it = _effects.begin(); it != _effects.end(); ) {
        (*it)->reduceTurn();
        if ((*it)->getRemainingTurns() == 0) {
            it = _effects.erase(it); // �Ƴ�����ʱ��Ϊ 0 ��Ч��
        }
        else {
            ++it;
        }
    }
}
