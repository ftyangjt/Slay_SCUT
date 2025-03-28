#include "FightingScene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* FightingScene::createScene()
{
    return FightingScene::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in FightingScene.cpp\n");
}

bool FightingScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 初始化选中卡牌
    _selectedCard = nullptr;
    _selectedCardIndex = -1;

    // 创建背景
    createBackground();

    // 创建角色和怪物
    createCharacters();

	// 初始化抽牌堆和弃牌堆
    initializeDrawPile();

    // 创建血量标签
    _heroHealthLabel = Label::createWithTTF("Hero Health: 100", "fonts/Marker Felt.ttf", 60);
    _heroHealthLabel->setTextColor(Color4B::RED); // 设置标签颜色为红色
    _heroHealthLabel->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + visibleSize.height - _heroHealthLabel->getContentSize().height));
    this->addChild(_heroHealthLabel, 1);

    _monsterHealthLabel = Label::createWithTTF("Monster Health: 100", "fonts/Marker Felt.ttf", 60);
    _monsterHealthLabel->setTextColor(Color4B::RED); // 设置标签颜色为红色
    _monsterHealthLabel->setPosition(Vec2(origin.x + 3 * visibleSize.width / 4, origin.y + visibleSize.height - _monsterHealthLabel->getContentSize().height));
    this->addChild(_monsterHealthLabel, 1);

    startPlayerTurn();

    return true;
}

void FightingScene::createBackground()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto background = Sprite::create("background.png");
    if (background == nullptr)
    {
        problemLoading("'background.png'");
        return;
    }

    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(background, 0);
}

void FightingScene::createCharacters()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建英雄
    _hero = Hero::create("hero.png");
    if (_hero == nullptr)
    {
        problemLoading("'hero.png'");
        return;
    }

    _hero->setPosition(Vec2(origin.x + _hero->getContentSize().width / 2, origin.y + visibleSize.height / 2));
    this->addChild(_hero, 1);

    // 创建怪物
    _monster = Monster::create("monster.png");
    if (_monster == nullptr)
    {
        problemLoading("'monster.png'");
        return;
    }

    _monster->setPosition(Vec2(origin.x + visibleSize.width - _monster->getContentSize().width / 2, origin.y + visibleSize.height / 2));
    this->addChild(_monster, 1);
}

bool FightingScene::onCardTouchBegan(cocos2d::Sprite* cardSprite, int index, cocos2d::Touch* touch, cocos2d::Event* event)
{
    Vec2 touchLocation = touch->getLocation();
    if (cardSprite->getBoundingBox().containsPoint(touchLocation))
    {
        if (_selectedCard == cardSprite)
        {
            // 如果再次点击选中的卡牌，则打出卡牌
            playCard(index);
        }
        else
        {
            // 选中卡牌
            selectCard(cardSprite, index);
        }
        return true;
    }
    return false;
}

void FightingScene::selectCard(cocos2d::Sprite* card, int index)
{
    if (_selectedCard != nullptr)
    {
        // 取消之前选中的卡牌的选中状态
        _selectedCard->setColor(cocos2d::Color3B::WHITE);
    }

    // 设置当前选中的卡牌
    _selectedCard = card;
    _selectedCardIndex = index;

    // 设置选中卡牌的颜色为灰色
    _selectedCard->setColor(cocos2d::Color3B::GRAY);
}

void FightingScene::playCard(int index)
{
    // 打出卡牌的逻辑
    CCLOG("Play Card: %s", _cards[index].getName().c_str());
    CCLOG("Card Effect: %s", _cards[index].getEffect().c_str());

    // 根据卡牌效果对怪物造成伤害
    if (_cards[index].getType() == Card::Type::Attack)
    {
        int damage = _cards[index].getAttack();
        int newHealth = _monster->getHealth() - damage;
        _monster->setHealth(newHealth);
        CCLOG("Monster Health: %d", _monster->getHealth());
    }

    // 更新血量标签
    updateHealthLabels();

    discardCard(index);
}

void FightingScene::updateHealthLabels()
{
    _heroHealthLabel->setString("Hero Health: " + std::to_string(_hero->getHealth()));
    _monsterHealthLabel->setString("Monster Health: " + std::to_string(_monster->getHealth()));
}

void FightingScene::startPlayerTurn()
{
    _isPlayerTurn = true;
    createCards();

    // 添加一个按钮来手动结束回合
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto endTurnButton = ui::Button::create("button.png", "button.png");

    if (endTurnButton == nullptr)
    {
        problemLoading("'buttonNormal.png' or 'buttonSelected.png'");
        return;
    }

    endTurnButton->setScale(0.25f); // 将按钮缩小到原来的50%


    endTurnButton->setPosition(Vec2(origin.x + visibleSize.width - endTurnButton->getContentSize().width / 2,
        origin.y + endTurnButton->getContentSize().height / 2));

    endTurnButton->addClickEventListener([this](Ref* sender) {
        this->endPlayerTurn();
        });

    this->addChild(endTurnButton, 1);
}

