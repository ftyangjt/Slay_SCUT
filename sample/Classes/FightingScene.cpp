﻿#include "FightingScene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "Map.h"
#include "fail.h"
#include "Effect.h"
#include "Victory.h"

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

    // 创建怪物意图标签
    _monsterIntentLabel = cocos2d::Label::createWithTTF(
        _monster->getNextActionDescription(),
        "fonts/Marker Felt.ttf", 40); // 建议字号与BUFF栏一致
    _monsterIntentLabel->setTextColor(cocos2d::Color4B::ORANGE);

    // 设置标签位置在怪物BUFF栏下方
    auto buffLabelPos = _monsterBuffLabel->getPosition();
    float intentOffsetY = _monsterBuffLabel->getContentSize().height / 2 + _monsterIntentLabel->getContentSize().height / 2 + 10; // 10像素间距
    _monsterIntentLabel->setPosition(buffLabelPos.x, buffLabelPos.y - intentOffsetY);

    this->addChild(_monsterIntentLabel, 10);
    

    // 开始玩家回合
    startPlayerTurn();

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
    _costLabel->setPosition(Vec2(heroPosition.x, heroPosition.y - heroHeight / 2));
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
    background->setScale(0.7f); // 设置背景缩放为0.53倍
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
    Monster* monster = nullptr;

    try {
        if (MyGame::currentRoomType == MyGame::RoomType::BOSS) {
            monster = Monster::createRandom(true, false); // 创建Boss怪物
        }
        else if (MyGame::currentRoomType == MyGame::RoomType::ELITE) {
            monster = Monster::createRandom(false, true); // 创建精英怪物
        }
        else {
            monster = Monster::createRandom(); // 创建普通怪物
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

    // 根据怪物类型设置位置
    float monsterPosX = origin.x + visibleSize.width - _monster->getContentSize().width / 2;

    // 判断怪物类型
    if (_monster->isElite()) {
        // 精英怪物向左移动200像素
        monsterPosX -= 200.0f;
    }
    else if (!_monster->isBoss()) {
        // 普通怪物向右移动200像素（Boss怪物保持原位）
        monsterPosX += 200.0f;
    }

    _monster->setPosition(Vec2(monsterPosX, origin.y + visibleSize.height / 2));
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

// 更新怪物意图显示标签
void FightingScene::updateMonsterIntentDisplay()
{
    if (!_monster || !_monsterIntentLabel || !_monsterBuffLabel) return;
    _monsterIntentLabel->setString(_monster->getNextActionDescription());

    // 固定在怪物BUFF栏下方
    auto buffLabelPos = _monsterBuffLabel->getPosition();
    float intentOffsetY = _monsterBuffLabel->getContentSize().height / 2 + _monsterIntentLabel->getContentSize().height / 2 + 10;
    _monsterIntentLabel->setPosition(buffLabelPos.x, buffLabelPos.y - intentOffsetY);
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
        _endTurnButton = cocos2d::ui::Button::create("Next.png", "Next.png");
        if (_endTurnButton == nullptr)
        {
            problemLoading("'buttonNormal.png' or 'buttonSelected.png'");
            return;
        }

        _endTurnButton->setScale(0.25f);
        _endTurnButton->setPosition(Vec2(origin.x + visibleSize.width - _endTurnButton->getContentSize().width / 4+100,
            origin.y + visibleSize.width / 2 - _endTurnButton->getContentSize().height / 3));

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
        "showDiscard_Normal.png",
        CC_CALLBACK_1(FightingScene::goToDiscardDeck, this)
    );
    showDiscardDeckButton->setScale(0.25f);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 设置按钮位置在画面右上角
    showDiscardDeckButton->setPosition(Vec2(
        origin.x + visibleSize.width - showDiscardDeckButton->getContentSize().width * 0.125f, // 0.25缩放的一半
		origin.y + visibleSize.height - showDiscardDeckButton->getContentSize().height * 0.125f // 0.25缩放的一半
    ));
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
        "showDraw_Normal.png",
        CC_CALLBACK_1(FightingScene::goToDrawDeck, this)
    );
    showDrawDeckButton->setScale(0.25f);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 设置按钮位置在画面左下角
    showDrawDeckButton->setPosition(Vec2(
        origin.x + showDrawDeckButton->getContentSize().width * 0.125f, // 0.25缩放的一半
        origin.y + visibleSize.height - showDrawDeckButton->getContentSize().height * 0.125f
    ));

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
        updateMonsterIntentDisplay();
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

        // 先禁用怪物的所有动作
        _monster->stopAllActions();

        // 创建淡出动画
        auto fadeOut = FadeOut::create(0.8f); // 0.8秒淡出
        auto scaleDown = ScaleTo::create(0.8f, 0.1f); // 同时缩小
        auto disappear = Spawn::create(fadeOut, scaleDown, nullptr);

        // 动画结束后执行胜利奖励逻辑
        auto afterDisappear = CallFunc::create([this]() {
            // 根据当前房间类型设置金币奖励
            int coinReward = 20;
            if (MyGame::currentRoomType == MyGame::RoomType::ELITE) {
                coinReward = 40;
            }
            else if (MyGame::currentRoomType == MyGame::RoomType::BOSS) {
                coinReward = 100;
            }
            Hero::addCoins(coinReward);

            auto visibleSize = Director::getInstance()->getVisibleSize();
            Vec2 origin = Director::getInstance()->getVisibleOrigin();

            auto coinLabel = Label::createWithTTF("Gain " + std::to_string(coinReward) + " Coins", "fonts/Marker Felt.ttf", 60);
            coinLabel->setTextColor(Color4B::YELLOW);
            coinLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 100));
            this->addChild(coinLabel, 10);

            auto victoryLabel = Label::createWithTTF("YOU WIN！", "fonts/Marker Felt.ttf", 80);
            victoryLabel->setTextColor(Color4B::YELLOW);
            victoryLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
            this->addChild(victoryLabel, 10);

            // 生成3张可选卡牌
            std::vector<Card> rewardCards = generateRandomCards(3);

            // 延迟1秒后再弹出选牌界面
            this->runAction(Sequence::create(
                DelayTime::create(1.0f),
                CallFunc::create([this, rewardCards]() {
                    showCardSelectionWithCallback(rewardCards, [this]() {
                        if (MyGame::currentRoomType == MyGame::RoomType::BOSS) {
                            auto victoryScene = MyGame::VictoryScene::createScene();
                            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, victoryScene));
                        }
                        else {
                            auto mapScene = MyGame::Map::createScene();
                            Director::getInstance()->replaceScene(TransitionFade::create(0.5f, mapScene));
                        }
                        });
                    }),
                nullptr
            ));
            });

        // 执行动画和后续逻辑
        _monster->runAction(Sequence::create(disappear, afterDisappear, nullptr));
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
            tempSprite->stopAllActions(); // 先停止所有动作
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

