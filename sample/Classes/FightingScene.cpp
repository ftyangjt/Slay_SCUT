#include "FightingScene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "Map.h"
#include "fail.h"
#include "Effect.h"

USING_NS_CC;

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

    // 初始化悬停相关变量
    _hoveringCardIndex = -1;
    _cardOriginalPositions.clear();
    _cardOriginalRotations.clear();
	_cardOriginalZOrders.clear();
    _cardIsHovering.clear();

    // 创建背景
    createBackground();

    // 创建角色和怪物
    createCharacters();

    // 确保英雄和怪物对象都创建成功
    if (!_hero || !_monster) {
        CCLOG("Failed to create hero or monster, aborting scene initialization");
        return false;
    }
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
    // 添加详细日志，输出当前房间类型
    CCLOG("Current Room Type in createCharacters(): %d", static_cast<int>(MyGame::currentRoomType));
    Monster* monster = nullptr;

    try {
        // 修改判断逻辑，先打印出房间类型，再进行判断
        int roomTypeInt = static_cast<int>(MyGame::currentRoomType);
        CCLOG("Room type as integer: %d", roomTypeInt);

        if (MyGame::currentRoomType == MyGame::RoomType::BOSS) {
            CCLOG("Creating BOSS monster");
            monster = Monster::createRandom(true, false); // 创建Boss怪物
        }
        else if (MyGame::currentRoomType == MyGame::RoomType::ELITE) {
            CCLOG("Creating ELITE monster");
            monster = Monster::createRandom(false, true); // 创建精英怪物
        }
        else {
            CCLOG("Creating NORMAL monster for room type: %d", roomTypeInt);
            monster = Monster::createRandom(false, false); // 显式传递参数，创建普通怪物
        }

        // 验证创建的怪物类型
        if (monster) {
            CCLOG("Created monster type: %s", monster->getMonsterName().c_str());
            if (monster->isBoss()) {
                CCLOG("Confirmed: It's a BOSS monster");
            }
            else if (monster->isElite()) {
                CCLOG("Confirmed: It's an ELITE monster");
            }
            else {
                CCLOG("Confirmed: It's a NORMAL monster");
            }
        }
    }
    catch (const std::exception& e) {
        CCLOG("Exception creating monster: %s", e.what());
    }

    // 如果随机创建失败，使用默认怪物作为后备方案
    if (monster == nullptr)
    {
        CCLOG("Random monster creation failed, falling back to default monster");
        monster = SlimeMonster::create(); // 使用史莱姆作为默认怪物
    }

    // 如果仍然失败，报错并返回
    if (monster == nullptr)
    {
        problemLoading("Unable to create any monster");
        return;
    }

    _monster = monster;
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


