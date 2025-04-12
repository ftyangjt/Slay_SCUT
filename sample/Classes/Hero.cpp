#include "Hero.h"

USING_NS_CC;

// 添加一个静态变量来存储英雄的健康值
static int heroHealth = 100; // 默认生命值为100

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

// 获取当前健康值
int Hero::getCurrentHealth()
{
    return heroHealth;
}

// 恢复指定数量的生命值，不超过上限
void Hero::healHealth(int amount)
{
    heroHealth = std::min(heroHealth + amount, MAX_HEALTH);
}

// 初始化主角
bool Hero::init(const std::string& filename)
{
    if (!Sprite::initWithFile(filename))
    {
        return false;
    }

    // 使用保存的生命值初始化主角
    _health = heroHealth;

    // 创建默认的卡组
    createDefaultDeck();

    return true;
}

// 设置主角的生命值
void Hero::setHealth(int health)
{
    _health = health;
    // 同时更新静态变量
    heroHealth = health;
}

// 获取主角的生命值
int Hero::getHealth() const
{
    return _health;
}

// 重置健康值（可以在游戏重新开始时调用）
void Hero::resetHealth()
{
    heroHealth = 100;  // 只需重置静态变量
    // 删除对 _health 的引用
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

    // 创建 Bash 卡牌并添加易伤效果
    Card bashCard("Bash", Card::Type::Attack, 2, "Deal 8 damage and apply Vulnerable", "cardBackground.jpg", 8, 0);
    // 使用新方法添加效果：直接指定效果类型、等级和持续时间
    bashCard.addEffect(Effect::Type::Vulnerable, 1, 3); // 1级易伤，持续3回合
    addCardToDeck(bashCard);

    Card strengthCard("Strength", Card::Type::Power, 1, "Gain 2 Strength", "cardBackground.jpg");
    strengthCard.addEffect(Effect::Type::Strength, 2, -1); // 2级力量，持续时间为永久
    addCardToDeck(strengthCard);
}

//  添加效果
void Hero::addEffect(std::shared_ptr<Effect> effect) {
    if (!effect) {
        CCLOG("Error: Attempted to add a null effect.");
        return; // 如果传入的 effect 是空指针，直接返回
    }

    for (auto& existingEffect : _effects) {
        // 检查是否存在相同类型的效果
        if (existingEffect->getType() == effect->getType()) {
            if (effect->getType() == Effect::Type::Strength) {
                // 力量效果：叠加等级
                existingEffect->setLevel(existingEffect->getLevel() + effect->getLevel());
            }
            else if (effect->getType() == Effect::Type::Vulnerable) {
                // 易伤效果：时长叠加
                existingEffect->addRemainingTurns(effect->getRemainingTurns());
            }
            return; // 处理完成后直接返回
        }
    }

    // 如果没有找到相同类型的效果，则添加新的效果
    _effects.push_back(effect);
}

// 获取拥有的效果
const std::vector<std::shared_ptr<Effect>>& Hero::getEffects() const
{
    return _effects;
}

// 更新效果，移除持续时间为 0 的效果
void Hero::updateEffects() {
    for (auto it = _effects.begin(); it != _effects.end(); ) {
        (*it)->reduceTurn();
        if ((*it)->getRemainingTurns() == 0) {
            it = _effects.erase(it); // 移除持续时间为 0 的效果
        }
        else {
            ++it;
        }
    }
}