//void FightingScene::discardCard(const Card& card)
//{
//    auto it = std::find_if(_cards.begin(), _cards.end(),
//        [&card](const Card& c) { return &c == &card; });
//    if (it != _cards.end()) {
//        _discardPile.push_back(*it);
//        _cards.erase(it);
//        updateHandDisplay();
//    }
//}


void FightingScene::exhaustCard(int index)
{
    if (index >= 0 && index < _cards.size())
    {
        // 将卡牌放入消耗牌堆
        _exhaustPile.push_back(_cards[index]);
        _cards.erase(_cards.begin() + index);
        updateHandDisplay(); // 更新手牌显示
    }
}

//void FightingScene::exhaustCard(const Card& card)
//{
//    auto it = std::find_if(_cards.begin(), _cards.end(),
//        [&card](const Card& c) { return &c == &card; });
//    if (it != _cards.end()) {
//        _exhaustPile.push_back(*it);
//        _cards.erase(it);
//        updateHandDisplay();
//    }
//}

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
    // 获取卡牌的宽度和高度
    float cardWidth = cardSprite->getContentSize().width;
    float cardHeight = cardSprite->getContentSize().height;

    // 根据文本长度自动调整字体大小
    int fontSize = 48;

    // 创建多行标签，设置最大宽度为卡牌宽度的85%
    auto effectLabel = Label::createWithTTF(effect, "fonts/Marker Felt.ttf", fontSize,
        Size(cardWidth * 0.85f, 0), TextHAlignment::CENTER);
    effectLabel->setColor(cocos2d::Color3B::BLACK);

    // 将文本位置稍微向上移动，使其更适合卡牌布局
    float yPosition = cardSprite->getContentSize().height / 2 + 10;

    effectLabel->setPosition(Vec2(cardWidth / 2, yPosition));

    // 添加自动缩放功能，确保文本不会超出卡牌可用区域的50%
    float maxHeight = cardHeight * 0.5f; // 限制文本高度为卡牌高度的50%
    if (effectLabel->getContentSize().height > maxHeight) {
        float scale = maxHeight / effectLabel->getContentSize().height;
        effectLabel->setScale(scale);
    }

    cardSprite->addChild(effectLabel, 1);

    return effectLabel; // 返回效果标签
}

