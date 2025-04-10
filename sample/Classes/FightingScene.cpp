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

// ��ʼ��һغ�(���ơ���ӻغϽ�����ť�����ð�ťλ��)
void FightingScene::startPlayerTurn()
{
    _isPlayerTurn = true;

	// ���»غ�����ǩ
    _turnCountLabel->setString("Turn: " + std::to_string(_turnCount));

    updateHandDisplay();

    drawSequentialCards(5);

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

    // ���水ťָ�뵽��Ա������
    _discardDeckButton = showDiscardDeckButton;

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
	// ���水ťָ�뵽��Ա������
	_drawDeckButton = showDrawDeckButton;
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
        Vec2 discardDeckPosition = _discardDeckButton->getPosition();
		Vec2 drawDeckPosition = _drawDeckButton->getPosition();
        playDiscardToDrawMeteorEffect(discardDeckPosition,drawDeckPosition);
        
    }

    else
    {
        // ����غϽ���ʱ�������غϼ�����
        _turnCount++;

		// ���ø�
        _hero->setBlock(0);
        _monster->setBlock(0);
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
void FightingScene::drawCard()
{
    if (_drawPile.empty())
    {
        // �����ƶ�Ϊ�գ������ƶ�ϴ�س��ƶ�
        _drawPile = _discardPile;
        _discardPile.clear();
        shuffleDrawPile();
    }

    if (!_drawPile.empty())
    {
        // �ӳ��ƶ�ȡ��
        Card drawnCard = _drawPile.back();
        _drawPile.pop_back();

        // ������ʱ���飬�ӳ��ƶѰ�ťλ�ó���
        auto tempSprite = Sprite::create("cardBackground.jpg");
        tempSprite->setPosition(_drawDeckButton->getPosition());
        tempSprite->setScale(0.0f);
        tempSprite->setLocalZOrder(9999); // ��֤��ʾ����ǰ
        this->addChild(tempSprite);

        // ����������Ʒ���λ�ã���ʾ���������������ģ�
        auto visibleSize = Director::getInstance()->getVisibleSize();
        float targetX = visibleSize.width * 0.5f;
        float targetY = tempSprite->getContentSize().height / 3;

        // ���в��Ŵ�
        auto moveAction = MoveTo::create(0.3f, Vec2(targetX, targetY));
        auto scaleAction = ScaleTo::create(0.3f, 1.0f);
        auto spawn = Spawn::create(moveAction, scaleAction, nullptr);

        // ���������󣬽����Ƽ��� _cards ��ˢ������
        auto finish = CallFunc::create([this, drawnCard, tempSprite]() {
            _cards.push_back(drawnCard);
            tempSprite->removeFromParent();
            updateHandDisplay();
            });

        tempSprite->runAction(Sequence::create(spawn, finish, nullptr));
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
// ˢ��������ʾ
void FightingScene::updateHandDisplay()
{
    size_t newCount = _cards.size();

    // ������е����п��ƾ���
    for (auto sprite : _cardSprites)
    {
        sprite->removeFromParent();
    }
    _cardSprites.clear();
    _lastClickTimes.clear();

    if (newCount == 0)
        return;  // û�п���ʱֱ�ӷ���

    // ������ʱ�����Ի�ȡʵ�ʳߴ�
    auto tempSprite = Sprite::create("cardBackground.jpg");
    float originalCardWidth = tempSprite->getContentSize().width;
    float originalCardHeight = tempSprite->getContentSize().height;

    // ���������ÿ�ȣ����Ǳ�Ե��ࣩ
    float availableWidth = _visibleSize.width * 0.9f;  // ������Ļ��Ե10%�Ŀռ�

    // ����ÿ�ſ�����С��ˮƽ��ࣨ�ɸ�����Ҫ������
    float minCardSpacing = 10.0f;

    // ���������ܵĿ��ƿ�ȣ�ʹ�����п����ܹ���ʾ
    float maxCardWidth;

    if (newCount == 1) {
        // ֻ��һ�ſ���ʱ����ȿ����Դ�
        maxCardWidth = originalCardWidth * 0.8f;
    }
    else {
        // ���������ܵĿ��ƿ�ȣ�ȷ�����п��ƶ�����ʾ
        float totalSpacing = (newCount - 1) * minCardSpacing;
        maxCardWidth = (availableWidth - totalSpacing) / newCount;
    }

    // ��������ȼ�����������
    float scaleFactor = std::min(1.0f, maxCardWidth / originalCardWidth);

    // ����ʵ�ʿ��ƿ�Ⱥ͸߶�
    float actualCardWidth = originalCardWidth * scaleFactor;
    float actualCardHeight = originalCardHeight * scaleFactor;

    // ���㿨�����е���ʼX���꣨������ʾ��
    float totalWidth = newCount * actualCardWidth + (newCount - 1) * minCardSpacing;
    float startX = (_visibleSize.width - totalWidth) / 2 + actualCardWidth / 2;

    // �޸�����������ƴ�ֱλ�ã�ʹ����ӽ���Ļ�ײ�
    // �����Ʒ��ڸ��͵�λ�ã�ֻ¶�����ָ߶�
    float cardY = _origin.y + actualCardHeight * 0.4f; // ��Ϊ0.4�����Ƹ߶�

    // ���������п���
    for (size_t i = 0; i < newCount; ++i)
    {
        auto sprite = Sprite::create("cardBackground.jpg");
        float posX = startX + i * (actualCardWidth + minCardSpacing);

        // ����λ�úͳ�ʼ����
        sprite->setPosition(Vec2(posX, cardY));
        sprite->setScale(scaleFactor);

        this->addChild(sprite, 1);
        _cardSprites.push_back(sprite);
        _lastClickTimes.push_back(std::chrono::steady_clock::now());

        // ��ӿ���Ч����ǩ
        addCardEffectLabel(sprite, _cards[i].getEffect());

        // ע�ᴥ���¼�
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

    // ��鲢����ѡ�еĿ���
    if (_selectedCardIndex >= 0 && _selectedCardIndex < _cardSprites.size())
    {
        highlightSelectedCard();
    }
}

// Ӧ��BUFF��DEBUFF
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

// Ӧ�ÿ���Ч��
void FightingScene::applyCardEffects(const Card& card)
{
    int damage = card.getAttack();
    int block = card.getBlock();

    // Ӧ��Ӣ�۵�Ч��
    applyEffects(damage, block, _hero->getEffects(), false);

    // Ӧ�ù����Ч��
    applyEffects(damage, block, _monster->getEffects(), true);

    for (const auto& effect : card.getEffects())
    {
        if (auto buff = dynamic_cast<Buff*>(effect.get()))
        {
            switch (buff->getType())
            {
            case Effect::Type::Strength:
                _hero->addEffect(effect);
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

    // �������ĸ�
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
        return; // ���������ȴ״̬��ֱ�ӷ���
    }

    if (index >= 0 && index < _cards.size()) {
        // ԭ�߼�����
        Card playedCard = _cards[index];
        applyCardEffects(playedCard);

        // ��ȡ��Ӧ���ƾ���
        auto cardSprite = _cardSprites[index];

        // �øÿ���������ʾ
        cardSprite->setLocalZOrder(9999);

        // Ŀ��λ�ã����ƶѰ�ť
        Vec2 discardPos = _discardDeckButton->getPosition();
        auto moveAction = MoveTo::create(0.3f, discardPos);
        auto scaleAction = ScaleTo::create(0.3f, 0.1f);
        auto moveAndScale = Spawn::create(moveAction, scaleAction, nullptr);

        // �������������Ƴ�������
        auto finish = CallFunc::create([this, index, cardSprite]() {
            cardSprite->removeFromParent();
            discardCard(index);
            _selectedCardIndex = -1;
            highlightSelectedCard();
            });

        cardSprite->runAction(Sequence::create(moveAndScale, finish, nullptr));

        // ������ȴ
        _isCooldown = true;
        this->runAction(Sequence::create(
            DelayTime::create(COOLDOWN_TIME),
            CallFunc::create([this]() { _isCooldown = false; }),
            nullptr
        ));
    }
}


// ����ѡ�еĿ���
void FightingScene::highlightSelectedCard()
{
    for (size_t i = 0; i < _cardSprites.size(); ++i)
    {
        // ȷ��������Ч����Ȼ�ڳ�����
        if (_cardSprites[i] && _cardSprites[i]->getParent())
        {
            if (i == _selectedCardIndex)
            {
                // ����ѡ�еĿ���
                _cardSprites[i]->setColor(cocos2d::Color3B(255, 255, 0)); // ��ɫ
            }
            else
            {
                // �ָ�������ɫ
                _cardSprites[i]->setColor(cocos2d::Color3B(255, 255, 255)); // ��ɫ
            }
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

void FightingScene::goToDrawDeck(Ref* sender)
{
    auto drawDeckScene = DrawDeck::createScene(_drawPile);
    Director::getInstance()->pushScene(drawDeckScene);
}

void FightingScene::goToDiscardDeck(Ref* sender)
{
    auto discardDeckScene = DiscardDeck::createScene(_discardPile);
    Director::getInstance()->pushScene(discardDeckScene);
}

// ���� discardPilePosition��drawPilePosition Ϊ���ƶ�����ƶѵ���Ļ����

void FightingScene::playDiscardToDrawMeteorEffect(const Vec2& discardPilePosition, const Vec2& drawPilePosition)
{
    // ������������
    auto meteor = ParticleMeteor::create();
    meteor->setPosition(discardPilePosition);
    meteor->setDuration(0.8f); // ��ʱ�����ɸ�����Ҫ����

    // ����������ɫ��������ɫ��
    Color4F currentColor = meteor->getStartColor();
    Color4F deepColor(currentColor.r * 0.5f, currentColor.g * 0.5f, currentColor.b * 0.5f, currentColor.a);
    meteor->setStartColor(deepColor);
    // ������Ҫ��Ҳ���Ե����յ���ɫ
    Color4F endColor = meteor->getEndColor();
    Color4F deepEndColor(endColor.r * 0.5f, endColor.g * 0.5f, endColor.b * 0.5f, endColor.a);
    meteor->setEndColor(deepEndColor);

    // �����ƶ�����
    auto moveAction = MoveTo::create(0.8f, drawPilePosition);
    auto sequence = Sequence::create(
        moveAction,
        CallFunc::create([meteor]() {
            meteor->removeFromParent(); // �Ƴ�����
            }),
        nullptr);

    // ���ж�������ӵ�����
    meteor->runAction(sequence);
    meteor->setAutoRemoveOnFinish(false); // ���Զ��Ƴ����ֶ��Ƴ�
    this->addChild(meteor, 10);
}

// �ݹ����γ�����Ƶĺ���
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
        // �ӳ��ƶ�ȡ��
        Card drawnCard = _drawPile.back();
        _drawPile.pop_back();

        // ������ʱ���飬�ӳ��ƶѰ�ťλ�ó���
        auto tempSprite = Sprite::create("cardBackground.jpg");
        tempSprite->setPosition(_drawDeckButton->getPosition());
        tempSprite->setScale(0.0f);
        tempSprite->setLocalZOrder(9999);
        this->addChild(tempSprite);

        // ���в��Ŵ�
        auto moveAction = MoveTo::create(0.3f, Vec2(_visibleSize.width * 0.5f, tempSprite->getContentSize().height / 3));
        auto scaleAction = ScaleTo::create(0.3f, 1.0f);
        auto spawn = Spawn::create(moveAction, scaleAction, nullptr);

        // ���������󣺼��뵽���ơ��Ƴ���ʱ���鲢�ݹ������һ��
        auto finish = CallFunc::create([this, drawnCard, tempSprite, count]() {
            _cards.push_back(drawnCard);
            tempSprite->removeFromParent();
            updateHandDisplay();
            drawSequentialCards(count - 1);
            });

        tempSprite->runAction(Sequence::create(spawn, finish, nullptr));
    }
}