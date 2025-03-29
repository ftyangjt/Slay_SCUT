#include "Hero.h"

USING_NS_CC;

// 创建主角实例
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

// 初始化主角
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

// 设置主角的生命值
void Hero::setHealth(int health)
{
    _health = health;
}

// 获取主角的生命值
int Hero::getHealth() const
{
    return _health;
}

// 设置主角的防御值
void Hero::setBlock(int block) {
    _block = block;
}

// 获取主角的防御值
int Hero::getBlock() const {
    return _block;
}

// 添加卡牌到卡组
void Hero::addCardToDeck(const Card& card)
{
    _deck.push_back(card);
}

// 获取卡组
const std::vector<Card>& Hero::getDeck() const
{
    return _deck;
}

// 清空卡组
void Hero::clearDeck()
{
    _deck.clear();
}

// 初始化默认卡组
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

// 添加效果
void Hero::addEffect(std::shared_ptr<Effect> effect)
{
    _effects.push_back(effect);
}

// 获取拥有的效果
const std::vector<std::shared_ptr<Effect>>& Hero::getEffects() const
{
    return _effects;
}
