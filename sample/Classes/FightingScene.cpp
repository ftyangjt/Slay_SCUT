#include "FightingScene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "Map.h"
#include "fail.h"
#include "Effect.h"

USING_NS_CC;

// 以下为初始化函数的实现
// 创建场景
Scene* FightingScene::createScene()
{
    return FightingScene::create();
}

// 检查加载资源是否出错
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

	// 设置抽牌堆按钮
    createDrawDeck();

	//创建弃牌堆按钮
    createDiscardDeck();


    // 初始化抽牌堆和弃牌堆
    initializeDrawPile();
	// 创建费用标签
	createCostLabel();

    // 创建血量标签
    createHealthLabels();

    // 创建格挡标签
    createBlockLabels();

	// 创建BUFF和DEBUFF标签
    createBuffLabels();

	// 更新血量标签
    updateHealthAndBlockLabels();

	// 初始化回合数
    _turnCount = 1;

	// 创建回合数标签
    createTurnCountLabel();

    

    // 开始玩家回合
    startPlayerTurn();

    // 在 FightingScene.cpp 的 init() 函数末尾添加
    CCLOG("Current Room Type: %d", static_cast<int>(MyGame::currentRoomType));


    return true;
}


//创建费用标签
void FightingScene::createCostLabel()
{
    // 获取英雄的位置
    Vec2 heroPosition = _hero->getPosition();
    float heroHeight = _hero->getContentSize().height;

    // 创建费用标签
    _costLabel = Label::createWithTTF("Cost: 3", "fonts/Marker Felt.ttf", 60);
    _costLabel->setTextColor(Color4B::ORANGE); // 设置标签颜色为orange

    // 设置费用标签的位置到英雄脚下
    _costLabel->setPosition(Vec2(heroPosition.x, heroPosition.y - heroHeight / 2+60));
    this->addChild(_costLabel, 1);
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

// 创建 BUFF 和 DEBUFF 标签
void FightingScene::createBuffLabels() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建英雄 BUFF 标签
    _heroBuffLabel = Label::createWithTTF("Hero Buffs: None", "fonts/Marker Felt.ttf", 40);
    _heroBuffLabel->setTextColor(Color4B::YELLOW); // 设置标签颜色为黄色
    _heroBuffLabel->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + visibleSize.height - _heroHealthLabel->getContentSize().height - 140));
    this->addChild(_heroBuffLabel, 1);

    // 创建怪物 BUFF 标签
    _monsterBuffLabel = Label::createWithTTF("Monster Buffs: None", "fonts/Marker Felt.ttf", 40);
    _monsterBuffLabel->setTextColor(Color4B::YELLOW); // 设置标签颜色为黄色
    _monsterBuffLabel->setPosition(Vec2(origin.x + 3 * visibleSize.width / 4, origin.y + visibleSize.height - _monsterHealthLabel->getContentSize().height - 140));
    this->addChild(_monsterBuffLabel, 1);
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

// 创建回合数标签
void FightingScene::createTurnCountLabel()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建回合数标签
    _turnCountLabel = Label::createWithTTF("Turn: 1", "fonts/Marker Felt.ttf", 60);
    _turnCountLabel->setTextColor(Color4B::GREEN); // 设置标签颜色为绿色
    _turnCountLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - _turnCountLabel->getContentSize().height));
    this->addChild(_turnCountLabel, 1);
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

// 初始化牌堆
void FightingScene::initializeDrawPile()
{
    // 获取 Hero 的卡组并设置为抽牌堆
    _drawPile = _hero->getDeck();
    shuffleDrawPile();
}


// 以下为回合制战斗逻辑的实现
// 更新血量和格挡标签
void FightingScene::updateHealthAndBlockLabels()
{
    _heroHealthLabel->setString("Hero Health: " + std::to_string(_hero->getHealth()));
    _monsterHealthLabel->setString("Monster Health: " + std::to_string(_monster->getHealth()));
    _heroBlockLabel->setString("Hero Block: " + std::to_string(_hero->getBlock()));
    _monsterBlockLabel->setString("Monster Block: " + std::to_string(_monster->getBlock()));


}

