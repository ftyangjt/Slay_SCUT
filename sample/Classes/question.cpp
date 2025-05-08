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

    // �����¼�������ǩ
    auto eventLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", 36);
    eventLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(eventLabel, 2);

    // ����������͸��ʷ��䲻ͬ�¼�
    if (randomValue < 0.2) { // 20% ���ʽ���ս������
        eventLabel->setString("Fighting now!");

        // 3������ս������
        this->scheduleOnce([](float dt) {
            auto scene = FightingScene::createScene();
            Director::getInstance()->replaceScene(scene);
            }, 3.0f, "goto_battle");
    }
    else if (randomValue < 0.3) { // 10% ���ʽ����̵�
        eventLabel->setString("You find a shop");

        // 3�������̵곡��
        this->scheduleOnce([](float dt) {
            auto scene = MyGame::ShopScene::createScene();
            Director::getInstance()->replaceScene(scene);
            }, 3.0f, "goto_shop");
    }
    else if (randomValue < 0.6) { // 30% ���ʻ�Ѫ���30
        // ��ȡ��ǰ�����Ѫ��
        int currentHealth = Hero::getCurrentHealth();
        int maxHealth = Hero::getMaxHealth();

        // ������Իظ���Ѫ����������30�Ҳ��������Ѫ����
        int healAmount = std::min(30, maxHealth - currentHealth);

        // �ظ�Ѫ��
        Hero::healHealth(healAmount);

        eventLabel->setString("You recover " + std::to_string(healAmount) + " hp");
    }
    else if (randomValue < 0.8) { // 20% ���ʻ��50���
        // ���ӽ��
        Hero::addCoins(50);

        eventLabel->setString("You gain 50 coins");
    }
    else { // 10% ���ʴ��͵��������
        eventLabel->setString("You will be sent to a random room");

        // 3��󷵻ص�ͼ����ͼ���Զ����ѡ��һ�����䣩
        this->scheduleOnce([](float dt) {
            auto scene = MyGame::Map::createScene();
            Director::getInstance()->replaceScene(scene);
            }, 3.0f, "random_teleport");
    }

    // ����Ӣ��״̬��ʾ
    Hero::updateStatusDisplayStatic();

    // ����һ������Ч����ʹ��ǩ�ȷŴ��ָ�
    eventLabel->runAction(
        Sequence::create(
            ScaleTo::create(0.2f, 1.2f),
            ScaleTo::create(0.2f, 1.0f),
            nullptr
        )
    );
}
