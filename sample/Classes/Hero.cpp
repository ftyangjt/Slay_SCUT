#include "Hero.h"

USING_NS_CC;

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

bool Hero::init(const std::string& filename)
{
    if (!Sprite::initWithFile(filename))
    {
        return false;
    }

    // 初始化主角的属性
    _health = 100; // 默认生命值

    // 创建默认的卡组
    createDefaultDeck();

    return true;
}

void Hero::setHealth(int health)
{
    _health = health;
}

int Hero::getHealth() const
{
    return _health;
}

void Hero::setBlock(int block) {
    _block = block;
}

int Hero::getBlock() const {
    return _block;
}

void Hero::addCardToDeck(const Card& card)
{
    _deck.push_back(card);
}

const std::vector<Card>& Hero::getDeck() const
{
    return _deck;
}

void Hero::clearDeck()
{
    _deck.clear();
}

void Hero::createDefaultDeck()
{
    // 这里示例创建一个初始卡组，具体卡牌属性根据实际需求调整
    clearDeck();

    // 添加示例卡牌（假设 Card 构造函数：Card(name, type, cost, effect, background, attack, defense)）
    addCardToDeck(Card("Strike", Card::Type::Attack, 1, "Deal 6 damage", "cardBackground.jpg", 6, 0));
    addCardToDeck(Card("Strike", Card::Type::Attack, 1, "Deal 6 damage", "cardBackground.jpg", 6, 0));
    addCardToDeck(Card("Strike", Card::Type::Attack, 1, "Deal 6 damage", "cardBackground.jpg", 6, 0));
    addCardToDeck(Card("Defend", Card::Type::Skill, 1, "Gain 5 Block", "cardBackground.jpg", 0, 5));
    addCardToDeck(Card("Defend", Card::Type::Skill, 1, "Gain 5 Block", "cardBackground.jpg", 0, 5));
    addCardToDeck(Card("Defend", Card::Type::Skill, 1, "Gain 5 Block", "cardBackground.jpg", 0, 5));
    addCardToDeck(Card("Bash", Card::Type::Attack, 2, "Deal 8 damage and apply Vulnerable", "cardBackground.jpg", 8, 0));
}