// 更新费用标签
void FightingScene::updateCostLabel()
{
    _costLabel->setString("Cost: " + std::to_string(_currentCost));
}

// 更新 BUFF 和 DEBUFF 标签
void FightingScene::updateBuffLabels() {
    // 更新英雄 BUFF 标签
    const auto& heroEffects = _hero->getEffects();
    if (heroEffects.empty()) {
        _heroBuffLabel->setString("Hero Buffs: None");
    }
    else {
        std::string heroBuffs = "Hero Buffs: ";
        for (const auto& effect : heroEffects) {
            heroBuffs += effect->getDescription() + " (" + std::to_string(effect->getRemainingTurns()) + " turns), ";
        }
        heroBuffs.pop_back(); // 移除最后一个逗号
        heroBuffs.pop_back();
        _heroBuffLabel->setString(heroBuffs);
    }

    // 更新怪物 BUFF 标签
    const auto& monsterEffects = _monster->getEffects();
    if (monsterEffects.empty()) {
        _monsterBuffLabel->setString("Monster Buffs: None");
    }
    else {
        std::string monsterBuffs = "Monster Buffs: ";
        for (const auto& effect : monsterEffects) {
            monsterBuffs += effect->getDescription() + " (" + std::to_string(effect->getRemainingTurns()) + " turns), ";
        }
        monsterBuffs.pop_back(); // 移除最后一个逗号
        monsterBuffs.pop_back();
        _monsterBuffLabel->setString(monsterBuffs);
    }
}

