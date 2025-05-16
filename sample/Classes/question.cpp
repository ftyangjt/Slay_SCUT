#include "question.h"
#include "Map.h"  // �����ͼ����ͷ�ļ�
#include "FightingScene.h" // ����ս������ͷ�ļ�
#include "Shop.h" // �����̵곡��ͷ�ļ�
#include "Hero.h" // ����Ӣ����ͷ�ļ�
#include <random>

USING_NS_CC;

Scene* Question::createScene()
{
    return Question::create();
}

bool Question::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // ��ʼ����Ա����
    _isWheelSpinning = false;

    // ����ʺų����ı���ͼƬ
    auto background = Sprite::create("question_background.png");
    if (background)
    {
        background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        this->addChild(background, 0);  // ������ͼƬ��ӵ���ײ�
    }

    // ����"����"��ť
    auto returnItem = MenuItemImage::create(
        "back.jpg",  // ����״̬��ͼƬ
        "return_selected.png",  // ѡ��״̬��ͼƬ
        CC_CALLBACK_1(Question::menuReturnCallback, this)
    );
    if (returnItem)
    {
        returnItem->setPosition(Vec2(
            origin.x + visibleSize.width - returnItem->getContentSize().width / 2 - 10,
            origin.y + returnItem->getContentSize().height / 2 + 10
        ));
        returnItem->setScale(0.5);  // ������ť��С
    }

    // ����ť����˵�
    auto menu = Menu::create(returnItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);  // ȷ���˵��ڱ���ͼƬ֮��

    // �����¼�������ǩ
    _eventLabel = Label::createWithTTF("Welcome to Mystery Room!", "fonts/Marker Felt.ttf", 36);
    _eventLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 50));
    this->addChild(_eventLabel, 2);

    // ���볡��ʱ������������¼�
    this->scheduleOnce([this](float dt) {
        triggerRandomEvent();
        }, 0.1f, "trigger_event");

    return true;
}

void Question::menuReturnCallback(Ref* pSender)
{
    // �л�����ͼ����
    auto scene = MyGame::Map::createScene();
    Director::getInstance()->replaceScene(scene);
}

void Question::triggerRandomEvent()
{
    // ���������������
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    // ����0��1֮��������
    float randomValue = dis(gen);

    auto visibleSize = Director::getInstance()->getVisibleSize();

    // ����������͸��ʷ��䲻ͬ�¼�
    if (randomValue < 0.2) { // 20% ���ʽ���ս������
        _eventLabel->setString("Fighting now!");

        // 3������ս������
        this->scheduleOnce([](float dt) {
            auto scene = FightingScene::createScene();
            Director::getInstance()->replaceScene(scene);
            }, 1.0f, "goto_battle");
    }
    else if (randomValue < 0.3) { // 10% ���ʽ����̵�
        _eventLabel->setString("You find a shop");

        // 3�������̵곡��
        this->scheduleOnce([](float dt) {
            auto scene = MyGame::ShopScene::createScene();
            Director::getInstance()->replaceScene(scene);
            }, 1.0f, "goto_shop");
    }
    else if (randomValue < 0.4) { // 10% ���ʻ�Ѫ���30
        // ��ȡ��ǰ�����Ѫ��
        int currentHealth = Hero::getCurrentHealth();
        int maxHealth = Hero::getMaxHealth();

        // ������Իظ���Ѫ����������30�Ҳ��������Ѫ����
        int healAmount = std::min(30, maxHealth - currentHealth);

        // �ظ�Ѫ��
        Hero::healHealth(healAmount);

        _eventLabel->setString("You recover " + std::to_string(healAmount) + " hp");
    }
    else if (randomValue < 0.6) { // 20% ���ʻ��50���
        // ���ӽ��
        Hero::addCoins(50);

        _eventLabel->setString("You gain 50 coins");
    }
    else { // 40% ���ʳ��ֳ齱ת��
        _eventLabel->setString("Try your luck with the wheel of fortune!");

        // �����齱ת��
        createLuckyWheel();
    }

    // ����Ӣ��״̬��ʾ
    Hero::updateStatusDisplayStatic();

    // ����һ������Ч����ʹ��ǩ�ȷŴ��ָ�
    _eventLabel->runAction(
        Sequence::create(
            ScaleTo::create(0.2f, 1.2f),
            ScaleTo::create(0.2f, 1.0f),
            nullptr
        )
    );
}

