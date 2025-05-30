#include "MainMenu.h"
<<<<<<< Updated upstream
=======
#include "Hero.h"
#include "TransitionScene.h"
>>>>>>> Stashed changes
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
    auto background = Sprite::create("background.webp");
    if (background)
    {
        background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        this->addChild(background, 0);  // 将背景图片添加到最底层
		setScale(1.6);  // 调整背景图片大小
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
            origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height / 2 - 145
        ));
        startItem->setScale(0.75);  // 调整按钮大小
    }

    // 创建“退出游戏”菜单项
    auto exitItem = MenuItemImage::create(
        "CloseNormal.png",  // 正常状态的图片
        "CloseSelected.png",  // 选中状态的图片
        CC_CALLBACK_1(MainMenu::menuCloseCallback, this)
    );
    if (exitItem)
    {
        exitItem->setPosition(Vec2(
            origin.x + visibleSize.width -570,
            origin.y + visibleSize.height -1100
        ));
        exitItem->setScale(0.2);  // 调整按钮大小
    }

    // 将两个菜单项加入菜单
    auto menu = Menu::create(startItem, exitItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);  // 确保菜单在背景图片之上

    return true;
}
<<<<<<< Updated upstream

void MainMenu::menuStartCallback(Ref* pSender)
{
    // 切换到地图场景
    auto scene = MyGame::Map::createScene();
    Director::getInstance()->replaceScene(scene);
=======
void MainMenu::menuStartCallback(cocos2d::Ref* pSender)
{
    // 重置游戏状态
    MyGame::resetGameState();

    // 重置英雄血量和金币
    Hero::resetHealth();
    Hero::resetCoins();

    // 切换到过场动画场景，而不是直接进入地图场景
    auto scene = MyGame::TransitionScene::createScene();

    // 使用淡入淡出过渡效果，持续1秒
    auto transition = TransitionFade::create(1.0f, scene, Color3B(0, 0, 0)); // 黑色淡入淡出

    // 切换到带过渡效果的过场动画场景
    Director::getInstance()->replaceScene(transition);
>>>>>>> Stashed changes
}

void MainMenu::menuCloseCallback(Ref* pSender)
{
    // 退出游戏
    Director::getInstance()->end();
}