void FightingScene::updateHandDisplay()
{
    size_t newCount = _cards.size();

    // 清除现有卡牌精灵
    for (auto sprite : _cardSprites)
    {
        sprite->stopAllActions();
        sprite->removeFromParent();
    }
    _cardSprites.clear();
    _lastClickTimes.clear();

    _disabledCardIndices.clear();
    
    // 重置悬停相关变量
    _hoveringCardIndex = -1;
    _cardOriginalPositions.clear();
    _cardOriginalRotations.clear();
    _cardOriginalZOrders.clear();
    _cardIsHovering.clear();  // 清空悬浮状态标志

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

        // 根据索引设置递增的Z顺序，确保卡牌从左到右依次叠加（右边的卡牌在上面）
        sprite->setLocalZOrder(i + 1); // 从1开始，让每张卡有唯一的Z顺序

        // 设置卡牌位置和固定缩放比例
        sprite->setPosition(Vec2(posX, _origin.y + scaledCardHeight * 0.4f));
        sprite->setScale(cardScale);
        this->addChild(sprite, 1);
        _cardSprites.push_back(sprite);
        _lastClickTimes.push_back(std::chrono::steady_clock::now());
        // 添加卡牌标题标签（在卡牌顶部居中）
        auto titleLabel = Label::createWithTTF(_cards[i].getName(), "fonts/Marker Felt.ttf", 60);
        titleLabel->setTextColor(Color4B::BLACK);
        // 设置标题在卡牌顶部
        titleLabel->setPosition(Vec2(sprite->getContentSize().width / 2, sprite->getContentSize().height - 80));
        sprite->addChild(titleLabel, 2);

        // 添加卡牌效果标签
        auto effectLabel = addCardEffectLabel(sprite, _cards[i].getEffect());

        // 添加费用标签
        auto costLabel = Label::createWithTTF("Cost: " + std::to_string(_cards[i].getCost()), "fonts/Marker Felt.ttf", 40);
        costLabel->setTextColor(Color4B::ORANGE);
        costLabel->setPosition(Vec2(sprite->getContentSize().width / 2, effectLabel->getPositionY() - 100));
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
                // 如果是技能牌，跳过力量效果的应用
                if (cardType != Card::Type::Skill) {
                    damage += buff->getLevel(); // 力量效果增加攻击力
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
                if (isTargetMonster) {
                    damage = static_cast<int>(damage * 1.5); // 易伤效果增加伤害
                }
                break;
            default:
                break;
            }
        }
    }
}