// 在updateHealthAndBlockLabels函数中添加安全检查
void FightingScene::updateHealthAndBlockLabels()
{
    // 确保英雄和怪物对象都有效
    if (!_hero || !_monster) {
        CCLOG("Warning: Hero or Monster is nullptr in updateHealthAndBlockLabels");
        return;
    }

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
    _currentCost = 3;
    _selectedCardIndex = -1; // 确保重置选中卡牌
    _turnCountLabel->setString("Turn: " + std::to_string(_turnCount));
    _costLabel->setString("Cost: " + std::to_string(_currentCost));
    updateHandDisplay();

    // 禁用结束回合按钮，因为将要开始抽牌
    _isEndTurnButtonEnabled = false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 如果按钮不存在，创建它
    if (_endTurnButton == nullptr)
    {
        _endTurnButton = cocos2d::ui::Button::create("button.png", "button_selected.png");
        if (_endTurnButton == nullptr)
        {
            problemLoading("'buttonNormal.png' or 'buttonSelected.png'");
            return;
        }

        _endTurnButton->setScale(0.25f);
        _endTurnButton->setPosition(Vec2(origin.x + visibleSize.width - _endTurnButton->getContentSize().width / 2,
            origin.y + visibleSize.width / 2 - _endTurnButton->getContentSize().height / 2));

        // 添加点击事件监听器
        _endTurnButton->addClickEventListener([this](Ref* sender) {
            // 严格检查冷却状态
            if (!_isEndTurnButtonEnabled) {
                return; // 如果处于冷却状态，直接返回不执行任何操作
            }

            // 立即设置冷却状态以防止连续点击
            _isEndTurnButtonEnabled = false;

            // 获取按钮对象并禁用它
            auto button = static_cast<ui::Button*>(sender);
            button->setEnabled(false);

            // 视觉反馈 - 可选：改变按钮颜色为灰色
            button->setColor(Color3B(150, 150, 150));

            // 创建一个动作序列，延迟后重新启用按钮
            this->runAction(Sequence::create(
                DelayTime::create(1.0f),
                CallFunc::create([this, button]() {
                    // 只有在玩家回合时才重新启用按钮
                    if (_isPlayerTurn && _selectedCardIndex == -1) {
                        _isEndTurnButtonEnabled = true;
                        button->setEnabled(true);
                        button->setColor(Color3B::WHITE); // 恢复原色
                    }
                    }),
                nullptr
            ));

            // 最后调用回合结束函数
            this->endTurn();
            });

        this->addChild(_endTurnButton, 1);
    }
    else
    {
        _endTurnButton->setVisible(true);
        _endTurnButton->setEnabled(false); // 初始禁用
        _endTurnButton->setColor(Color3B(150, 150, 150)); // 灰色
    }

    // 开始抽牌
    drawSequentialCards(5);
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

    // 创建一个延迟后执行怪物回合的序列
    auto delay = DelayTime::create(1.0f);
    auto monsterAction = CallFunc::create([=]() {
        // 使用怪物的行为模式
        if (_monster) {
            // 获取当前行为
            MonsterActionType action = _monster->getCurrentAction().type;


			if (action == MonsterActionType::ATTACK)
			{
				// 播放怪物攻击动画
				playMonsterAttackAnimation();
			}


            // 执行当前行为
            _monster->executeCurrentAction(_hero);

            // 更新UI
            updateHealthAndBlockLabels();
            updateBuffLabels();
        }
        });

    auto endTurnAction = CallFunc::create([this]() {
        this->endTurn();
        });

    // 执行延迟和回合逻辑的序列
    this->runAction(Sequence::create(monsterAction, delay, endTurnAction, nullptr));
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
        CCLOG("Hero is dead. Game Over.");

        // 创建失败信息标签
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();

        auto defeatLabel = Label::createWithTTF("你已阵亡！", "fonts/Marker Felt.ttf", 80);
        defeatLabel->setTextColor(Color4B::RED);
        defeatLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        this->addChild(defeatLabel, 10);

        // 添加延迟动作，然后切换到失败场景
        this->runAction(Sequence::create(
            DelayTime::create(0.1f),  // 延迟0.1秒
            CallFunc::create([]() {
                // 切换到失败场景
                auto failScene = FailScene::createScene();
                Director::getInstance()->replaceScene(TransitionFade::create(0.5f, failScene));
                }),
            nullptr
        ));
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


// 单张抽牌函数 - 修改版
void FightingScene::drawCard()
{
    // 开始抽牌时禁用结束回合按钮
    _isEndTurnButtonEnabled = false;
    if (_endTurnButton) {
        _endTurnButton->setEnabled(false);
        _endTurnButton->setColor(Color3B(150, 150, 150));
    }

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

            // 抽牌结束后，只有在没有选中卡牌时才启用结束回合按钮
            if (_selectedCardIndex == -1) {
                _isEndTurnButtonEnabled = true;
                if (_endTurnButton) {
                    _endTurnButton->setEnabled(true);
                    _endTurnButton->setColor(Color3B::WHITE);
                }
            }
            });

        tempSprite->runAction(Sequence::create(spawn, finish, nullptr));
    }
    else {
        // 如果没有卡牌可抽，也要恢复按钮状态
        if (_selectedCardIndex == -1) {
            _isEndTurnButtonEnabled = true;
            if (_endTurnButton) {
                _endTurnButton->setEnabled(true);
                _endTurnButton->setColor(Color3B::WHITE);
            }
        }
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

void FightingScene::updateHandDisplay()
{
    size_t newCount = _cards.size();

    // 清除现有卡牌精灵
        // 清除现有卡牌精灵
    for (auto sprite : _cardSprites)
    {
        sprite->removeFromParent();
    }
    _cardSprites.clear();
    _lastClickTimes.clear();
    
    // 重置悬停相关变量
    _hoveringCardIndex = -1;
    _cardOriginalPositions.clear();
    _cardOriginalRotations.clear();
    _cardOriginalZOrders.clear();
    _cardIsHovering.clear();  // 清空悬浮状态标志

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

        // 根据索引设置递增的Z顺序，确保卡牌从左到右依次叠加（右边的卡牌在上面）
        sprite->setLocalZOrder(i + 1); // 从1开始，让每张卡有唯一的Z顺序

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

        // 添加鼠标移动监听器（实现悬停效果）
        auto mouseListener = EventListenerMouse::create();
        mouseListener->onMouseMove = [this, i, sprite](EventMouse* event) {
            // 将鼠标坐标转换为节点坐标系
            Vec2 mousePos = Vec2(event->getCursorX(), event->getCursorY());

            // 检查鼠标是否在卡牌区域内
            if (sprite->getBoundingBox().containsPoint(mousePos))
            {
                // 如果鼠标在卡牌上，且不是当前已悬停的卡牌
                if (_hoveringCardIndex != i)
                {
                    // 如果有其他卡牌正在悬停，先复位它
                    if (_hoveringCardIndex != -1) {
                        resetHoverEffect(_hoveringCardIndex);
                    }

                    // 设置新的悬停卡牌
                    _hoveringCardIndex = i;
                    applyHoverEffect(i);
                }
            }
            else if (_hoveringCardIndex == i)
            {
                // 如果鼠标离开当前悬停的卡牌
                resetHoverEffect(i);
            }
            };
		// 添加点击监听器
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
        _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, sprite);
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

        // 新增：立即将该卡牌标记为不可悬浮状态
        _cardIsHovering[index] = true; // 设为true表示卡牌正在悬浮中，不能再次触发悬浮效果

        // 如果卡牌当前是悬浮状态，先让它回到原位
        if (_hoveringCardIndex == index) {
            // 立即重置卡牌位置但保持不可悬浮状态
            if (_cardOriginalPositions.find(index) != _cardOriginalPositions.end()) {
                Vec2 originalPos = _cardOriginalPositions[index];
                float originalRot = _cardOriginalRotations[index];
                int originalZOrder = (_cardOriginalZOrders.find(index) != _cardOriginalZOrders.end()) ?
                    _cardOriginalZOrders[index] : 1;

                // 立即重置位置、旋转和缩放，不使用动画
                cardSprite->stopAllActions();
                cardSprite->setPosition(originalPos);
                cardSprite->setRotation(originalRot);
                cardSprite->setScale(0.6f);
                cardSprite->setLocalZOrder(originalZOrder);

                // 重置当前悬停索引
                _hoveringCardIndex = -1;
            }
        }

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
            _selectedCardIndex = -1; // 重置选中的卡牌
            highlightSelectedCard();

            // 打出卡牌后，如果没有选中的卡牌，则启用结束回合按钮
            if (_selectedCardIndex == -1) {
                _isEndTurnButtonEnabled = true;
                if (_endTurnButton) {
                    _endTurnButton->setEnabled(true);
                    _endTurnButton->setColor(Color3B::WHITE);
                }
            }
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

// 递归依次抽多张牌的函数 - 修改版（移除选中卡牌的条件判断）
void FightingScene::drawSequentialCards(int count)
{
    // 第一次调用时初始化
    if (count > 0 && !_isDrawingCards) {
        _isDrawingCards = true;

        // 禁用结束回合按钮
        _isEndTurnButtonEnabled = false;
        if (_endTurnButton) {
            _endTurnButton->setEnabled(false);
            _endTurnButton->setColor(Color3B(150, 150, 150)); // 灰色表示禁用
        }
    }

    // 若抽牌结束
    if (count <= 0) {
        // 抽牌过程完全结束时重置标志并启用按钮
        if (_isDrawingCards) {
            _isDrawingCards = false;
            _isEndTurnButtonEnabled = true;
            _endTurnButton->setEnabled(true);
            _endTurnButton->setColor(Color3B(255, 255, 255));
        }
        return;
    }

    // 处理抽牌逻辑
    if (_drawPile.empty()) {
        _drawPile = _discardPile;
        _discardPile.clear();
        shuffleDrawPile();
    }

    if (!_drawPile.empty()) {
        // 从抽牌堆取一张牌
        Card drawnCard = _drawPile.back();
        _drawPile.pop_back();

        // 创建临时精灵显示抽牌动画
        auto tempSprite = Sprite::create("cardBackground.jpg");
        tempSprite->setPosition(_drawDeckButton->getPosition());
        tempSprite->setScale(0.0f);
        tempSprite->setLocalZOrder(9999);
        this->addChild(tempSprite);

        // 飞行并放大的动画
        auto moveAction = MoveTo::create(0.3f, Vec2(_visibleSize.width * 0.5f, tempSprite->getContentSize().height / 3));
        auto scaleAction = ScaleTo::create(0.3f, 1.0f);
        auto spawn = Spawn::create(moveAction, scaleAction, nullptr);

        // 动画结束后的回调函数
        auto finish = CallFunc::create([this, drawnCard, tempSprite, count]() {
            // 添加到手牌并移除临时精灵
            _cards.push_back(drawnCard);
            tempSprite->removeFromParent();

            // 确保更新手牌时按钮状态保持禁用
            bool oldState = _isEndTurnButtonEnabled;
            _isEndTurnButtonEnabled = false;

            updateHandDisplay();

            // 恢复按钮禁用状态，确保在整个抽牌过程中按钮都保持禁用
            _isEndTurnButtonEnabled = oldState;
            if (_endTurnButton) {
                _endTurnButton->setEnabled(false);
                _endTurnButton->setColor(Color3B(150, 150, 150));
            }

            // 继续抽下一张牌
            drawSequentialCards(count - 1);
            });

        // 执行动画序列
        tempSprite->runAction(Sequence::create(spawn, finish, nullptr));
    }
    else {
        // 如果没有牌可抽，也要结束递归
        drawSequentialCards(0);
    }
}

// 英雄和怪物的攻击动画
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

// 英雄和怪物受击动画
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

// 更新怪物意图显示
void FightingScene::createMonsterIntentLabel()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建怪物意图标签
    _monsterIntentLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", 40);
    _monsterIntentLabel->setTextColor(Color4B::ORANGE);
    _monsterIntentLabel->setPosition(Vec2(
        origin.x + 3 * visibleSize.width / 4,
        origin.y + visibleSize.height - _monsterHealthLabel->getContentSize().height - 210));
    this->addChild(_monsterIntentLabel, 1);

    updateMonsterIntentDisplay();
}

