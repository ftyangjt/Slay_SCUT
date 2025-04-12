#include "FightingScene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "Map.h"
#include "fail.h"
#include "Effect.h"

USING_NS_CC;

// ����Ϊ��ʼ��������ʵ��
// ��������
Scene* FightingScene::createScene()
{
    return FightingScene::create();
}

// ��������Դ�Ƿ����
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in FightingScene.cpp\n");
}

// ��ʼ������
bool FightingScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    _visibleSize = Director::getInstance()->getVisibleSize();
    _origin = Director::getInstance()->getVisibleOrigin();

    // ��������
    createBackground();

    // ������ɫ�͹���
    createCharacters();

	// ���ó��ƶѰ�ť
    createDrawDeck();

	//�������ƶѰ�ť
    createDiscardDeck();


    // ��ʼ�����ƶѺ����ƶ�
    initializeDrawPile();

    // ����Ѫ����ǩ
    createHealthLabels();

    // �����񵲱�ǩ
    createBlockLabels();

	// ����BUFF��DEBUFF��ǩ
    createBuffLabels();

	// ����Ѫ����ǩ
    updateHealthAndBlockLabels();

	// ��ʼ���غ���
    _turnCount = 1;

	// �����غ�����ǩ
    createTurnCountLabel();

    

    // ��ʼ��һغ�
    startPlayerTurn();

    return true;
}

// ����Ѫ����ǩ
void FightingScene::createHealthLabels()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // ����Ӣ��Ѫ����ǩ
    _heroHealthLabel = Label::createWithTTF("Hero Health: 100", "fonts/Marker Felt.ttf", 60);
    _heroHealthLabel->setTextColor(Color4B::RED); // ���ñ�ǩ��ɫΪ��ɫ
    _heroHealthLabel->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + visibleSize.height - _heroHealthLabel->getContentSize().height));
    this->addChild(_heroHealthLabel, 1);

    // ��������Ѫ����ǩ
    _monsterHealthLabel = Label::createWithTTF("Monster Health: 100", "fonts/Marker Felt.ttf", 60);
    _monsterHealthLabel->setTextColor(Color4B::RED); // ���ñ�ǩ��ɫΪ��ɫ
    _monsterHealthLabel->setPosition(Vec2(origin.x + 3 * visibleSize.width / 4, origin.y + visibleSize.height - _monsterHealthLabel->getContentSize().height));
    this->addChild(_monsterHealthLabel, 1);
}

// �����񵲱�ǩ
void FightingScene::createBlockLabels()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // ����Ӣ�۸񵲱�ǩ
    _heroBlockLabel = Label::createWithTTF("Hero Block: 0", "fonts/Marker Felt.ttf", 60);
    _heroBlockLabel->setTextColor(Color4B::BLUE); // ���ñ�ǩ��ɫΪ��ɫ
    _heroBlockLabel->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + visibleSize.height - _heroHealthLabel->getContentSize().height - 70));
    this->addChild(_heroBlockLabel, 1);

    // ��������񵲱�ǩ
    _monsterBlockLabel = Label::createWithTTF("Monster Block: 0", "fonts/Marker Felt.ttf", 60);
    _monsterBlockLabel->setTextColor(Color4B::BLUE); // ���ñ�ǩ��ɫΪ��ɫ
    _monsterBlockLabel->setPosition(Vec2(origin.x + 3 * visibleSize.width / 4, origin.y + visibleSize.height - _monsterHealthLabel->getContentSize().height - 70));
    this->addChild(_monsterBlockLabel, 1);
}