void FightingScene::applyCardEffects(const Card& card)
{
    int damage = card.getAttack();
    int block = card.getBlock();

    // 应用英雄的效果（如力量），传入卡牌类型
    applyEffects(damage, block, _hero->getEffects(), card.getType(), false);

    // 应用怪物的效果（如易伤），传入卡牌类型
    applyEffects(damage, block, _monster->getEffects(), card.getType(), true);

    // 处理卡牌自带的效果
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
                _monster->addEffect(effect); // 易伤效果应用到怪物
                break;
            default:
                break;
            }
        }
    }

    // 处理特殊效果
    if (card.hasSpecialEffect(Card::SpecialEffect::DrawCard)) {
        int cardsToDraw = card.getSpecialEffectValue(Card::SpecialEffect::DrawCard);
        for (int i = 0; i < cardsToDraw; i++) {
            this->runAction(Sequence::create(
                DelayTime::create(0.3f + i * 0.2f),
                CallFunc::create([this]() { this->drawCard(); }),
                nullptr
            ));
        }
    }

    if (card.hasSpecialEffect(Card::SpecialEffect::LoseHealth)) {
        int healthLoss = card.getSpecialEffectValue(Card::SpecialEffect::LoseHealth);
        _hero->setHealth(_hero->getHealth() - healthLoss);
        playHeroHitAnimation();
        updateHealthAndBlockLabels();
        checkBattleEnd();
    }

    if (card.hasSpecialEffect(Card::SpecialEffect::GainEnergy)) {
        int energyGain = card.getSpecialEffectValue(Card::SpecialEffect::GainEnergy);
        _currentCost += energyGain;
        updateCostLabel();
    }

    if (card.hasSpecialEffect(Card::SpecialEffect::DiscardCard)) {
        int cardsToDiscard = card.getSpecialEffectValue(Card::SpecialEffect::DiscardCard);
        for (int i = 0; i < cardsToDiscard && !_cards.empty(); i++) {
            discardCard(0); // 弃掉第一张牌
        }
    }

    // 更新怪物的格挡
    int monsterBlock = _monster->getBlock();
    if (monsterBlock > 0) {
        if (monsterBlock >= damage) {
            _monster->setBlock(monsterBlock - damage);
            damage = 0;
        }
        else {
            damage -= monsterBlock;
            _monster->setBlock(0);
        }
    }

    // 更新怪物的生命值
    if (damage > 0) {
        playMonsterHitAnimation();
        _monster->setHealth(_monster->getHealth() - damage);
    }

    // 更新英雄的格挡
    if (block > 0) {
        _hero->setBlock(_hero->getBlock() + block);
    }

    // 更新UI
    updateHealthAndBlockLabels();
    updateBuffLabels();
    checkBattleEnd();
}



