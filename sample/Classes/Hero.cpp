// Hero.cpp
#include "Hero.h"
#include "CardLibrary.h"

USING_NS_CC;

// 添加静态成员变量存储英雄的生命值和金币
static int heroHealth = 100; // 默认生命值为100
static int heroCoins = 100; // 默认金币为100
// 添加一个静态变量存储英雄的最大生命值上限
static int heroMaxHealth = Hero::MAX_HEALTH;
static bool _isDeckInitialized = false; // 卡组初始化标志
std::vector<Card> Hero::_deck; // 卡组静态变量

// 获取当前最大生命值上限
int Hero::getMaxHealth()
{
    return heroMaxHealth;
}
// 在Hero.cpp中实现这个静态方法
void Hero::addCardToDeckStatic(const Card& card)
{
    _deck.push_back(card);
}

void Hero::clearDeckStatic()
{
    _deck.clear();
}

// 增加最大生命值上限
void Hero::increaseMaxHealth(int amount)
{
    heroMaxHealth += amount;
    // 同时增加当前生命值
    healHealth(amount);
    updateStatusDisplayStatic(); // 更新显示
}

// 初始化静态标签
cocos2d::Label* Hero::s_statusLabel = nullptr;

// 创建英雄实例
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

// 获取当前生命值
int Hero::getCurrentHealth()
{
    return heroHealth;
}

// 恢复指定数量的生命值（不会超过上限）
void Hero::healHealth(int amount)
{
    // 修改为使用动态的 heroMaxHealth 而不是静态常量 MAX_HEALTH
    heroHealth = std::min(heroHealth + amount, heroMaxHealth);
    updateStatusDisplayStatic(); // 更新显示
}

// 获取当前金币数
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

// 添加金币
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
    // 释放已经存在的标签
    if (s_statusLabel != nullptr) {
        if (s_statusLabel->getParent()) {
            s_statusLabel->removeFromParent();
        }
        s_statusLabel->release(); // 释放之前的retain
        s_statusLabel = nullptr;
    }

    // 创建状态显示标签，修改显示格式为当前生命值/最大生命值
    s_statusLabel = Label::createWithTTF("Coins: " + std::to_string(heroCoins) +
        " Health: " + std::to_string(heroHealth) + "/" + std::to_string(heroMaxHealth),
        "fonts/Marker Felt.ttf", 24);
    if (s_statusLabel == nullptr) {
        CCLOG("Error: Failed to create status label");
        return;
    }

    s_statusLabel->setAnchorPoint(Vec2(0, 1)); // 左上角对齐

    // 获取当前运行场景和可见区域大小
    Scene* runningScene = Director::getInstance()->getRunningScene();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 设置标签位置在左上角
    s_statusLabel->setPosition(Vec2(origin.x + 10, origin.y + visibleSize.height - 10));
    s_statusLabel->setTextColor(Color4B::WHITE); // 设置标签颜色为白色

    // 为标签添加黑色边框，提高可读性
    s_statusLabel->enableOutline(Color4B::BLACK, 1);

    // 将标签添加到当前运行场景
    if (runningScene) {
        runningScene->addChild(s_statusLabel, 100); // 使用较高的Z顺序确保显示在最前
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

    // 更新标签文本，修改显示格式为当前生命值/最大生命值
    s_statusLabel->setString("Coins: " + std::to_string(heroCoins) +
        " Health: " + std::to_string(heroHealth) + "/" + std::to_string(heroMaxHealth));

    // 检查标签是否在当前场景中
    Scene* currentScene = Director::getInstance()->getRunningScene();
    if (currentScene && !s_statusLabel->getParent()) {
        s_statusLabel->removeFromParent(); // 确保标签从之前的父节点移除
        currentScene->addChild(s_statusLabel, 100); // 添加到当前场景
    }
}



// 初始化英雄
bool Hero::init(const std::string& filename)
{
    if (!Sprite::initWithFile(filename))
    {
        return false;
    }

    // 使用角色生命值初始化成员
    _health = heroHealth;

    if (_deck.empty()) {
        createDefaultDeck();
    }

    // 初始化英雄实例状态标签
    initStatusDisplay();
    _statusLabel = s_statusLabel;

    return true;
}

// 设置英雄的生命值
void Hero::setHealth(int health)
{
    _health = health;
    // 同时更新静态变量
    heroHealth = health;
    updateStatusDisplayStatic(); // 使用静态方法更新
}

// 获取英雄的生命值
int Hero::getHealth() const
{
    return _health;
}

// 设置生命值（常用于游戏重新开始时调用）
void Hero::resetHealth()
{
    // 修改为使用heroMaxHealth而非MAX_HEALTH
    heroHealth = heroMaxHealth;  // 重置为当前最大生命值
    updateStatusDisplayStatic(); // 使用静态方法更新
}

// 设置英雄的格挡值
void Hero::setBlock(int block) {
    _block = block;
}

// 获取英雄的格挡值
int Hero::getBlock() const {
    return _block;
}

// 添加卡牌到卡组
void Hero::addCardToDeck(const Card& card)
{
    _deck.push_back(card);
}

// 获取卡组
const std::vector<Card>& Hero::getDeck()
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
    if (_isDeckInitialized) {
        CCLOG("Deck is already initialized. Skipping initialization.");
        return;
    }
    // 以下示例创建一个初始卡组，具体卡牌可根据实际情况调整
    clearDeck();
    _isDeckInitialized = true;
    _deck = CardLibrary::getStarterDeck();
}

// 添加效果
void Hero::addEffect(std::shared_ptr<Effect> effect) {
    if (!effect) {
        CCLOG("Error: Attempted to add a null effect.");
        return; // 如果传入的 effect 是空指针，直接返回
    }

    for (auto& existingEffect : _effects) {
        // 检查是否存在相同类型的效果
        if (existingEffect->getType() == effect->getType()) {
            if (effect->getType() == Effect::Type::Strength) {
                // 如果效果是力量，加等级
                existingEffect->setLevel(existingEffect->getLevel() + effect->getLevel());
            }
            else if (effect->getType() == Effect::Type::Vulnerable) {
                // 如果效果是易伤，加时间
                existingEffect->addRemainingTurns(effect->getRemainingTurns());
            }
            return; // 处理完成后直接返回
        }
    }

    // 如果没有找到相同类型的效果，添加新的效果
    _effects.push_back(effect);
}

// 获取拥有的效果
const std::vector<std::shared_ptr<Effect>>& Hero::getEffects() const
{
    return _effects;
}

// 更新效果并移除持续时间为 0 的效果
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
    updateStatusDisplay(); // 调用实例方法的调用，方便扩展
}

// 更新状态显示 (实例方法)
void Hero::updateStatusDisplay() {
    updateStatusDisplayStatic(); // 简单地调用静态方法
}

bool Hero::isDeckInitialized() const {
    return _isDeckInitialized;
}

// 设置卡组初始化状态
void Hero::setDeckInitialized(bool initialized) {
    _isDeckInitialized = initialized;
}
