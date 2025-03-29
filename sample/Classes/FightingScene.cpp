#include "FightingScene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

// 创建场景
Scene* FightingScene::createScene()
{
    return FightingScene::create();
}

// 找错误函数
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in FightingScene.cpp\n");
}

// 初始化场景
bool FightingScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    _visibleSize = Director::getInstance()->getVisibleSize();
    _origin = Director::getInstance()->getVisibleOrigin();

    // 创建背景
    createBackground();

    // 创建角色和怪物
    createCharacters();

    // 初始化抽牌堆和弃牌堆
    initializeDrawPile();

    // 创建血量标签
    createHealthLabels();

    // 创建格挡标签
    createBlockLabels();

	// 更新血量标签
    updateHealthAndBlockLabels();

    // 开始玩家回合
    startPlayerTurn();

    return true;
}

// 创建血量标签
void FightingScene::createHealthLabels()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建英雄血量标签
    _heroHealthLabel = Label::createWithTTF("Hero Health: 100", "fonts/Marker Felt.ttf", 60);
    _heroHealthLabel->setTextColor(Color4B::RED); // 设置标签颜色为红色
    _heroHealthLabel->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + visibleSize.height - _heroHealthLabel->getContentSize().height));
    this->addChild(_heroHealthLabel, 1);

    // 创建怪物血量标签
    _monsterHealthLabel = Label::createWithTTF("Monster Health: 100", "fonts/Marker Felt.ttf", 60);
    _monsterHealthLabel->setTextColor(Color4B::RED); // 设置标签颜色为红色
    _monsterHealthLabel->setPosition(Vec2(origin.x + 3 * visibleSize.width / 4, origin.y + visibleSize.height - _monsterHealthLabel->getContentSize().height));
    this->addChild(_monsterHealthLabel, 1);
}

// 创建格挡标签
void FightingScene::createBlockLabels()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建英雄格挡标签
    _heroBlockLabel = Label::createWithTTF("Hero Block: 0", "fonts/Marker Felt.ttf", 60);
    _heroBlockLabel->setTextColor(Color4B::BLUE); // 设置标签颜色为蓝色
    _heroBlockLabel->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + visibleSize.height - _heroHealthLabel->getContentSize().height - 70));
    this->addChild(_heroBlockLabel, 1);

    // 创建怪物格挡标签
    _monsterBlockLabel = Label::createWithTTF("Monster Block: 0", "fonts/Marker Felt.ttf", 60);
    _monsterBlockLabel->setTextColor(Color4B::BLUE); // 设置标签颜色为蓝色
    _monsterBlockLabel->setPosition(Vec2(origin.x + 3 * visibleSize.width / 4, origin.y + visibleSize.height - _monsterHealthLabel->getContentSize().height - 70));
    this->addChild(_monsterBlockLabel, 1);
}

// 更新血量和格挡标签
void FightingScene::updateHealthAndBlockLabels()
{
    _heroHealthLabel->setString("Hero Health: " + std::to_string(_hero->getHealth()));
    _monsterHealthLabel->setString("Monster Health: " + std::to_string(_monster->getHealth()));
    _heroBlockLabel->setString("Hero Block: " + std::to_string(_hero->getBlock()));
    _monsterBlockLabel->setString("Monster Block: " + std::to_string(_monster->getBlock()));
}

// 创建背景
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

// 创建角色和怪物
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

// 开始玩家回合(抽牌、添加回合结束按钮、设置按钮位置)
void FightingScene::startPlayerTurn()
{
    _isPlayerTurn = true;

    for (int i = 0; i < 5; i++) {
        drawCard();
    }

    // 添加一个按钮来手动结束回合
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto endTurnButton = ui::Button::create("button.png", "button_selected.png");

    if (endTurnButton == nullptr)
    {
        problemLoading("'buttonNormal.png' or 'buttonSelected.png'");
        return;
    }

    endTurnButton->setScale(0.25f); // 将按钮缩小到原来的50%

	// 设置按钮位置
    endTurnButton->setPosition(Vec2(origin.x + visibleSize.width - endTurnButton->getContentSize().width / 2,
        origin.y + visibleSize.width / 2 - endTurnButton->getContentSize().height / 2));

    endTurnButton->addClickEventListener([this](Ref* sender) {
        this->endTurn();
        });

    this->addChild(endTurnButton, 1);
}