// ���� BUFF �� DEBUFF ��ǩ
void FightingScene::createBuffLabels() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // ����Ӣ�� BUFF ��ǩ
    _heroBuffLabel = Label::createWithTTF("Hero Buffs: None", "fonts/Marker Felt.ttf", 40);
    _heroBuffLabel->setTextColor(Color4B::YELLOW); // ���ñ�ǩ��ɫΪ��ɫ
    _heroBuffLabel->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + visibleSize.height - _heroHealthLabel->getContentSize().height - 140));
    this->addChild(_heroBuffLabel, 1);

    // �������� BUFF ��ǩ
    _monsterBuffLabel = Label::createWithTTF("Monster Buffs: None", "fonts/Marker Felt.ttf", 40);
    _monsterBuffLabel->setTextColor(Color4B::YELLOW); // ���ñ�ǩ��ɫΪ��ɫ
    _monsterBuffLabel->setPosition(Vec2(origin.x + 3 * visibleSize.width / 4, origin.y + visibleSize.height - _monsterHealthLabel->getContentSize().height - 140));
    this->addChild(_monsterBuffLabel, 1);
}

// ��������
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

// �����غ�����ǩ
void FightingScene::createTurnCountLabel()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // �����غ�����ǩ
    _turnCountLabel = Label::createWithTTF("Turn: 1", "fonts/Marker Felt.ttf", 60);
    _turnCountLabel->setTextColor(Color4B::GREEN); // ���ñ�ǩ��ɫΪ��ɫ
    _turnCountLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - _turnCountLabel->getContentSize().height));
    this->addChild(_turnCountLabel, 1);
}

// ������ɫ�͹���
void FightingScene::createCharacters()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // ����Ӣ��
    _hero = Hero::create("hero.png");
    if (_hero == nullptr)
    {
        problemLoading("'hero.png'");
        return;
    }

    _hero->setPosition(Vec2(origin.x + _hero->getContentSize().width / 2, origin.y + visibleSize.height / 2));
    this->addChild(_hero, 1);

    // ��������
    _monster = Monster::create("monster.png");
    if (_monster == nullptr)
    {
        problemLoading("'monster.png'");
        return;
    }

    _monster->setPosition(Vec2(origin.x + visibleSize.width - _monster->getContentSize().width / 2, origin.y + visibleSize.height / 2));
    this->addChild(_monster, 1);
}

// ��ʼ���ƶ�
void FightingScene::initializeDrawPile()
{
    // ��ȡ Hero �Ŀ��鲢����Ϊ���ƶ�
    _drawPile = _hero->getDeck();
    shuffleDrawPile();
}


// ����Ϊ�غ���ս���߼���ʵ��
// ����Ѫ���͸񵲱�ǩ
void FightingScene::updateHealthAndBlockLabels()
{
    _heroHealthLabel->setString("Hero Health: " + std::to_string(_hero->getHealth()));
    _monsterHealthLabel->setString("Monster Health: " + std::to_string(_monster->getHealth()));
    _heroBlockLabel->setString("Hero Block: " + std::to_string(_hero->getBlock()));
    _monsterBlockLabel->setString("Monster Block: " + std::to_string(_monster->getBlock()));
}

// ���� BUFF �� DEBUFF ��ǩ
void FightingScene::updateBuffLabels() {
    // ����Ӣ�� BUFF ��ǩ
    const auto& heroEffects = _hero->getEffects();
    if (heroEffects.empty()) {
        _heroBuffLabel->setString("Hero Buffs: None");
    }
    else {
        std::string heroBuffs = "Hero Buffs: ";
        for (const auto& effect : heroEffects) {
            heroBuffs += effect->getDescription() + " (" + std::to_string(effect->getRemainingTurns()) + " turns), ";
        }
        heroBuffs.pop_back(); // �Ƴ����һ������
        heroBuffs.pop_back();
        _heroBuffLabel->setString(heroBuffs);
    }

    // ���¹��� BUFF ��ǩ
    const auto& monsterEffects = _monster->getEffects();
    if (monsterEffects.empty()) {
        _monsterBuffLabel->setString("Monster Buffs: None");
    }
    else {
        std::string monsterBuffs = "Monster Buffs: ";
        for (const auto& effect : monsterEffects) {
            monsterBuffs += effect->getDescription() + " (" + std::to_string(effect->getRemainingTurns()) + " turns), ";
        }
        monsterBuffs.pop_back(); // �Ƴ����һ������
        monsterBuffs.pop_back();
        _monsterBuffLabel->setString(monsterBuffs);
    }
}

