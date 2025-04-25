// Hero.cpp
#include "Hero.h"

USING_NS_CC;

// 添加静态变量来存储英雄的健康值和金币
static int heroHealth = 100; // 默认生命值为100
static int heroCoins = 100; // 默认金币为100

// 初始化静态标签
cocos2d::Label* Hero::s_statusLabel = nullptr;

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
    updateStatusDisplayStatic(); // 更新显示
}

// 获取当前金币数量
int Hero::getCoins()
{
    return heroCoins;
}

// 设置金币数量
void Hero::setCoins(int coins)
{
    heroCoins = coins;
    updateStatusDisplayStatic(); // 更新显示
}

// 增加金币
void Hero::addCoins(int amount)
{
    heroCoins += amount;
    updateStatusDisplayStatic(); // 更新显示
}

// 重置金币
void Hero::resetCoins()
{
    heroCoins = INITIAL_COINS;
    updateStatusDisplayStatic(); // 更新显示
}

// 初始化状态显示

void Hero::initStatusDisplay()
{
    // 先释放可能已经存在的标签
    if (s_statusLabel != nullptr) {
        if (s_statusLabel->getParent()) {
            s_statusLabel->removeFromParent();
        }
        s_statusLabel->release(); // 释放之前的retain
        s_statusLabel = nullptr;
    }

    // 创建状态显示标签
    s_statusLabel = Label::createWithTTF("Coins: " + std::to_string(heroCoins) +
        " Health: " + std::to_string(heroHealth),
        "fonts/Marker Felt.ttf", 24);
    if (s_statusLabel == nullptr) {
        CCLOG("Error: Failed to create status label");
        return;
    }

    s_statusLabel->setAnchorPoint(Vec2(0, 1)); // 左上角对齐

    // 获取当前运行场景和可视区域大小
    Scene* runningScene = Director::getInstance()->getRunningScene();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 设置标签位置在左上角
    s_statusLabel->setPosition(Vec2(origin.x + 10, origin.y + visibleSize.height - 10));
    s_statusLabel->setTextColor(Color4B::WHITE); // 设置标签颜色为白色

    // 为标签添加黑色描边，提高可读性
    s_statusLabel->enableOutline(Color4B::BLACK, 1);

    // 将标签添加到当前运行场景
    if (runningScene) {
        runningScene->addChild(s_statusLabel, 100); // 使用极高的Z顺序确保显示在最前
    }

    // 添加场景切换监听器
    Director::getInstance()->getEventDispatcher()->addCustomEventListener(
        Director::EVENT_AFTER_SET_NEXT_SCENE,
        [](EventCustom* event) {
            Hero::updateStatusDisplayStatic(); // 场景切换后更新状态显示
        }
    );

    s_statusLabel->retain(); // 保留标签以防场景切换时被自动释放
}


void Hero::updateStatusDisplayStatic()
{
    // 如果标签不存在，则创建
    if (s_statusLabel == nullptr) {
        initStatusDisplay();
        return;
    }

    // 更新标签文本
    s_statusLabel->setString("Coins: " + std::to_string(heroCoins) +
        " Health: " + std::to_string(heroHealth));

    // 检查标签是否在当前场景中
    Scene* currentScene = Director::getInstance()->getRunningScene();
    if (currentScene && !s_statusLabel->getParent()) {
        s_statusLabel->removeFromParent(); // 确保标签从之前的父节点中移除
        currentScene->addChild(s_statusLabel, 100); // 添加到当前场景
    }
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

    // 初始化并关联实例状态标签
    initStatusDisplay();
    _statusLabel = s_statusLabel;

    return true;
}

// 设置主角的生命值
void Hero::setHealth(int health)
{
    _health = health;
    // 同时更新静态变量
    heroHealth = health;
    updateStatusDisplayStatic(); // 使用静态方法更新
}

// 获取主角的生命值
int Hero::getHealth() const
{
    return _health;
}

// 重置健康值（可以在游戏重新开始时调用）
void Hero::resetHealth()
{
    heroHealth = MAX_HEALTH;  // 重置静态变量
    updateStatusDisplayStatic(); // 使用静态方法更新
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
    updateStatusDisplay(); // 这是实例方法的调用，不会有错误
}

// 更新状态显示 (实例方法)
void Hero::updateStatusDisplay() {
    updateStatusDisplayStatic(); // 简单地调用静态方法
}