void Question::createLuckyWheel()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // ����ת�̱��� (���û��wheel.png������ʹ������Բ��ͼ������Լ�����һ��DrawNode��Ϊת��)
    _wheelSprite = Sprite::create("wheel.png");

    // ���û��ת��ͼƬ������һ����ʱ��Բ��ת��
    if (!_wheelSprite)
    {
        // ����һ��Բ�ε�DrawNode��Ϊת��
        auto drawNode = DrawNode::create();

        // ת�̰뾶
        float radius = 150.0f;

        // ��һ������Բ��Ϊ����
        drawNode->drawSolidCircle(Vec2::ZERO, radius, 0, 60, Color4F(0.8f, 0.8f, 0.8f, 1.0f));

        // ������ɫ��������-20 HP��
        auto redDrawNode = DrawNode::create();
        for (int angle = 0; angle < 120; angle++) {
            float radians1 = CC_DEGREES_TO_RADIANS(angle);
            float radians2 = CC_DEGREES_TO_RADIANS(angle + 1);
            Vec2 v1(std::cos(radians1) * radius, std::sin(radians1) * radius);
            Vec2 v2(std::cos(radians2) * radius, std::sin(radians2) * radius);
            redDrawNode->drawTriangle(Vec2::ZERO, v1, v2, Color4F(1.0f, 0.0f, 0.0f, 1.0f));
        }
        redDrawNode->setPosition(Vec2::ZERO);
        drawNode->addChild(redDrawNode);

        // ������ɫ��������+20 HP��
        auto greenDrawNode = DrawNode::create();
        for (int angle = 120; angle < 240; angle++) {
            float radians1 = CC_DEGREES_TO_RADIANS(angle);
            float radians2 = CC_DEGREES_TO_RADIANS(angle + 1);
            Vec2 v1(std::cos(radians1) * radius, std::sin(radians1) * radius);
            Vec2 v2(std::cos(radians2) * radius, std::sin(radians2) * radius);
            greenDrawNode->drawTriangle(Vec2::ZERO, v1, v2, Color4F(0.0f, 1.0f, 0.0f, 1.0f));
        }
        greenDrawNode->setPosition(Vec2::ZERO);
        drawNode->addChild(greenDrawNode);

        // ������ɫ��������+40 Coins��
        auto yellowDrawNode = DrawNode::create();
        for (int angle = 240; angle < 360; angle++) {
            float radians1 = CC_DEGREES_TO_RADIANS(angle);
            float radians2 = CC_DEGREES_TO_RADIANS(angle + 1);
            Vec2 v1(std::cos(radians1) * radius, std::sin(radians1) * radius);
            Vec2 v2(std::cos(radians2) * radius, std::sin(radians2) * radius);
            yellowDrawNode->drawTriangle(Vec2::ZERO, v1, v2, Color4F(1.0f, 1.0f, 0.0f, 1.0f));
        }
        yellowDrawNode->setPosition(Vec2::ZERO);
        drawNode->addChild(yellowDrawNode);

        // ����һ����ʱ�������DrawNode
        _wheelSprite = Sprite::create();
        auto rt = RenderTexture::create(radius * 2, radius * 2);
        rt->beginWithClear(0, 0, 0, 0);

        // ��drawNode��ӵ���Ⱦ������
        drawNode->setPosition(Vec2(radius, radius));

        // ��ȡ��Ⱦ���͵�ǰ�ı任����
        auto renderer = Director::getInstance()->getRenderer();
        auto& transform = Director::getInstance()->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

        // ��ȷ����visit������������Ⱦ���ͱ任����
        drawNode->visit(renderer, transform, 0);

        rt->end();

        // ���þ��������
        _wheelSprite->setTexture(rt->getSprite()->getTexture());
        _wheelSprite->setTextureRect(Rect(0, 0, radius * 2, radius * 2));
    }

    // ����ת��λ�� - �����Ҳ�
    float wheelX = visibleSize.width / 2 + 150;
    _wheelSprite->setPosition(Vec2(wheelX, visibleSize.height / 2));
    this->addChild(_wheelSprite, 1);

    // �Ƴ�ת���ϵ����ֱ�ǩ����Ϊ�ڳ�����ֱ����Ӿ�̬����˵��

    // �������Ľ���˵���ı���
    auto descriptionBox = LayerColor::create(Color4B(50, 50, 50, 180), 250, 300);
    descriptionBox->setPosition(Vec2(wheelX - 450, visibleSize.height / 2 - 150));
    this->addChild(descriptionBox, 1);

    // ��ӱ���
    auto titleLabel = Label::createWithTTF("Exlaination", "fonts/Marker Felt.ttf", 32);
    titleLabel->setPosition(Vec2(125, 270));
    titleLabel->setColor(Color3B::WHITE);
    descriptionBox->addChild(titleLabel);

    // ��ӽ�������
    auto redDescription = Label::createWithTTF("Red: +20 HP", "fonts/Marker Felt.ttf", 24);
    redDescription->setPosition(Vec2(125, 200));
    redDescription->setColor(Color3B::RED);
    descriptionBox->addChild(redDescription);

    auto greenDescription = Label::createWithTTF("Green: -20 HP", "fonts/Marker Felt.ttf", 24);
    greenDescription->setPosition(Vec2(125, 150));
    greenDescription->setColor(Color3B::GREEN);
    descriptionBox->addChild(greenDescription);

    auto yellowDescription = Label::createWithTTF("Yellow: +40 coins", "fonts/Marker Felt.ttf", 24);
    yellowDescription->setPosition(Vec2(125, 100));
    yellowDescription->setColor(Color3B::YELLOW);
    descriptionBox->addChild(yellowDescription);

    // ���ʹ��˵��
    auto instructionLabel = Label::createWithTTF("click the button to start", "fonts/Marker Felt.ttf", 22);
    instructionLabel->setPosition(Vec2(125, 40));
    instructionLabel->setColor(Color3B::WHITE);
    descriptionBox->addChild(instructionLabel);

    // ����ָ��
    _pointerSprite = Sprite::create("pointer.png");
    if (!_pointerSprite)
    {
        // ���û��ָ��ͼƬ������һ����ʱ��������ָ��
        auto drawNode = DrawNode::create();
        Vec2 points[3] = { Vec2(0, 30), Vec2(-15, 0), Vec2(15, 0) };
        drawNode->drawSolidPoly(points, 3, Color4F(1.0f, 0.5f, 0.0f, 1.0f));

        // ����һ����ʱ�������DrawNode
        _pointerSprite = Sprite::create();
        auto rt = RenderTexture::create(30, 30);
        rt->beginWithClear(0, 0, 0, 0);

        // ��ȡ��Ⱦ���͵�ǰ�ı任����
        auto renderer = Director::getInstance()->getRenderer();
        auto& transform = Director::getInstance()->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

        drawNode->setPosition(Vec2(15, 15));
        // ��ȷ����visit������������Ⱦ���ͱ任����
        drawNode->visit(renderer, transform, 0);

        rt->end();
        _pointerSprite->setTexture(rt->getSprite()->getTexture());
        _pointerSprite->setTextureRect(Rect(0, 0, 30, 30));
    }

    // ����ָ��λ��
    _pointerSprite->setPosition(Vec2(wheelX, visibleSize.height / 2 + 170));
    this->addChild(_pointerSprite, 2);

    // �����齱��ť
    _spinButton = MenuItemImage::create(
        "button.png",   // ����״̬��ͼƬ
        "button_selected.png",  // ѡ��״̬��ͼƬ
        CC_CALLBACK_1(Question::onSpinButtonClicked, this)
    );

    if (!_spinButton)
    {
        // ���û�а�ťͼƬ������һ����ʱ��ť
        _spinButton = MenuItemImage::create();
        auto drawNode = DrawNode::create();
        drawNode->drawSolidRect(Vec2(-60, -20), Vec2(60, 20), Color4F(0.0f, 0.5f, 1.0f, 1.0f));

        auto rt = RenderTexture::create(120, 40);
        rt->beginWithClear(0, 0, 0, 0);

        // ��ȡ��Ⱦ���͵�ǰ�ı任����
        auto renderer = Director::getInstance()->getRenderer();
        auto& transform = Director::getInstance()->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

        drawNode->setPosition(Vec2(60, 20));
        // ��ȷ����visit������������Ⱦ���ͱ任����
        drawNode->visit(renderer, transform, 0);

        rt->end();

        _spinButton->setNormalImage(rt->getSprite());
        _spinButton->setCallback(CC_CALLBACK_1(Question::onSpinButtonClicked, this));
    }

    // �ڰ�ť���������
    auto spinButtonLabel = Label::createWithTTF("start to lottery", "fonts/Marker Felt.ttf", 60);
    spinButtonLabel->setPosition(Vec2(_spinButton->getContentSize().width / 2, _spinButton->getContentSize().height / 2));
    _spinButton->addChild(spinButtonLabel);

    // ���ð�ťλ�� - ����ת���·�
    _spinButton->setPosition(Vec2(wheelX, visibleSize.height / 2 - 300));

    // �����˵�����Ӱ�ť
    auto menu = Menu::create(_spinButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 2);

    // ���ð�ť��С
    _spinButton->setScale(0.3); // �Ŵ�ť
}