// ��ʼ��һغ�(���ơ���ӻغϽ�����ť�����ð�ťλ��)
void FightingScene::startPlayerTurn()
{
    _isPlayerTurn = true;

	// ���»غ�����ǩ
    _turnCountLabel->setString("Turn: " + std::to_string(_turnCount));

    for (int i = 0; i < 5; i++) {
        drawCard();
    }

    // ���һ����ť���ֶ������غ�
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto endTurnButton = ui::Button::create("button.png", "button_selected.png");

    if (endTurnButton == nullptr)
    {
        problemLoading("'buttonNormal.png' or 'buttonSelected.png'");
        return;
    }

    endTurnButton->setScale(0.25f); // ����ť��С��ԭ����50%

	// ���ð�ťλ��
    endTurnButton->setPosition(Vec2(origin.x + visibleSize.width - endTurnButton->getContentSize().width / 2,
        origin.y + visibleSize.width / 2 - endTurnButton->getContentSize().height / 2));

    endTurnButton->addClickEventListener([this](Ref* sender) {
        this->endTurn();
        });

    this->addChild(endTurnButton, 1);
}

// �������ƶѰ�ť
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

    // ���ð�ťλ���ڻ������½�
    showDiscardDeckButton->setPosition(Vec2(origin.x + visibleSize.width - showDiscardDeckButton->getContentSize().width / 2,
        origin.y + showDiscardDeckButton->getContentSize().height / 2));
    auto buttonMenu = Menu::create(showDiscardDeckButton, nullptr);
    buttonMenu->setPosition(Vec2::ZERO);
    this->addChild(buttonMenu, 4);
}


// ���ó��ƶѰ�ť
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

    // ���ð�ťλ���ڻ������½�
    showDrawDeckButton->setPosition(Vec2(origin.x + showDrawDeckButton->getContentSize().width / 2,
        origin.y + showDrawDeckButton->getContentSize().height / 2));
    auto buttonMenu = Menu::create(showDrawDeckButton, nullptr);
    buttonMenu->setPosition(Vec2::ZERO);
    this->addChild(buttonMenu, 4);
}


// ��ʼ����غ�
void FightingScene::startMonsterTurn()
{
    _isPlayerTurn = false;
	// Ŀǰ����غ϶��������10�˺������5����
    int damage = 10;
    int block = 5;

	_monster->setBlock(block);

    // �����
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

    // �������������˺�
    int newHealth = _hero->getHealth() - damage;
    _hero->setHealth(newHealth);
    CCLOG("Hero Health: %d", _hero->getHealth());
    updateHealthAndBlockLabels();
    endTurn();
}

