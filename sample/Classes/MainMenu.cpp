#include "MainMenu.h"
#include "Hero.h"
#include "TransitionScene.h"
#include "GameSaveManager.h"
#include "Map.h"  // �����ͼ����ͷ�ļ�

USING_NS_CC;

Scene* MainMenu::createScene()
{
    return MainMenu::create();
}

bool MainMenu::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // ���ӱ���ͼƬ
    auto background = Sprite::create("background.webp");
    if (background)
    {
        background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        this->addChild(background, 0);  // ������ͼƬ���ӵ���ײ�
		setScale(1.6);  // ��������ͼƬ��С
    }

    // ��������ʼ��Ϸ���˵���
    auto startItem = MenuItemImage::create(
        "start_normal.png",  // ����״̬��ͼƬ
        "start_normal.png",  // ѡ��״̬��ͼƬ
        CC_CALLBACK_1(MainMenu::menuStartCallback, this)
    );
    if (startItem)
    {
        startItem->setPosition(Vec2(
            origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height / 2 - 145
        ));
        startItem->setScale(0.75);  // ������ť��С
    }

    // �������˳���Ϸ���˵���
    auto exitItem = MenuItemImage::create(
        "CloseNormal.png",  // ����״̬��ͼƬ
        "CloseSelected.png",  // ѡ��״̬��ͼƬ
        CC_CALLBACK_1(MainMenu::menuCloseCallback, this)
    );
    if (exitItem)
    {
        exitItem->setPosition(Vec2(
            origin.x + visibleSize.width -570,
            origin.y + visibleSize.height -1100
        ));
        exitItem->setScale(0.2);  // ������ť��С
    }

    // �������˵������˵�
    auto menu = Menu::create(startItem, exitItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);  // ȷ���˵��ڱ���ͼƬ֮��
    createContinueButton();

    return true;
}
<<<<<<< Updated upstream

void MainMenu::menuStartCallback(cocos2d::Ref* pSender)
{
    // ������Ϸ״̬
    MyGame::resetGameState();

    // ����Ӣ��Ѫ���ͽ��
    Hero::resetHealth();
    Hero::resetCoins();

    // �л�����ͼ����
    auto scene = MyGame::Map::createScene();
    Director::getInstance()->replaceScene(scene);
=======
void MainMenu::menuStartCallback(cocos2d::Ref* pSender)
{
    // ������Ϸ״̬
    MyGame::resetGameState();

    // ����Ӣ��Ѫ���ͽ��
    Hero::resetHealth();
    Hero::resetCoins();

    // �л�����������������������ֱ�ӽ����ͼ����
    auto scene = MyGame::TransitionScene::createScene();

    // ʹ�õ��뵭������Ч��������1��
    auto transition = TransitionFade::create(1.0f, scene, Color3B(0, 0, 0)); // ��ɫ���뵭��

    // �л���������Ч���Ĺ�����������
    Director::getInstance()->replaceScene(transition);
>>>>>>> Stashed changes
}


void MainMenu::menuCloseCallback(Ref* pSender)
{
    // �˳���Ϸ
    Director::getInstance()->end();
}

// �����˵���������
void MainMenu::createContinueButton()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // ����������Ϸ��ť
    auto continueButton = ui::Button::create("button.png", "button_selected.png");
    continueButton->setPosition(Vec2(origin.x + visibleSize.width / 2+60, origin.y + visibleSize.height / 2 +60));
	continueButton->setScale(0.5);

    // �����Ƿ��д浵������ť״̬
    bool hasSave = GameSaveManager::hasSavedGame();
    continueButton->setEnabled(hasSave);
    if (!hasSave)
    {
        continueButton->setColor(Color3B(150, 150, 150)); // ��ɫ��ʾ����
    }

    // ���ӵ���¼�
    continueButton->addClickEventListener([this](Ref* sender) {
        loadGame();
        });

    this->addChild(continueButton, 10);
}

void MainMenu::loadGame()
{
    // ��ȡ�浵
    int health, gold, currentMapId, currentLevel;
    std::vector<Card> deck;
    std::vector<std::vector<MyGame::RoomInfo>> mapInfo;
    std::vector<MyGame::ConnectionInfo> connectionInfo;
    int maxLayer;
    cocos2d::Vec2 currentRoomPos;
    MyGame::RoomType roomType;
    bool completed;

    if (GameSaveManager::loadGame(
        health,
        gold,
        deck,
        currentMapId,
        currentLevel,
        mapInfo,
        connectionInfo,
        maxLayer,
        currentRoomPos,
        roomType,
        completed
    ))
    {
        // �ָ���Ϸ״̬
        Hero::resetHealth();
        int maxHealth = Hero::getMaxHealth();
        Hero::healHealth(health - Hero::getCurrentHealth());
        Hero::setCoins(gold);

        // ��տ��鲢���ӿ���
        // ������� Hero ���Ѿ��о�̬������������
        Hero::clearDeckStatic();  // �������Ǿ�̬����
        for (const auto& card : deck) {
            Hero::addCardToDeckStatic(card);  // �������Ǿ�̬����
        }

        // �ָ���ͼ״̬
        MyGame::Map::currentLayer = currentMapId;
        MyGame::Map::currentRoom = currentLevel;

        // �ָ����о�̬��ͼ����
        MyGame::staticMapInfo = mapInfo;
        MyGame::staticConnectionInfo = connectionInfo;
        MyGame::maxAccessibleLayer = maxLayer;
        MyGame::currentRoomPosition = currentRoomPos;
        MyGame::currentRoomType = roomType;
        MyGame::roomCompleted = completed;

        // �л�����ͼ����
        auto mapScene = MyGame::Map::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(1.0f, mapScene));
    }
}