// 开始玩家回合(抽牌、添加回合结束按钮、设置按钮位置)
void FightingScene::startPlayerTurn()
{
    _isPlayerTurn = true;
	// 重置费用
    _currentCost = 3;
	// 更新回合数标签
    _turnCountLabel->setString("Turn: " + std::to_string(_turnCount));

	_costLabel->setString("Cost: " + std::to_string(_currentCost));

    updateHandDisplay();

    drawSequentialCards(5);

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

// 设置弃牌堆按钮
void FightingScene::createDiscardDeck()
{
    auto showDiscardDeckButton = MenuItemImage::create(
        "showDiscard_Normal.png",
        "showDiscard_Selected.png",
        CC_CALLBACK_1(FightingScene::goToDiscardDeck, this)
    );
    showDiscardDeckButton->setScale(0.25f);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 设置按钮位置在画面右下角
    showDiscardDeckButton->setPosition(Vec2(origin.x + visibleSize.width - showDiscardDeckButton->getContentSize().width / 2,
        origin.y + showDiscardDeckButton->getContentSize().height * 1.4));

    // 保存按钮指针到成员变量中
    _discardDeckButton = showDiscardDeckButton;

    auto buttonMenu = Menu::create(showDiscardDeckButton, nullptr);
    buttonMenu->setPosition(Vec2::ZERO);
    this->addChild(buttonMenu, 4);
}



// 设置抽牌堆按钮
void FightingScene::createDrawDeck()
{
    auto showDrawDeckButton = MenuItemImage::create(
        "showDraw_Normal.png",
        "showDraw_Selected.png",
        CC_CALLBACK_1(FightingScene::goToDrawDeck, this)
    );
    showDrawDeckButton->setScale(0.25f);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 设置按钮位置在画面左下角
    showDrawDeckButton->setPosition(Vec2(origin.x + showDrawDeckButton->getContentSize().width / 2,
        origin.y + showDrawDeckButton->getContentSize().height * 1.4));
	// 保存按钮指针到成员变量中
	_drawDeckButton = showDrawDeckButton;
    auto buttonMenu = Menu::create(showDrawDeckButton, nullptr);
    buttonMenu->setPosition(Vec2::ZERO);
    this->addChild(buttonMenu, 4);
}


// 开始怪物回合
void FightingScene::startMonsterTurn()
{
    _isPlayerTurn = false;
    // 目前怪物回合对主角造成10伤害并获得5护甲
    int damage = 10;
    int block = 5;
    playMonsterAttackAnimation();
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
	if (damage > 0)
	{
    
		playHeroHitAnimation();
	}
    int newHealth = _hero->getHealth() - damage;
	
    _hero->setHealth(newHealth);
    CCLOG("Hero Health: %d", _hero->getHealth());
    updateHealthAndBlockLabels();
    endTurn();
}
// 结束回合
void FightingScene::endTurn()
{
    if (_isPlayerTurn)
    {
        // 丢弃所有手牌至弃牌堆
        _discardPile.insert(_discardPile.end(), _cards.begin(), _cards.end());
        _cards.clear();
        Vec2 discardDeckPosition = _discardDeckButton->getPosition();
		    Vec2 drawDeckPosition = _drawDeckButton->getPosition();
        playDiscardToDrawMeteorEffect(discardDeckPosition,drawDeckPosition);

        updateHandDisplay(); // 更新手牌显示

		    // 重置怪物格挡
        _monster->setBlock(0);
		    updateHealthAndBlockLabels();
    }

    else
    {
        // 怪物回合结束时，递增回合计数器
        _turnCount++;

		// 重置英雄格挡
        _hero->setBlock(0);
       
        updateHealthAndBlockLabels();

        // 更新效果持续时间
        _hero->updateEffects(); // 更新英雄的效果
        _monster->updateEffects(); // 更新怪物的效果
		updateBuffLabels(); // 更新 BUFF 和 DEBUFF 标签
    }

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
        // 现有失败处理代码保持不变...
    }
    else if (_monster->getHealth() <= 0)
    {
        CCLOG("Monster is dead. You Win!");

        // 根据当前房间类型设置金币奖励
        int coinReward = 20;  // 默认普通小怪奖励20金币

        if (MyGame::currentRoomType == MyGame::RoomType::ELITE) {
            coinReward = 40;  // 精英怪奖励40金币
        }
        else if (MyGame::currentRoomType == MyGame::RoomType::BOSS) {
            coinReward = 100;  // Boss奖励100金币
        }

        // 添加金币
        Hero::addCoins(coinReward);

        // 创建获得金币的提示标签
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();

        auto coinLabel = Label::createWithTTF("获得 " + std::to_string(coinReward) + " 金币!", "fonts/Marker Felt.ttf", 60);
        coinLabel->setTextColor(Color4B::YELLOW);
        coinLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 100));
        this->addChild(coinLabel, 10);

        // 创建胜利消息标签（现有代码）
        auto victoryLabel = Label::createWithTTF("YOU WIN！", "fonts/Marker Felt.ttf", 80);
        victoryLabel->setTextColor(Color4B::YELLOW);
        victoryLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        this->addChild(victoryLabel, 10);

        // 延迟动作（现有代码）
        this->runAction(Sequence::create(
            DelayTime::create(0.1f),
            CallFunc::create([]() {
                auto mapScene = MyGame::Map::createScene();
                Director::getInstance()->replaceScene(TransitionFade::create(0.5f, mapScene));
                }),
            nullptr
        ));
    }
}