// �����غ�
void FightingScene::endTurn()
{
    if (_isPlayerTurn)
    {
        // �����������������ƶ�
        _discardPile.insert(_discardPile.end(), _cards.begin(), _cards.end());
        _cards.clear();
        updateHandDisplay(); // ����������ʾ

		// ���ù����
        _monster->setBlock(0);
		updateHealthAndBlockLabels();
    }

    else
    {
        // ����غϽ���ʱ�������غϼ�����
        _turnCount++;

		// ����Ӣ�۸�
        _hero->setBlock(0);
        updateHealthAndBlockLabels();

        // ����Ч������ʱ��
        _hero->updateEffects(); // ����Ӣ�۵�Ч��
        _monster->updateEffects(); // ���¹����Ч��
		updateBuffLabels(); // ���� BUFF �� DEBUFF ��ǩ
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

// ���ս���Ƿ����
void FightingScene::checkBattleEnd()
{
    if (_hero->getHealth() <= 0)
    {
        CCLOG("Hero is dead. Game Over.");

        // ����ʧ����Ϣ��ǩ
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();

        auto defeatLabel = Label::createWithTTF("����������", "fonts/Marker Felt.ttf", 80);
        defeatLabel->setTextColor(Color4B::RED);
        defeatLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        this->addChild(defeatLabel, 10);

        // ����ӳٶ�����Ȼ���л���ʧ�ܳ���
        this->runAction(Sequence::create(
            DelayTime::create(0.1f),  // �ӳ�2��
            CallFunc::create([]() {
                // �л���ʧ�ܳ���
                auto failScene = FailScene::createScene();
                Director::getInstance()->replaceScene(TransitionFade::create(0.5f, failScene));
                }),
            nullptr
        ));
    }
    else if (_monster->getHealth() <= 0)
    {
        CCLOG("Monster is dead. You Win!");

        // ����ʤ����Ϣ��ǩ
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();

        auto victoryLabel = Label::createWithTTF("ս��ʤ����", "fonts/Marker Felt.ttf", 80);
        victoryLabel->setTextColor(Color4B::YELLOW);
        victoryLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        this->addChild(victoryLabel, 10);

        // ����ӳٶ�����Ȼ�󷵻ص�ͼ����
        this->runAction(Sequence::create(
            DelayTime::create(0.1f),  // �ӳ�2��
            CallFunc::create([]() {
                // ���ص���ͼ����
                auto mapScene = MyGame::Map::createScene();
                Director::getInstance()->replaceScene(TransitionFade::create(0.5f, mapScene));
                }),
            nullptr
        ));
    }
}

// ����Ϊ����ϵͳ��ʵ��
// ��һ����
void FightingScene::drawCard()
{
    if (_drawPile.empty())
    {
        // ������ƶ�Ϊ�գ������ƶѵ���ϴ����ƶ�
        _drawPile = _discardPile;
        _discardPile.clear();
        shuffleDrawPile();
    }

    if (!_drawPile.empty())
    {
        // �ӳ��ƶѶ���һ����
        Card drawnCard = _drawPile.back();
        _drawPile.pop_back();
        _cards.push_back(drawnCard);
        updateHandDisplay(); // ����������ʾ
    }
}

// ��һ����
void FightingScene::discardCard(int index)
{
    if (index >= 0 && index < _cards.size())
    {
        // �����Ʒ������ƶ�
        _discardPile.push_back(_cards[index]);
        _cards.erase(_cards.begin() + index);
        updateHandDisplay(); // ����������ʾ
    }
}

// ϴ��
void FightingScene::shuffleDrawPile()
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(_drawPile.begin(), _drawPile.end(), g);
}

// ���������Ч����ǩ
void FightingScene::addCardEffectLabel(cocos2d::Sprite* cardSprite, const std::string& effect)
{
    // ��ȡ���ƵĿ��
    float cardWidth = cardSprite->getContentSize().width;

    // �������б�ǩ�������������Ϊ���ƿ��
    auto effectLabel = Label::createWithTTF(effect, "fonts/Marker Felt.ttf", 48, Size(cardWidth, 0), TextHAlignment::CENTER);
    effectLabel->setColor(cocos2d::Color3B::BLACK);

    // ���ñ�ǩλ��Ϊ��������
    effectLabel->setPosition(Vec2(cardSprite->getContentSize().width / 2, cardSprite->getContentSize().height / 2));
    cardSprite->addChild(effectLabel, 1);
}

// ����������ʾ
void FightingScene::updateHandDisplay()
{
    // �����ǰ������ʾ
    for (auto cardSprite : _cardSprites)
    {
        this->removeChild(cardSprite);
    }
    _cardSprites.clear();
    _lastClickTimes.clear(); // ����ϴε��ʱ��

    // ��ȡ�ɼ������С��ԭ��
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // ����ÿ�ſ��Ƶı���ͼƬΪ "cardBackground.jpg"
    auto tempCardSprite = Sprite::create("cardBackground.jpg");
    float cardWidth = tempCardSprite->getContentSize().width;

    // ���㿨��֮��ļ��
    float totalWidth = _cards.size() * cardWidth;
    float spacing = 0.0f;
    if (totalWidth > visibleSize.width)
    {
        cardWidth = (visibleSize.width - 20.0f) / _cards.size(); // 20.0f �����ұ߾�
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
        _lastClickTimes.push_back(std::chrono::steady_clock::now()); // ��ʼ���ϴε��ʱ��

        addCardEffectLabel(cardSprite, _cards[i].getEffect());

        // ��ӵ���¼��������������Ӧ��
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

// Ӧ��BUFF��DEBUFF
void FightingScene::applyEffects(int& damage, int& block, const std::vector<std::shared_ptr<Effect>>& effects, const Card::Type cardType, bool isTargetMonster)
{
    for (const auto& effect : effects)
    {
        if (auto buff = dynamic_cast<Buff*>(effect.get()))
        {
            switch (buff->getType())
            {
            case Effect::Type::Strength:
                // ֻ�й������͵Ŀ��ƲŻ��ܵ�����Ч����Ӱ��
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
                    damage = static_cast<int>(damage * 1.5); // ���������˺�����
                }
                break;
            default:
                break;
            }
        }
    }
}

// Ӧ�ÿ���Ч�����������񵲡����BUFF��
void FightingScene::applyCardEffects(const Card& card)
{
    int damage = card.getAttack();
    int block = card.getBlock();

    // Ӧ��Ӣ�۵�Ч��
	// ���磬���������Ч�������ӹ����˺�
    applyEffects(damage, block, _hero->getEffects(), card.getType(), false);

    // Ӧ�ù����Ч��
	// ���磬���������Ч�������Ӷ�����ɵ��˺�
    applyEffects(damage, block, _monster->getEffects(), card.getType(), true);

    std::vector<std::shared_ptr<Effect>> effects = card.createEffects();

    for (const auto& effect : effects)
    {
        if (auto buff = dynamic_cast<Buff*>(effect.get()))
        {
            switch (buff->getType())
            {
            case Effect::Type::Strength:
                _hero->addEffect(effect); // ����Ч��Ӧ�õ�Ӣ��
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

    // �������ĸ�
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

    // ������������ֵ
    int newHealth = _monster->getHealth() - damage;
    _monster->setHealth(newHealth);
    CCLOG("Monster Health: %d", _monster->getHealth());

    // ����Ӣ�۵ĸ�
    if (block > 0)
    {
        int newBlock = _hero->getBlock() + block;
        _hero->setBlock(newBlock);
        CCLOG("Hero Block: %d", _hero->getBlock());
    }

    updateHealthAndBlockLabels();
}

// �������
void FightingScene::playCard(int index)
{
    if (_isCooldown) {
        // ���������ȴ״̬��ֱ�ӷ���
        return;
    }

    if (index >= 0 && index < _cards.size())
    {
        // ִ�д�����Ƶ��߼�
        Card playedCard = _cards[index];
        applyCardEffects(playedCard);

		checkBattleEnd();

        // ������ȴ״̬
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

// ����ѡ�еĿ���
void FightingScene::highlightSelectedCard()
{
    for (size_t i = 0; i < _cardSprites.size(); ++i)
    {
        if (i == _selectedCardIndex)
        {
            _cardSprites[i]->setColor(Color3B::YELLOW); // ����ѡ�еĿ���
        }
        else
        {
            _cardSprites[i]->setColor(Color3B::WHITE); // ȡ��������������
        }
    }
}

// �����Ƶ���¼�
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

// ������ƶѳ���
void FightingScene::goToDrawDeck(Ref* sender)
{
    auto drawDeckScene = DrawDeck::createScene(_drawPile);
    Director::getInstance()->pushScene(drawDeckScene);
}

// �������ƶѳ���
void FightingScene::goToDiscardDeck(Ref* sender)
{
    auto discardDeckScene = DiscardDeck::createScene(_discardPile);
    Director::getInstance()->pushScene(discardDeckScene);
}