void FightingScene::startMonsterTurn()
{
    _isPlayerTurn = false;
    // 怪物对主角造成10点伤害
    int newHealth = _hero->getHealth() - 10;
    _hero->setHealth(newHealth);
    CCLOG("Hero Health: %d", _hero->getHealth());
    updateHealthLabels();
    endTurn();
}

void FightingScene::endTurn()
{
    checkBattleEnd();
    if (!_isPlayerTurn)
    {
        startPlayerTurn();
    }
    else
    {
        startMonsterTurn();
    }
}

void FightingScene::checkBattleEnd()
{
    if (_hero->getHealth() <= 0)
    {
        CCLOG("Hero is dead. Game Over.");
        // 处理游戏结束逻辑
    }
    else if (_monster->getHealth() <= 0)
    {
        CCLOG("Monster is dead. You Win!");
        // 处理游戏胜利逻辑
    }
}

// 抽一张牌
void FightingScene::drawCard()
{
    if (_drawPile.empty())
    {
        // 如果抽牌堆为空，将弃牌堆的牌洗入抽牌堆
        _drawPile = _discardPile;
        _discardPile.clear();
        shuffleDrawPile();
    }

    if (!_drawPile.empty())
    {
        // 从抽牌堆顶抽一张牌
        Card drawnCard = _drawPile.back();
        _drawPile.pop_back();
        _cards.push_back(drawnCard);
    }
}

// 弃一张牌
void FightingScene::discardCard(int index)
{
    if (index >= 0 && index < _cards.size())
    {
        // 将卡牌放入弃牌堆
        _discardPile.push_back(_cards[index]);
        _cards.erase(_cards.begin() + index);
    }
}

// 洗牌
void FightingScene::shuffleDrawPile()
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(_drawPile.begin(), _drawPile.end(), g);
}

void FightingScene::updateHandDisplay()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 移除所有已有的卡牌
    removeExistingCards();

    // 显示当前手牌中的卡牌
    displayDrawnCards(_cards.size(), visibleSize, origin);
}

void FightingScene::removeExistingCards()
{
    auto children = this->getChildren();
    std::vector<Node*> toRemove;
    for (const auto& child : children)
    {
        if (child->getTag() >= 1000 && child->getTag() < 1010)
        {
            toRemove.push_back(child);
        }
    }
    for (const auto& node : toRemove)
    {
        node->removeFromParent();
    }
}

void FightingScene::displayDrawnCards(int numCards, const Size& visibleSize, const Vec2& origin)
{
    float cardWidth = visibleSize.width / (numCards + 1);
    float cardHeight = visibleSize.height / 5;

    for (int i = 0; i < _cards.size(); ++i)
    {
        auto cardSprite = _cards[i].createCardSprite();
        if (cardSprite == nullptr)
        {
            continue;
        }

        setupCardSprite(cardSprite, i, cardWidth, cardHeight, origin);
        addCardEffectLabel(cardSprite, _cards[i].getEffect());
        addCardTouchListener(cardSprite, i);
    }
}

void FightingScene::setupCardSprite(cocos2d::Sprite* cardSprite, int index, float cardWidth, float cardHeight, const Vec2& origin)
{
    cardSprite->setTag(1000 + index);

    float scaleX = cardWidth / cardSprite->getContentSize().width;
    float scaleY = cardHeight / cardSprite->getContentSize().height;
    float scale = std::max(scaleX, scaleY);
    cardSprite->setScale(scale);

    float xPos = origin.x + (index + 1) * cardWidth;
    float yPos = origin.y + cardHeight / 2;
    cardSprite->setPosition(Vec2(xPos, yPos));
    this->addChild(cardSprite, 1);
}

void FightingScene::addCardEffectLabel(cocos2d::Sprite* cardSprite, const std::string& effect)
{
    auto effectLabel = Label::createWithTTF(effect, "fonts/Marker Felt.ttf", 24);
    effectLabel->setColor(cocos2d::Color3B::BLACK);
    effectLabel->setPosition(Vec2(cardSprite->getContentSize().width / 2, cardSprite->getContentSize().height / 2));
    cardSprite->addChild(effectLabel, 1);
}

void FightingScene::addCardTouchListener(cocos2d::Sprite* cardSprite, int index)
{
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [this, cardSprite, index](Touch* touch, Event* event) {
        return this->onCardTouchBegan(cardSprite, index, touch, event);
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, cardSprite);
}

void FightingScene::initializeDrawPile()
{
    // 获取 Hero 的卡组并设置为抽牌堆
    _drawPile = _hero->getDeck();
    shuffleDrawPile();
}

void FightingScene::endPlayerTurn()
{
    endTurn();
}