void Question::onSpinButtonClicked(Ref* pSender)
{
    if (_isWheelSpinning)
        return;

    _isWheelSpinning = true;

    // ���ó齱��ť
    _spinButton->setEnabled(false);

    // ����������
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> wheelDis(0, 2);
    int wheelResult = wheelDis(gen);

    // ִ��ת����ת����
    spinWheel(wheelResult);
}

void Question::spinWheel(int result)
{
    // ����������ת�Ƕ�
    // ���ݽ���������սǶȣ�0=ʧȥ����(-20HP), 1=�������(+20HP), 2=��ý��(+40��)
    float finalAngle = 0;
    switch (result) {
    case 0: finalAngle = 60.0f; break;  // ָ���ɫ����(ʧȥ����)
    case 1: finalAngle = 180.0f; break; // ָ����ɫ����(�������)
    case 2: finalAngle = 300.0f; break; // ָ���ɫ����(��ý��)
    }

    // ���һЩ�����תȦ����ʹ��ת����Ȼ
    int numRotations = 5 + (std::rand() % 3); // 5-7Ȧ
    float totalAngle = 360.0f * numRotations + finalAngle;

    // ִ����ת����
    auto rotateAction = RotateBy::create(3.0f, totalAngle);
    auto easeAction = EaseOut::create(rotateAction, 2.0f); // ʹ�ü���Ч��

    // ������������ʾ���
    auto showResultCallback = CallFunc::create([this, result]() {
        _isWheelSpinning = false;
        showWheelResult(result);
        });

    // ִ�ж�������
    _wheelSprite->runAction(Sequence::create(easeAction, showResultCallback, nullptr));
}