// 开始怪物回合
void FightingScene::startMonsterTurn()
{
    _isPlayerTurn = false;
	// 目前怪物回合对主角造成10伤害并获得5护甲
    int damage = 10;
    int block = 5;

	_monster->setBlock(block);

    // 处理格挡
    int heroBlock = _hero->getBlock();
    if (heroBlock > 0)
    {
        if (heroBlock >= damage)
        {
            _hero->setBlock(heroBlock - damage);
            damage = 0;
        }
        else
        {
            damage -= heroBlock;
            _hero->setBlock(0);
        }
    }

    // 怪物对主角造成伤害
    int newHealth = _hero->getHealth() - damage;
    _hero->setHealth(newHealth);
    CCLOG("Hero Health: %d", _hero->getHealth());
    updateHealthAndBlockLabels();
    endTurn();
}

// 判断回合是否结束
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

// 检查战斗是否结束
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
        updateHandDisplay(); // 更新手牌显示
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
        updateHandDisplay(); // 更新手牌显示
    }
}

// 洗牌
void FightingScene::shuffleDrawPile()
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(_drawPile.begin(), _drawPile.end(), g);
}

// 初始化牌堆
void FightingScene::initializeDrawPile()
{
    // 获取 Hero 的卡组并设置为抽牌堆
    _drawPile = _hero->getDeck();
    shuffleDrawPile();
}

// 给卡牌添加效果标签
void FightingScene::addCardEffectLabel(cocos2d::Sprite* cardSprite, const std::string& effect)
{
    auto effectLabel = Label::createWithTTF(effect, "fonts/Marker Felt.ttf", 48);
    effectLabel->setColor(cocos2d::Color3B::BLACK);
    effectLabel->setPosition(Vec2(cardSprite->getContentSize().width / 2, cardSprite->getContentSize().height / 2));
    cardSprite->addChild(effectLabel, 1);
}

// 更新手牌显示
void FightingScene::updateHandDisplay()
{
    // 清除当前手牌显示
    for (auto cardSprite : _cardSprites)
    {
        this->removeChild(cardSprite);
    }
    _cardSprites.clear();
    _lastClickTimes.clear(); // 清除上次点击时间

    // 获取可见区域大小和原点
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 假设每张卡牌的背景图片为 "cardBackground.jpg"
    auto tempCardSprite = Sprite::create("cardBackground.jpg");
    float cardWidth = tempCardSprite->getContentSize().width;

    // 计算卡牌之间的间距
    float totalWidth = _cards.size() * cardWidth;
    float spacing = 0.0f;
    if (totalWidth > visibleSize.width)
    {
        cardWidth = (visibleSize.width - 20.0f) / _cards.size(); // 20.0f 是左右边距
        spacing = 0.0f;
    }
    else
    {
        spacing = (visibleSize.width - totalWidth) / (_cards.size() + 1);
    }

    float startX = origin.x + spacing + cardWidth / 2;

    for (size_t i = 0; i < _cards.size(); ++i)
    {
        auto cardSprite = Sprite::create("cardBackground.jpg");
        cardSprite->setPosition(Vec2(startX + i * (cardWidth + spacing), origin.y + cardSprite->getContentSize().height / 3));
        cardSprite->setScale(cardWidth / cardSprite->getContentSize().width);
        this->addChild(cardSprite, 1);
        _cardSprites.push_back(cardSprite);
        _lastClickTimes.push_back(std::chrono::steady_clock::now()); // 初始化上次点击时间

        addCardEffectLabel(cardSprite, _cards[i].getEffect());

        // 添加点击事件监听器：提高响应性
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [this, i](Touch* touch, Event* event) -> bool {
            auto target = static_cast<Sprite*>(event->getCurrentTarget());
            if (target->getBoundingBox().containsPoint(touch->getLocation()))
            {
                handleCardTap(i, touch);
                return true;
            }
            return false;
            };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, cardSprite);
    }
}