// 以下为卡牌系统的实现
void FightingScene::drawCard()
{
    if (_drawPile.empty())
    {
        // 若抽牌堆为空，将弃牌堆洗回抽牌堆
        _drawPile = _discardPile;
        _discardPile.clear();
        shuffleDrawPile();
    }

    if (!_drawPile.empty())
    {
        // 从抽牌堆取顶
        Card drawnCard = _drawPile.back();
        _drawPile.pop_back();

        // 创建临时精灵，从抽牌堆按钮位置出现
        auto tempSprite = Sprite::create("cardBackground.jpg");
        tempSprite->setPosition(_drawDeckButton->getPosition());
        tempSprite->setScale(0.0f);
        tempSprite->setLocalZOrder(9999); // 保证显示在最前
        this->addChild(tempSprite);

        // 计算飞往手牌放置位置（简单示例：手牌区域中心）
        auto visibleSize = Director::getInstance()->getVisibleSize();
        float targetX = visibleSize.width * 0.5f;
        float targetY = tempSprite->getContentSize().height / 3;

        // 飞行并放大
        auto moveAction = MoveTo::create(0.3f, Vec2(targetX, targetY));
        auto scaleAction = ScaleTo::create(0.3f, 1.0f);
        auto spawn = Spawn::create(moveAction, scaleAction, nullptr);

        // 动作结束后，将卡牌加入 _cards 并刷新手牌
        auto finish = CallFunc::create([this, drawnCard, tempSprite]() {
            _cards.push_back(drawnCard);
            tempSprite->removeFromParent();
            updateHandDisplay();
            });

        tempSprite->runAction(Sequence::create(spawn, finish, nullptr));
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

// 给卡牌添加效果标签
cocos2d::Label* FightingScene::addCardEffectLabel(cocos2d::Sprite* cardSprite, const std::string& effect)
{
    // 获取卡牌的宽度
    float cardWidth = cardSprite->getContentSize().width;

    // 创建多行标签，并设置最大宽度为卡牌宽度
    auto effectLabel = Label::createWithTTF(effect, "fonts/Marker Felt.ttf", 48, Size(cardWidth, 0), TextHAlignment::CENTER);
    effectLabel->setColor(cocos2d::Color3B::BLACK);

    // 设置标签位置为卡牌中心
    effectLabel->setPosition(Vec2(cardSprite->getContentSize().width / 2, cardSprite->getContentSize().height / 2));
    cardSprite->addChild(effectLabel, 1);

    return effectLabel; // 返回效果标签
}


// 刷新手牌显示
// 刷新手牌显示
void FightingScene::updateHandDisplay()
{
    size_t newCount = _cards.size();

    // 清除现有卡牌精灵
    for (auto sprite : _cardSprites)
    {
        sprite->removeFromParent();
    }
    _cardSprites.clear();
    _lastClickTimes.clear();

    if (newCount == 0)
        return;  // 无卡牌则直接返回

    // 创建临时精灵获取卡牌原始尺寸
    auto tempSprite = Sprite::create("cardBackground.jpg");
    float originalCardWidth = tempSprite->getContentSize().width;
    float originalCardHeight = tempSprite->getContentSize().height;

    // 设置固定的缩放比例
    float cardScale = 0.6f;

    // 计算缩放后的实际卡牌宽度
    float scaledCardWidth = originalCardWidth * cardScale;
    float scaledCardHeight = originalCardHeight * cardScale;

    // 使用负值的间距使卡牌重叠，这将使它们更紧密
    float cardOverlap = -20.0f;  // 负值表示重叠

    // 计算全部卡牌排列的总宽度，并居中显示
    float totalWidth = scaledCardWidth + (newCount - 1) * (scaledCardWidth + cardOverlap);
    float startX = (_visibleSize.width - totalWidth) / 2 + scaledCardWidth / 2;

    // 创建并排列卡牌
    for (size_t i = 0; i < newCount; ++i)
    {
        auto sprite = Sprite::create("cardBackground.jpg");
        // 计算重叠的位置
        float posX = startX + i * (scaledCardWidth + cardOverlap);

        // 设置卡牌位置和固定缩放比例
        sprite->setPosition(Vec2(posX, _origin.y + scaledCardHeight * 0.4f));
        sprite->setScale(cardScale);
        this->addChild(sprite, 1);
        _cardSprites.push_back(sprite);
        _lastClickTimes.push_back(std::chrono::steady_clock::now());

        // 添加卡牌效果标签
        auto effectLabel = addCardEffectLabel(sprite, _cards[i].getEffect());

        // 添加费用标签
        auto costLabel = Label::createWithTTF("Cost: " + std::to_string(_cards[i].getCost()), "fonts/Marker Felt.ttf", 24);
        costLabel->setTextColor(Color4B::ORANGE);
        costLabel->setPosition(Vec2(sprite->getContentSize().width / 2, effectLabel->getPositionY() - 60));
        sprite->addChild(costLabel, 1);

        // 注册触摸事件
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
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, sprite);
    }

    // 高亮选中的卡牌（如果有）
    if (_selectedCardIndex >= 0 && _selectedCardIndex < _cardSprites.size())
    {
        highlightSelectedCard();
    }
}




// 应用BUFF和DEBUFF
void FightingScene::applyEffects(int& damage, int& block, const std::vector<std::shared_ptr<Effect>>& effects, const Card::Type cardType, bool isTargetMonster)
{
    for (const auto& effect : effects)
    {
        if (auto buff = dynamic_cast<Buff*>(effect.get()))
        {
            switch (buff->getType())
            {
            case Effect::Type::Strength:
                // 只有攻击类型的卡牌才会受到力量效果的影响
                if (cardType == Card::Type::Attack)
                {
                    damage += buff->getLevel();
                }
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
                    damage = static_cast<int>(damage * 1.5); // 易伤增加伤害倍率
                }
                break;
            default:
                break;
            }
        }
    }
}