void FightingScene::updateMonsterIntentDisplay()
{
    if (!_monster || !_monsterIntentLabel) {
        return;
    }

    _monsterIntentLabel->setString("next" + _monster->getNextActionDescription());
}

// 修改 applyHoverEffect 方法，确保悬浮的卡牌处于最高图层
void FightingScene::applyHoverEffect(int cardIndex)
{
    if (cardIndex < 0 || cardIndex >= _cardSprites.size() || !_cardSprites[cardIndex])
        return;

    // 检查卡牌是否已经在悬浮中，如果是则不执行任何操作
    if (_cardIsHovering.find(cardIndex) != _cardIsHovering.end() && _cardIsHovering[cardIndex])
        return;

    // 如果有其他卡牌正在悬停，先重置它
    if (_hoveringCardIndex >= 0 && _hoveringCardIndex != cardIndex &&
        _hoveringCardIndex < _cardSprites.size() && _cardSprites[_hoveringCardIndex])
    {
        resetHoverEffect(_hoveringCardIndex);
    }

    // 获取当前卡牌
    auto sprite = _cardSprites[cardIndex];

    // 保存原始位置、旋转和Z顺序
    _cardOriginalPositions[cardIndex] = sprite->getPosition();
    _cardOriginalRotations[cardIndex] = sprite->getRotation();
    _cardOriginalZOrders[cardIndex] = sprite->getLocalZOrder(); // 保存原始z顺序

    // 停止所有正在执行的动作
    sprite->stopAllActions();

    // 使卡牌上浮并放大一点
    float hoverDistance = 30.0f; // 上浮距离
    auto moveUp = MoveTo::create(0.1f, Vec2(sprite->getPositionX(),
        sprite->getPositionY() + hoverDistance));
    auto scaleUp = ScaleTo::create(0.1f, sprite->getScale() * 1.1f);
    auto rotate = RotateTo::create(0.1f, 0.0f); // 使卡牌保持正面朝上

    // 设置为最高图层，确保在所有卡牌之上
    sprite->setLocalZOrder(1000);

    // 标记此卡牌为悬浮状态
    _cardIsHovering[cardIndex] = true;

    // 同时执行多个动作
    auto hoverAction = Spawn::create(moveUp, scaleUp, rotate, nullptr);
    sprite->runAction(hoverAction);

    // 更新当前悬停索引
    _hoveringCardIndex = cardIndex;
}

