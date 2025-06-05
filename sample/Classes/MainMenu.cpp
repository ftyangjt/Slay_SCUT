#include "MainMenu.h"
#include "Hero.h"
#include "GameSaveManager.h"
#include "TransitionScene.h"
#include "Map.h"  // 引入地图场景头文件

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

    // 添加背景图片
    auto background = Sprite::create("mainmenu.png");
    if (background)
    {
        background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        background->setScale(1.6);  // 只缩放背景图片
        this->addChild(background, 0);
    }

    // 创建“开始游戏”菜单项
    auto startItem = MenuItemImage::create(
        "start_normal.png",  // 正常状态的图片
        "start_normal.png",  // 选中状态的图片
        CC_CALLBACK_1(MainMenu::menuStartCallback, this)
    );
    if (startItem)
    {
        startItem->setPosition(Vec2(
            origin.x + visibleSize.width / 2-620,
            origin.y + visibleSize.height / 2 - 340
        ));
        startItem->setScale(1.50);  // 调整按钮大小
        // 添加缩放动画效果
        auto scaleUp = ScaleTo::create(0.6f, 1.60);
        auto scaleDown = ScaleTo::create(0.6f, 1.50);
        auto sequence = Sequence::create(scaleUp, scaleDown, nullptr);
        auto repeatForever = RepeatForever::create(sequence);
        startItem->runAction(repeatForever);
    }

    // 创建“退出游戏”菜单项
    auto exitItem = MenuItemImage::create(
        "exit.png",  // 正常状态的图片
        "CloseSelected.png",  // 选中状态的图片
        CC_CALLBACK_1(MainMenu::menuCloseCallback, this)
    );
    if (exitItem)
    {
        exitItem->setPosition(Vec2(
            origin.x + visibleSize.width/2 + 620,
            origin.y + visibleSize.height / 2 - 340
        ));
        exitItem->setScale(1.52);  // 调整按钮大小
    }

    // 将两个菜单项加入菜单
    auto menu = Menu::create(startItem, exitItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);  // 确保菜单在背景图片之上
    createContinueButton();

    return true;
}

void MainMenu::menuStartCallback(cocos2d::Ref* pSender)
{
    // 重置游戏状态
    MyGame::resetGameState();

    // 重置英雄血量和金币
    Hero::resetHealth();
    Hero::resetCoins();

    // 切换到过场动画场景
    auto scene = MyGame::TransitionScene::createScene();
    Director::getInstance()->replaceScene(scene);
}


void MainMenu::menuCloseCallback(Ref* pSender)
{
    // 退出游戏
    Director::getInstance()->end();
}

// 在主菜单类中添加
void MainMenu::createContinueButton()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建继续游戏按钮
    auto continueButton = ui::Button::create("load.png", "load.png");
    continueButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 -335));
	continueButton->setScale(1.48);

    // 根据是否有存档决定按钮状态
    bool hasSave = GameSaveManager::hasSavedGame();
    continueButton->setEnabled(hasSave);
    if (!hasSave)
    {
        continueButton->setColor(Color3B(150, 150, 150)); // 灰色表示禁用
    }

    // 添加点击事件
    continueButton->addClickEventListener([this](Ref* sender) {
        loadGame();
        });

    this->addChild(continueButton, 10);
}

void MainMenu::loadGame()
{
    // 读取存档
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
        // 恢复游戏状态
        Hero::resetHealth();
        int maxHealth = Hero::getMaxHealth();
        Hero::healHealth(health - Hero::getCurrentHealth());
        Hero::setCoins(gold);

        // 清空卡组并添加卡牌
        // 如果假设 Hero 类已经有静态方法操作卡组
        Hero::clearDeckStatic();  // 假设这是静态方法
        for (const auto& card : deck) {
            Hero::addCardToDeckStatic(card);  // 假设这是静态方法
        }

        // 恢复地图状态
        MyGame::Map::currentLayer = currentMapId;
        MyGame::Map::currentRoom = currentLevel;

        // 恢复所有静态地图数据
        MyGame::staticMapInfo = mapInfo;
        MyGame::staticConnectionInfo = connectionInfo;
        MyGame::maxAccessibleLayer = maxLayer;
        MyGame::currentRoomPosition = currentRoomPos;
        MyGame::currentRoomType = roomType;
        MyGame::roomCompleted = completed;

        // 切换到地图场景
        auto mapScene = MyGame::Map::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(1.0f, mapScene));
    }
}