// 打出卡牌
void FightingScene::playCard(int index)
{
    if (_isSelectingCard) return;
    if (_isCooldown) return;
    if (_isAnimating) return; // 动画期间禁止操作
    if (_disabledCardIndices.find(index) != _disabledCardIndices.end()) return;

    // 新增：打牌时禁用所有手牌
    for (size_t i = 0; i < _cards.size(); ++i) {
        _disabledCardIndices.insert(i);
    }

    if (index >= 0 && index < _cards.size()) {
        // 立即将卡牌添加到禁用列表
        _disabledCardIndices.insert(index);

        // 原逻辑不变
        Card playedCard = _cards[index];

        CCLOG("Played\"%s\"，at index %d", playedCard.getName().c_str(), index);


        if (!playedCard.isPlayable()) {
            CCLOG("This card is not playable!");
            // 新增：如果不能打出，恢复所有手牌可用
            _disabledCardIndices.clear();
            return;
        }

        int cost = playedCard.getCost();
        // 检查能量是否足够
        if (cost > _currentCost)
        {
            CCLOG("Energy not enough to play this card!");
            _disabledCardIndices.clear();
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

        if (_cards[index].isExhaust()) {
            _isAnimating = true; // 动画开始前加锁
            // 消耗牌动画：渐隐+缩小+旋转
            auto fadeOut = FadeOut::create(0.4f);
            auto scaleDown = ScaleTo::create(0.4f, 0.1f);
            auto rotate = RotateBy::create(0.4f, 180);
            auto exhaustAnim = Spawn::create(fadeOut, scaleDown, rotate, nullptr);

            auto finish = CallFunc::create([this, index, cardSprite]() {
                cardSprite->stopAllActions(); // 先停止所有动作
                cardSprite->removeFromParent();
                exhaustCard(index); // 进消耗牌堆
                _selectedCardIndex = -1;
                highlightSelectedCard();
                if (_selectedCardIndex == -1) {
                    _isEndTurnButtonEnabled = true;
                    if (_endTurnButton) {
                        _endTurnButton->setEnabled(true);
                        _endTurnButton->setColor(Color3B::WHITE);
                    }
                }
                _isAnimating = false; // 动画结束，解除锁定
                _disabledCardIndices.clear();
                });

            cardSprite->runAction(Sequence::create(exhaustAnim, finish, nullptr));
        }
        else {
            // 原有弃牌动画
            _isAnimating = true; // 动画开始前加锁
            Vec2 discardPos = _discardDeckButton->getPosition();
            auto moveAction = MoveTo::create(0.3f, discardPos);
            auto scaleAction = ScaleTo::create(0.3f, 0.1f);
            auto moveAndScale = Spawn::create(moveAction, scaleAction, nullptr);

            auto finish = CallFunc::create([this, index, cardSprite]() {
                cardSprite->stopAllActions(); // 先停止所有动作
                cardSprite->removeFromParent();
                discardCard(index); // 进弃牌堆
                _selectedCardIndex = -1;
                highlightSelectedCard();
                if (_selectedCardIndex == -1) {
                    _isEndTurnButtonEnabled = true;
                    if (_endTurnButton) {
                        _endTurnButton->setEnabled(true);
                        _endTurnButton->setColor(Color3B::WHITE);
                    }
                }
                _isAnimating = false; // 动画结束，解除锁定
                _disabledCardIndices.clear();
                });

            cardSprite->runAction(Sequence::create(moveAndScale, finish, nullptr));
        }

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
    if (_isSelectingCard) return; // 选牌时禁止操作

    if (_disabledCardIndices.find(cardIndex) != _disabledCardIndices.end()) {
        return; // 卡牌处于禁用状态，不响应点击
    }

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
            meteor->stopAllActions();
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
            tempSprite->stopAllActions(); // 先停止所有动作
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

std::vector<Card> FightingScene::generateRandomCards(int count) {
    std::vector<Card> randomCards;
    std::set<std::string> cardNames; // 用于判重，假设卡牌名唯一

    int tryLimit = 100; // 防止死循环
    while (randomCards.size() < count && tryLimit-- > 0) {
        Card card = CardLibrary::getRandomNonInitialNonCurseCard();
        if (cardNames.count(card.getName()) == 0) {
            randomCards.push_back(card);
            cardNames.insert(card.getName());
        }
    }
    return randomCards;
}

void FightingScene::showCardSelectionWithCallback(const std::vector<Card>& cards, const std::function<void()>& onSelectionComplete)
{
    _isEndTurnButtonEnabled = false;
    _isSelectingCard = true;
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // 半透明背景
    auto background = LayerColor::create(Color4B(0, 0, 0, 180));
    this->addChild(background, 10);

    // 提示文字
    auto tipLabel = Label::createWithTTF("Please select a card to add to your deck", "fonts/Marker Felt.ttf", 48);
    tipLabel->setTextColor(Color4B::YELLOW);
    tipLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.82f));
    background->addChild(tipLabel, 100);

    // 跳过选牌按钮
    auto skipButton = cocos2d::ui::Button::create("Next.png", "Next.png");
    skipButton->setScale(0.25f);
    skipButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.18f));
    auto skipLabel = Label::createWithTTF("Skip", "fonts/Marker Felt.ttf", 36);
    skipLabel->setTextColor(Color4B::WHITE);
    skipLabel->setPosition(skipButton->getContentSize().width / 2, skipButton->getContentSize().height / 2);
    skipButton->addChild(skipLabel);
    skipButton->addClickEventListener([background, this, onSelectionComplete](Ref*) {
        background->removeFromParent();
        _isSelectingCard = false;
        _isEndTurnButtonEnabled = true;
        if (onSelectionComplete) {
            onSelectionComplete();
        }
        });
    background->addChild(skipButton, 101);

    // 获取卡牌原始尺寸
    auto tempSprite = Sprite::create("cardBackground.jpg");
    float originalCardWidth = tempSprite->getContentSize().width;
    float originalCardHeight = tempSprite->getContentSize().height;
    float cardScale = 0.9f; // 放大
    float scaledCardWidth = originalCardWidth * cardScale;
    float scaledCardHeight = originalCardHeight * cardScale;
    float cardSpacing = 60.0f; // 间隔

    size_t cardCount = cards.size();
    // 计算总宽度
    float totalWidth = cardCount * scaledCardWidth + (cardCount - 1) * cardSpacing;
    float startX = origin.x + (visibleSize.width - totalWidth) / 2 + scaledCardWidth / 2;

    for (size_t i = 0; i < cardCount; ++i) {
        auto cardSprite = Sprite::create("cardBackground.jpg");
        float posX = startX + i * (scaledCardWidth + cardSpacing);
        float posY = origin.y + visibleSize.height / 2;
        cardSprite->setPosition(Vec2(posX, posY));
        cardSprite->setScale(cardScale);
        cardSprite->setLocalZOrder(i + 1);
        background->addChild(cardSprite);

        // 标题
        auto titleLabel = Label::createWithTTF(cards[i].getName(), "fonts/Marker Felt.ttf", 60);
        titleLabel->setTextColor(Color4B::BLACK);
        titleLabel->setPosition(Vec2(cardSprite->getContentSize().width / 2, cardSprite->getContentSize().height - 80));
        cardSprite->addChild(titleLabel, 2);

        // 效果
        auto effectLabel = addCardEffectLabel(cardSprite, cards[i].getEffect());

        // 费用
        auto costLabel = Label::createWithTTF("Cost: " + std::to_string(cards[i].getCost()), "fonts/Marker Felt.ttf", 40);
        costLabel->setTextColor(Color4B::ORANGE);
        costLabel->setPosition(Vec2(cardSprite->getContentSize().width / 2, effectLabel->getPositionY() - 100));
        cardSprite->addChild(costLabel, 1);

        // 点击事件
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [this, background, card = cards[i], onSelectionComplete, cardSprite](Touch* touch, Event* event) -> bool {
            auto target = static_cast<Sprite*>(event->getCurrentTarget());
            if (target->getBoundingBox().containsPoint(touch->getLocation())) {
                // 1. 播放动画：放大+上浮+渐隐
                float animScale = 1.3f;
                float moveUp = 80.0f;
                auto scaleUp = ScaleTo::create(0.18f, animScale);
                auto moveUpAction = MoveBy::create(0.18f, Vec2(0, moveUp));
                auto fadeOut = FadeOut::create(0.18f);
                auto anim = Spawn::create(scaleUp, moveUpAction, fadeOut, nullptr);

                // 2. 动画结束后加卡并关闭界面
                auto finish = CallFunc::create([background, this, card, onSelectionComplete]() {
                    background->stopAllActions();
                    _hero->addCardToDeck(card);
                    background->removeFromParent();
                    _isSelectingCard = false;
                    _isEndTurnButtonEnabled = true;
                    CCLOG("Card added to deck: %s", card.getEffect().c_str());
                    if (onSelectionComplete) {
                        onSelectionComplete();
                    }
                    });

                cardSprite->runAction(Sequence::create(anim, finish, nullptr));
                return true;
            }
            return false;
            };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, cardSprite);
    }
}


// 更新怪物意图显示
void FightingScene::createMonsterIntentLabel()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建怪物意图标签
    _monsterIntentLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", 80);
    _monsterIntentLabel->setTextColor(Color4B::ORANGE);
    _monsterIntentLabel->setPosition(Vec2(
        origin.x + 3 * visibleSize.width / 4,
        origin.y + visibleSize.height - _monsterHealthLabel->getContentSize().height - 210));
    this->addChild(_monsterIntentLabel, 1);

    updateMonsterIntentDisplay();
}

// 修改 applyHoverEffect 方法，确保悬浮的卡牌处于最高图层
void FightingScene::applyHoverEffect(int cardIndex)
{
    if (_isSelectingCard) return; // 选牌时禁止悬浮
    if (_isAnimating) return; // 动画期间禁止悬浮

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
    if (_isAnimating) return; // 动画期间禁止悬浮

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