// 应用卡牌效果（攻击、格挡、添加BUFF）
void FightingScene::applyCardEffects(const Card& card)
{
    int damage = card.getAttack();
    int block = card.getBlock();

    // 应用英雄的效果
	// 例如，如果有力量效果，增加攻击伤害
    applyEffects(damage, block, _hero->getEffects(), card.getType(), false);

    // 应用怪物的效果
	// 例如，如果有易伤效果，增加对其造成的伤害
    applyEffects(damage, block, _monster->getEffects(), card.getType(), true);

    std::vector<std::shared_ptr<Effect>> effects = card.createEffects();

    for (const auto& effect : effects)
    {
        if (auto buff = dynamic_cast<Buff*>(effect.get()))
        {
            switch (buff->getType())
            {
            case Effect::Type::Strength:
                _hero->addEffect(effect); // 力量效果应用到英雄
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
                _monster->addEffect(effect);
                break;
            default:
                break;
            }
        }
    }

    updateBuffLabels();

    // 处理怪物的格挡
    int monsterBlock = _monster->getBlock();

    CCLOG("Monster block: %d", monsterBlock);

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

	CCLOG("Damage: %d", damage);

    // 处理怪物的生命值
    
    int newHealth = _monster->getHealth() - damage;
    if (damage > 0) {
        playMonsterHitAnimation(); // 如果造成了伤害，播放怪物受击动画
    }
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
        return; // 如果处于冷却状态，直接返回
    }

    if (index >= 0 && index < _cards.size()) {
        // 原逻辑不变
        Card playedCard = _cards[index];

        int cost = playedCard.getCost();
        // 检查能量是否足够
        if (cost > _currentCost)
        {
            CCLOG("Energy not enough to play this card!");
            return;
        }
        // 如果费用足够，扣费并进行原有逻辑
        _currentCost -= cost;
        updateCostLabel();

        // 处理卡牌精灵
        auto cardSprite = _cardSprites[index];

        // 如果是攻击类型的卡牌，播放英雄的挥刀动画
        if (playedCard.getType() == Card::Type::Attack) {
            // 播放英雄的挥刀动画
            playHeroAttackAnimation();
        }

        applyCardEffects(playedCard);
        // 让该卡牌优先显示
        cardSprite->setLocalZOrder(9999);

        // 目标位置：弃牌堆按钮
        Vec2 discardPos = _discardDeckButton->getPosition();
        auto moveAction = MoveTo::create(0.3f, discardPos);
        auto scaleAction = ScaleTo::create(0.3f, 0.1f);
        auto moveAndScale = Spawn::create(moveAction, scaleAction, nullptr);

        // 动画播放完再移除并丢弃
        auto finish = CallFunc::create([this, index, cardSprite]() {
            cardSprite->removeFromParent();
            discardCard(index);
            _selectedCardIndex = -1;
            highlightSelectedCard();
            });

        cardSprite->runAction(Sequence::create(moveAndScale, finish, nullptr));

        // 设置冷却
        checkBattleEnd();

        // 设置冷却状态
        _isCooldown = true;
        this->runAction(Sequence::create(
            DelayTime::create(COOLDOWN_TIME),
            CallFunc::create([this]() { _isCooldown = false; }),
            nullptr
        ));
    }
}


