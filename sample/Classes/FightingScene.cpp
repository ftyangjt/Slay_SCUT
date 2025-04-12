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

	// 更新回合数标签
    _turnCountLabel->setString("Turn: " + std::to_string(_turnCount));

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

// 设置弃牌堆按钮
void FightingScene::createDiscardDeck()
{
    auto showDiscardDeckButton = MenuItemImage::create(
        "showDiscard_Normal.png",
        "showDiscard_Selected.png",
        CC_CALLBACK_1(FightingScene::goToDiscardDeck, this)
    );
    showDiscardDeckButton->setScale(0.5f);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 设置按钮位置在画面右下角
    showDiscardDeckButton->setPosition(Vec2(origin.x + visibleSize.width - showDiscardDeckButton->getContentSize().width / 2,
        origin.y + showDiscardDeckButton->getContentSize().height / 2));
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
    showDrawDeckButton->setScale(0.5f);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 设置按钮位置在画面左下角
    showDrawDeckButton->setPosition(Vec2(origin.x + showDrawDeckButton->getContentSize().width / 2,
        origin.y + showDrawDeckButton->getContentSize().height / 2));
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

// 结束回合
void FightingScene::endTurn()
{
    if (_isPlayerTurn)
    {
        // 丢弃所有手牌至弃牌堆
        _discardPile.insert(_discardPile.end(), _cards.begin(), _cards.end());
        _cards.clear();
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
            DelayTime::create(0.1f),  // 延迟2秒
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

        // 创建胜利消息标签
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();

        auto victoryLabel = Label::createWithTTF("战斗胜利！", "fonts/Marker Felt.ttf", 80);
        victoryLabel->setTextColor(Color4B::YELLOW);
        victoryLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        this->addChild(victoryLabel, 10);

        // 添加延迟动作，然后返回地图场景
        this->runAction(Sequence::create(
            DelayTime::create(0.1f),  // 延迟2秒
            CallFunc::create([]() {
                // 返回到地图场景
                auto mapScene = MyGame::Map::createScene();
                Director::getInstance()->replaceScene(TransitionFade::create(0.5f, mapScene));
                }),
            nullptr
        ));
    }
}

// 以下为卡牌系统的实现
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

// 给卡牌添加效果标签
void FightingScene::addCardEffectLabel(cocos2d::Sprite* cardSprite, const std::string& effect)
{
    // 获取卡牌的宽度
    float cardWidth = cardSprite->getContentSize().width;

    // 创建多行标签，并设置最大宽度为卡牌宽度
    auto effectLabel = Label::createWithTTF(effect, "fonts/Marker Felt.ttf", 48, Size(cardWidth, 0), TextHAlignment::CENTER);
    effectLabel->setColor(cocos2d::Color3B::BLACK);

    // 设置标签位置为卡牌中心
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

		checkBattleEnd();

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