// 应用BUFF和DEBUFF
void FightingScene::applyEffects(int& damage, int& block, const std::vector<std::shared_ptr<Effect>>& effects, bool isTargetMonster)
{
    for (const auto& effect : effects)
    {
        if (auto buff = dynamic_cast<Buff*>(effect.get()))
        {
            switch (buff->getType())
            {
            case Effect::Type::Strength:
                damage += buff->getLevel();
                break;
            default:
                break;
            }
        }
        else if (auto debuff = dynamic_cast<Debuff*>(effect.get()))
        {
            switch (debuff->getType())
            {
            case Effect::Type::Vulnerable:
                if (isTargetMonster)
                {
                    damage = static_cast<int>(damage * 1.5);
                }
                break;
            default:
                break;
            }
        }
    }
}

// 应用卡牌效果
void FightingScene::applyCardEffects(const Card& card)
{
    int damage = card.getAttack();
    int block = card.getBlock();

    // 应用英雄的效果
    applyEffects(damage, block, _hero->getEffects(), false);

    // 应用怪物的效果
    applyEffects(damage, block, _monster->getEffects(), true);

    // 处理怪物的格挡
    int monsterBlock = _monster->getBlock();
    if (monsterBlock > 0)
    {
        if (monsterBlock >= damage)
        {
            _monster->setBlock(monsterBlock - damage);
            damage = 0;
        }
        else
        {
            damage -= monsterBlock;
            _monster->setBlock(0);
        }
    }

    // 处理怪物的生命值
    int newHealth = _monster->getHealth() - damage;
    _monster->setHealth(newHealth);
    CCLOG("Monster Health: %d", _monster->getHealth());

    // 处理英雄的格挡
    if (block > 0)
    {
        int newBlock = _hero->getBlock() + block;
        _hero->setBlock(newBlock);
        CCLOG("Hero Block: %d", _hero->getBlock());
    }

    updateHealthAndBlockLabels();
}

// 打出卡牌
void FightingScene::playCard(int index)
{
    if (_isCooldown) {
        // 如果处于冷却状态，直接返回
        return;
    }

    if (index >= 0 && index < _cards.size())
    {
        // 执行打出卡牌的逻辑
        Card playedCard = _cards[index];
        applyCardEffects(playedCard);

        // 设置冷却状态
        _isCooldown = true;
        this->runAction(Sequence::create(
            Spawn::create(
                Sequence::create(
                    DelayTime::create(COOLDOWN_TIME),
                    CallFunc::create([this]() { _isCooldown = false; }),
                    nullptr
                ),
                Sequence::create(
                    DelayTime::create(DISCARD_DELAY),
                    CallFunc::create([this, index]() {
                        discardCard(index);
                        _selectedCardIndex = -1;
                        highlightSelectedCard();
                        }),
                    nullptr
                ),
                nullptr
            ),
            nullptr
        ));
    }
}

// 高亮选中的卡牌
void FightingScene::highlightSelectedCard()
{
    for (size_t i = 0; i < _cardSprites.size(); ++i)
    {
        if (i == _selectedCardIndex)
        {
            _cardSprites[i]->setColor(Color3B::YELLOW); // 高亮选中的卡牌
        }
        else
        {
            _cardSprites[i]->setColor(Color3B::WHITE); // 取消高亮其他卡牌
        }
    }
}

// 处理卡牌点击事件
void FightingScene::handleCardTap(size_t cardIndex, cocos2d::Touch* touch)
{
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastClickTimes[cardIndex]).count();

    if (duration < DOUBLE_CLICK_THRESHOLD)
    {
        playCard(cardIndex);
    }
    else
    {
        _selectedCardIndex = cardIndex;
        highlightSelectedCard();
    }
    _lastClickTimes[cardIndex] = now;
}