// 高亮选中的卡牌
void FightingScene::highlightSelectedCard()
{
    for (size_t i = 0; i < _cardSprites.size(); ++i)
    {
        // 确保精灵有效且仍然在场景中
        if (_cardSprites[i] && _cardSprites[i]->getParent())
        {
            if (i == _selectedCardIndex)
            {
                // 获取卡牌的费用
                int cardCost = _cards[i].getCost();

                // 如果费用足够，标黄；否则标红
                if (cardCost <= _currentCost)
                {
                    _cardSprites[i]->setColor(cocos2d::Color3B(255, 255, 0)); // 黄色
                }
                else
                {
                    _cardSprites[i]->setColor(cocos2d::Color3B(255, 0, 0)); // 红色
                }
            }
            else
            {
                // 恢复正常颜色
                _cardSprites[i]->setColor(cocos2d::Color3B(255, 255, 255)); // 白色
            }
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

// 进入抽牌堆场景
void FightingScene::goToDrawDeck(Ref* sender)
{
    auto drawDeckScene = DrawDeck::createScene(_drawPile);
    Director::getInstance()->pushScene(drawDeckScene);
}

// 进入弃牌堆场景
void FightingScene::goToDiscardDeck(Ref* sender)
{
    auto discardDeckScene = DiscardDeck::createScene(_discardPile);
    Director::getInstance()->pushScene(discardDeckScene);
}

// 假设 discardPilePosition、drawPilePosition 为弃牌堆与抽牌堆的屏幕坐标

void FightingScene::playDiscardToDrawMeteorEffect(const Vec2& discardPilePosition, const Vec2& drawPilePosition)
{
    // 创建流星粒子
    auto meteor = ParticleMeteor::create();
    meteor->setPosition(discardPilePosition);
    meteor->setDuration(0.8f); // 总时长，可根据需要调整

    // 调整粒子颜色（加深颜色）
    Color4F currentColor = meteor->getStartColor();
    Color4F deepColor(currentColor.r * 0.5f, currentColor.g * 0.5f, currentColor.b * 0.5f, currentColor.a);
    meteor->setStartColor(deepColor);
    // 如有需要，也可以调整终点颜色
    Color4F endColor = meteor->getEndColor();
    Color4F deepEndColor(endColor.r * 0.5f, endColor.g * 0.5f, endColor.b * 0.5f, endColor.a);
    meteor->setEndColor(deepEndColor);

    // 创建移动动作
    auto moveAction = MoveTo::create(0.8f, drawPilePosition);
    auto sequence = Sequence::create(
        moveAction,
        CallFunc::create([meteor]() {
            meteor->removeFromParent(); // 移除粒子
            }),
        nullptr);

    // 运行动作并添加到场景
    meteor->runAction(sequence);
    meteor->setAutoRemoveOnFinish(false); // 不自动移除，手动移除
    this->addChild(meteor, 10);
}

// 递归依次抽多张牌的函数
void FightingScene::drawSequentialCards(int count)
{
    if (count <= 0) return;

    if (_drawPile.empty())
    {
        _drawPile = _discardPile;
        _discardPile.clear();
        shuffleDrawPile();
    }

    if (!_drawPile.empty())
    {
        // 从抽牌堆取顶
        Card drawnCard = _drawPile.back();
        _drawPile.pop_back();

        // 创建临时精灵，从抽牌堆按钮位置出现
        auto tempSprite = Sprite::create("cardBackground.jpg");
        tempSprite->setPosition(_drawDeckButton->getPosition());
        tempSprite->setScale(0.0f);
        tempSprite->setLocalZOrder(9999);
        this->addChild(tempSprite);

        // 飞行并放大
        auto moveAction = MoveTo::create(0.3f, Vec2(_visibleSize.width * 0.5f, tempSprite->getContentSize().height / 3));
        auto scaleAction = ScaleTo::create(0.3f, 1.0f);
        auto spawn = Spawn::create(moveAction, scaleAction, nullptr);

        // 动画结束后：加入到手牌、移除临时精灵并递归调用下一张
        auto finish = CallFunc::create([this, drawnCard, tempSprite, count]() {
            _cards.push_back(drawnCard);
            tempSprite->removeFromParent();
            updateHandDisplay();
            drawSequentialCards(count - 1);
            });

        tempSprite->runAction(Sequence::create(spawn, finish, nullptr));
    }
}

void FightingScene::playHeroAttackAnimation()
{
    if (!_hero) return;

    // 保存英雄原始位置
    Vec2 originalPos = _hero->getPosition();

    // 计算向怪物方向冲锋的目标位置
    Vec2 monsterPos = _monster->getPosition();
    Vec2 direction = monsterPos - originalPos;
    direction.normalize();

    // 冲刺距离，可以调整以得到最佳效果
    float dashDistance = 150.0f;
    Vec2 dashPos = originalPos + direction * dashDistance;

    // 创建动作序列：
    // 1. 快速冲向前方
    // 2. 短暂停顿
    // 3. 返回原位
    auto dashForward = EaseSineOut::create(MoveTo::create(0.1f, dashPos));
    auto pause = DelayTime::create(0.1f);
    auto dashBack = EaseSineIn::create(MoveTo::create(0.1f, originalPos));

    
    // 组合动作
    auto dashAction = Sequence::create(
        Spawn::create(dashForward, nullptr),
        pause,
        Spawn::create(dashBack, nullptr),
        nullptr
    );

    // 执行动画
    _hero->runAction(dashAction);

    // 添加攻击音效
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/sword_swing.mp3");
}


void FightingScene::playMonsterAttackAnimation()
{
    if (!_monster) return;

    // 保存怪物原始位置
    Vec2 originalPos = _monster->getPosition();

    // 计算向英雄方向冲锋的目标位置
    Vec2 heroPos = _hero->getPosition();
    Vec2 direction = heroPos - originalPos;
    direction.normalize();

    // 冲刺距离，可以调整以得到最佳效果
    float dashDistance = 150.0f;
    Vec2 dashPos = originalPos + direction * dashDistance;

    // 创建动作序列：
    // 1. 快速冲向英雄
    // 2. 短暂停顿
    // 3. 返回原位
    auto dashForward = EaseSineOut::create(MoveTo::create(0.1f, dashPos));
    auto pause = DelayTime::create(0.1f);
    auto dashBack = EaseSineIn::create(MoveTo::create(0.1f, originalPos));

    // 组合动作
    auto dashAction = Sequence::create(
        Spawn::create(dashForward, nullptr),
        pause,
        Spawn::create(dashBack, nullptr),
        nullptr
    );

    // 执行动画
    _monster->runAction(dashAction);

    // 添加攻击音效
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/monster_attack.mp3");
}

void FightingScene::playHeroHitAnimation()
{
    if (!_hero) return;

    // 保存英雄的原始位置
    Vec2 originalPos = _hero->getPosition();

    // 定义左右摇晃的偏移量
    float shakeOffset = 20.0f;

    // 创建左右摇晃的动作序列
    auto moveLeft = MoveTo::create(0.05f, Vec2(originalPos.x - shakeOffset, originalPos.y));
    auto moveRight = MoveTo::create(0.05f, Vec2(originalPos.x + shakeOffset, originalPos.y));
    auto moveBack = MoveTo::create(0.05f, originalPos);

    // 重复左右摇晃几次
    auto shakeSequence = Sequence::create(moveLeft, moveRight, nullptr);
    auto repeatShake = Repeat::create(shakeSequence, 3);

    // 最后回到原始位置
    auto resetPosition = MoveTo::create(0.05f, originalPos);

    // 组合动作
    auto hitAction = Sequence::create(repeatShake, resetPosition, nullptr);

    // 执行动作
    _hero->runAction(hitAction);
}

void FightingScene::playMonsterHitAnimation()
{
    if (!_monster) return;

    // 保存怪物的原始位置
    Vec2 originalPos = _monster->getPosition();

    // 定义左右摇晃的偏移量
    float shakeOffset = 20.0f;

    // 创建左右摇晃的动作序列
    auto moveLeft = MoveTo::create(0.05f, Vec2(originalPos.x - shakeOffset, originalPos.y));
    auto moveRight = MoveTo::create(0.05f, Vec2(originalPos.x + shakeOffset, originalPos.y));
    auto moveBack = MoveTo::create(0.05f, originalPos);

    // 重复左右摇晃几次
    auto shakeSequence = Sequence::create(moveLeft, moveRight, nullptr);
    auto repeatShake = Repeat::create(shakeSequence, 3);

    // 最后回到原始位置
    auto resetPosition = MoveTo::create(0.05f, originalPos);

    // 组合动作
    auto hitAction = Sequence::create(repeatShake, resetPosition, nullptr);

    // 执行动作
    _monster->runAction(hitAction);
}