// 重置悬停效果
void FightingScene::resetHoverEffect(int cardIndex)
{
    if (cardIndex < 0 || cardIndex >= _cardSprites.size() || !_cardSprites[cardIndex])
        return;

    // 获取当前卡牌
    auto sprite = _cardSprites[cardIndex];

    // 检查是否有保存的原始位置和Z顺序
    if (_cardOriginalPositions.find(cardIndex) != _cardOriginalPositions.end())
    {
        // 记录原始位置和旋转以便恢复
        Vec2 originalPos = _cardOriginalPositions[cardIndex];
        float originalRot = _cardOriginalRotations[cardIndex];
        int originalZOrder = (_cardOriginalZOrders.find(cardIndex) != _cardOriginalZOrders.end()) ?
            _cardOriginalZOrders[cardIndex] : 1;

        // 确保先停止所有正在执行的动作
        sprite->stopAllActions();

        // 恢复原始位置、旋转和缩放
        auto moveBack = MoveTo::create(0.1f, originalPos);
        auto scaleBack = ScaleTo::create(0.1f, 0.6f); // 恢复原始缩放
        auto rotateBack = RotateTo::create(0.1f, originalRot); // 恢复原始旋转

        // 创建一个回调，确保重置完成后清理状态和恢复Z顺序
        auto resetCallback = CallFunc::create([this, sprite, cardIndex, originalZOrder]() {
            // 恢复Z顺序
            sprite->setLocalZOrder(originalZOrder);

            // 如果当前悬停索引是这张卡，将其重置
            if (_hoveringCardIndex == cardIndex) {
                _hoveringCardIndex = -1;
            }

            // 重要：标记卡牌不再处于悬浮状态，允许再次悬浮
            _cardIsHovering[cardIndex] = false;
            });

        // 使用Sequence确保所有动作按顺序完成
        auto resetAction = Sequence::create(
            Spawn::create(moveBack, scaleBack, rotateBack, nullptr),
            resetCallback,
            nullptr
        );

        sprite->runAction(resetAction);
    }
}