void Question::showWheelResult(int result)
{
    switch (result) {
    case 0: { // ʧȥ20����
        int currentHealth = Hero::getCurrentHealth();
        // ȷ������ֵ�������1
        int loseAmount = std::min(currentHealth - 1, 20);
        Hero::healHealth(-loseAmount); // ʹ�ø�����ʾ�۳�����ֵ
        _eventLabel->setString("Bad luck! You lose " + std::to_string(loseAmount) + " health");
        _eventLabel->setTextColor(Color4B::RED);
        break;
    }
    case 1: { // ���20����
        int currentHealth = Hero::getCurrentHealth();
        int maxHealth = Hero::getMaxHealth();
        int healAmount = std::min(20, maxHealth - currentHealth);
        Hero::healHealth(healAmount);
        _eventLabel->setString("Good luck! You gain " + std::to_string(healAmount) + " health");
        _eventLabel->setTextColor(Color4B::GREEN);
        break;
    }
    case 2: { // ���40���
        Hero::addCoins(40);
        _eventLabel->setString("Jackpot! You gain 40 coins");
        _eventLabel->setTextColor(Color4B::YELLOW);
        break;
    }
    }

    // ����һ������Ч����ʹ��ǩ�ȷŴ��ָ�
    _eventLabel->runAction(
        Sequence::create(
            ScaleTo::create(0.2f, 1.2f),
            ScaleTo::create(0.2f, 1.0f),
            nullptr
        )
    );

    // 3��󷵻ص�ͼ
    this->scheduleOnce([](float dt) {
        auto scene = MyGame::Map::createScene();
        Director::getInstance()->replaceScene(scene);
        }, 3.0f, "goto_map");
}
