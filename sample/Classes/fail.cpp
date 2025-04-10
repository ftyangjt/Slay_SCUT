#include "fail.h"
#include "MainMenu.h"
#include "SimpleAudioEngine.h"
#include "Map.h" // 添加Map头文件以访问resetGameState函数

USING_NS_CC;

Scene* FailScene::createScene()
{
    return FailScene::create();
}

// 错误处理函数
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in FailScene.cpp\n");
}

bool FailScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    // 创建背景
    createBackground();

    // 创建UI元素
    createUI();

    return true;
}

void FailScene::createBackground()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建一个暗色背景
    auto background = LayerColor::create(Color4B(50, 50, 50, 255), visibleSize.width, visibleSize.height);
    this->addChild(background, 0);
}

void FailScene::createUI()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建失败标题
    auto failLabel = Label::createWithTTF("Failed!", "fonts/Marker Felt.ttf", 120);
    failLabel->setTextColor(Color4B::RED);
    failLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height * 0.7));
    this->addChild(failLabel, 1);

    // 创建提示信息
    auto messageLabel = Label::createWithTTF("The hero died...", "fonts/Marker Felt.ttf", 60);
    messageLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height * 0.5));
    this->addChild(messageLabel, 1);

    // 创建返回主菜单按钮
    auto returnButton = MenuItemImage::create(
        "replay.png",
        "button_selected.png",
        [](Ref* sender) {
            // 重置游戏状态，确保重新开始游戏时创建新地图
            MyGame::resetGameState();

            // 返回主菜单
            auto mainMenu = MainMenu::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, mainMenu));
        });

    if (returnButton == nullptr ||
        returnButton->getContentSize().width <= 0 ||
        returnButton->getContentSize().height <= 0)
    {
        problemLoading("'button.png' and 'button_selected.png'");
    }
    else
    {
        returnButton->setScale(1.0f);

        // 添加按钮文本
        /*
        auto buttonLabel = Label::createWithTTF("Back to the menu", "fonts/Marker Felt.ttf", 36);
        buttonLabel->setPosition(Vec2(returnButton->getContentSize().width / 2,
            returnButton->getContentSize().height / 2));
        returnButton->addChild(buttonLabel);
        */
        // 创建菜单
        auto menu = Menu::create(returnButton, nullptr);
        menu->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height * 0.3));
        this->addChild(menu, 1);
    }
}
