// Hero.cpp
#include "Hero.h"

USING_NS_CC;

// ��Ӿ�̬�������洢Ӣ�۵Ľ���ֵ�ͽ��
static int heroHealth = 100; // Ĭ������ֵΪ100
static int heroCoins = 100; // Ĭ�Ͻ��Ϊ100

// ��ʼ����̬��ǩ
cocos2d::Label* Hero::s_statusLabel = nullptr;

// ��������ʵ��
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

// ��ȡ��ǰ����ֵ
int Hero::getCurrentHealth()
{
    return heroHealth;
}

// �ָ�ָ������������ֵ������������
void Hero::healHealth(int amount)
{
    heroHealth = std::min(heroHealth + amount, MAX_HEALTH);
    updateStatusDisplayStatic(); // ������ʾ
}

// ��ȡ��ǰ�������
int Hero::getCoins()
{
    return heroCoins;
}

// ���ý������
void Hero::setCoins(int coins)
{
    heroCoins = coins;
    updateStatusDisplayStatic(); // ������ʾ
}

// ���ӽ��
void Hero::addCoins(int amount)
{
    heroCoins += amount;
    updateStatusDisplayStatic(); // ������ʾ
}

// ���ý��
void Hero::resetCoins()
{
    heroCoins = INITIAL_COINS;
    updateStatusDisplayStatic(); // ������ʾ
}

// ��ʼ��״̬��ʾ

void Hero::initStatusDisplay()
{
    // ���ͷſ����Ѿ����ڵı�ǩ
    if (s_statusLabel != nullptr) {
        if (s_statusLabel->getParent()) {
            s_statusLabel->removeFromParent();
        }
        s_statusLabel->release(); // �ͷ�֮ǰ��retain
        s_statusLabel = nullptr;
    }

    // ����״̬��ʾ��ǩ
    s_statusLabel = Label::createWithTTF("Coins: " + std::to_string(heroCoins) +
        " Health: " + std::to_string(heroHealth),
        "fonts/Marker Felt.ttf", 24);
    if (s_statusLabel == nullptr) {
        CCLOG("Error: Failed to create status label");
        return;
    }

    s_statusLabel->setAnchorPoint(Vec2(0, 1)); // ���ϽǶ���

    // ��ȡ��ǰ���г����Ϳ��������С
    Scene* runningScene = Director::getInstance()->getRunningScene();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // ���ñ�ǩλ�������Ͻ�
    s_statusLabel->setPosition(Vec2(origin.x + 10, origin.y + visibleSize.height - 10));
    s_statusLabel->setTextColor(Color4B::WHITE); // ���ñ�ǩ��ɫΪ��ɫ

    // Ϊ��ǩ��Ӻ�ɫ��ߣ���߿ɶ���
    s_statusLabel->enableOutline(Color4B::BLACK, 1);

    // ����ǩ��ӵ���ǰ���г���
    if (runningScene) {
        runningScene->addChild(s_statusLabel, 100); // ʹ�ü��ߵ�Z˳��ȷ����ʾ����ǰ
    }

    // ��ӳ����л�������
    Director::getInstance()->getEventDispatcher()->addCustomEventListener(
        Director::EVENT_AFTER_SET_NEXT_SCENE,
        [](EventCustom* event) {
            Hero::updateStatusDisplayStatic(); // �����л������״̬��ʾ
        }
    );

    s_statusLabel->retain(); // ������ǩ�Է������л�ʱ���Զ��ͷ�
}


void Hero::updateStatusDisplayStatic()
{
    // �����ǩ�����ڣ��򴴽�
    if (s_statusLabel == nullptr) {
        initStatusDisplay();
        return;
    }

    // ���±�ǩ�ı�
    s_statusLabel->setString("Coins: " + std::to_string(heroCoins) +
        " Health: " + std::to_string(heroHealth));

    // ����ǩ�Ƿ��ڵ�ǰ������
    Scene* currentScene = Director::getInstance()->getRunningScene();
    if (currentScene && !s_statusLabel->getParent()) {
        s_statusLabel->removeFromParent(); // ȷ����ǩ��֮ǰ�ĸ��ڵ����Ƴ�
        currentScene->addChild(s_statusLabel, 100); // ��ӵ���ǰ����
    }
}



// ��ʼ������
bool Hero::init(const std::string& filename)
{
    if (!Sprite::initWithFile(filename))
    {
        return false;
    }

    // ʹ�ñ��������ֵ��ʼ������
    _health = heroHealth;

    // ����Ĭ�ϵĿ���
    createDefaultDeck();

    // ��ʼ��������ʵ��״̬��ǩ
    initStatusDisplay();
    _statusLabel = s_statusLabel;

    return true;
}

// �������ǵ�����ֵ
void Hero::setHealth(int health)
{
    _health = health;
    // ͬʱ���¾�̬����
    heroHealth = health;
    updateStatusDisplayStatic(); // ʹ�þ�̬��������
}

// ��ȡ���ǵ�����ֵ
int Hero::getHealth() const
{
    return _health;
}

// ���ý���ֵ����������Ϸ���¿�ʼʱ���ã�
void Hero::resetHealth()
{
    heroHealth = MAX_HEALTH;  // ���þ�̬����
    updateStatusDisplayStatic(); // ʹ�þ�̬��������
}

// �������ǵķ���ֵ
void Hero::setBlock(int block) {
    _block = block;
}

// ��ȡ���ǵķ���ֵ
int Hero::getBlock() const {
    return _block;
}

// ��ӿ��Ƶ�����
void Hero::addCardToDeck(const Card& card)
{
    _deck.push_back(card);
}

// ��ȡ����
const std::vector<Card>& Hero::getDeck() const
{
    return _deck;
}

// ��տ���
void Hero::clearDeck()
{
    _deck.clear();
}

// ��ʼ��Ĭ�Ͽ���
void Hero::createDefaultDeck()
{
    // ����ʾ������һ����ʼ���飬���忨�����Ը���ʵ���������
    clearDeck();

    // ���ʾ�����ƣ����� Card ���캯����Card(name, type, cost, effect, background, attack, defense)��
    addCardToDeck(Card("Strike", Card::Type::Attack, 1, "Deal 6 damage", "cardBackground.jpg", 6, 0));
    addCardToDeck(Card("Strike", Card::Type::Attack, 1, "Deal 6 damage", "cardBackground.jpg", 6, 0));
    addCardToDeck(Card("Strike", Card::Type::Attack, 1, "Deal 6 damage", "cardBackground.jpg", 6, 0));
    addCardToDeck(Card("Defend", Card::Type::Skill, 1, "Gain 5 Block", "cardBackground.jpg", 0, 5));
    addCardToDeck(Card("Defend", Card::Type::Skill, 1, "Gain 5 Block", "cardBackground.jpg", 0, 5));
    addCardToDeck(Card("Defend", Card::Type::Skill, 1, "Gain 5 Block", "cardBackground.jpg", 0, 5));

    // ���� Bash ���Ʋ��������Ч��
    Card bashCard("Bash", Card::Type::Attack, 2, "Deal 8 damage and apply Vulnerable", "cardBackground.jpg", 8, 0);
    // ʹ���·������Ч����ֱ��ָ��Ч�����͡��ȼ��ͳ���ʱ��
    bashCard.addEffect(Effect::Type::Vulnerable, 1, 3); // 1�����ˣ�����3�غ�
    addCardToDeck(bashCard);

    Card strengthCard("Strength", Card::Type::Power, 1, "Gain 2 Strength", "cardBackground.jpg");
    strengthCard.addEffect(Effect::Type::Strength, 2, -1); // 2������������ʱ��Ϊ����
    addCardToDeck(strengthCard);
}

//  ���Ч��
void Hero::addEffect(std::shared_ptr<Effect> effect) {
    if (!effect) {
        CCLOG("Error: Attempted to add a null effect.");
        return; // �������� effect �ǿ�ָ�룬ֱ�ӷ���
    }

    for (auto& existingEffect : _effects) {
        // ����Ƿ������ͬ���͵�Ч��
        if (existingEffect->getType() == effect->getType()) {
            if (effect->getType() == Effect::Type::Strength) {
                // ����Ч�������ӵȼ�
                existingEffect->setLevel(existingEffect->getLevel() + effect->getLevel());
            }
            else if (effect->getType() == Effect::Type::Vulnerable) {
                // ����Ч����ʱ������
                existingEffect->addRemainingTurns(effect->getRemainingTurns());
            }
            return; // ������ɺ�ֱ�ӷ���
        }
    }

    // ���û���ҵ���ͬ���͵�Ч����������µ�Ч��
    _effects.push_back(effect);
}

// ��ȡӵ�е�Ч��
const std::vector<std::shared_ptr<Effect>>& Hero::getEffects() const
{
    return _effects;
}

// ����Ч�����Ƴ�����ʱ��Ϊ 0 ��Ч��
void Hero::updateEffects() {
    for (auto it = _effects.begin(); it != _effects.end(); ) {
        (*it)->reduceTurn();
        if ((*it)->getRemainingTurns() == 0) {
            it = _effects.erase(it); // �Ƴ�����ʱ��Ϊ 0 ��Ч��
        }
        else {
            ++it;
        }
    }
    updateStatusDisplay(); // ����ʵ�������ĵ��ã������д���
}

// ����״̬��ʾ (ʵ������)
void Hero::updateStatusDisplay() {
    updateStatusDisplayStatic(); // �򵥵ص��þ�̬����
}
