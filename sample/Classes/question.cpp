#include "question.h"
#include "Map.h"  // 引入地图场景头文件
#include "FightingScene.h" // 引入战斗场景头文件
#include "Shop.h" // 引入商店场景头文件
#include "Hero.h" // 引入英雄类头文件
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

    // 添加问号场景的背景图片
    auto background = Sprite::create("question_background.png");
    if (background)
    {
        background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        this->addChild(background, 0);  // 将背景图片添加到最底层
    }

    // 创建"返回"按钮
    auto returnItem = MenuItemImage::create(
        "back.jpg",  // 正常状态的图片
        "return_selected.png",  // 选中状态的图片
        CC_CALLBACK_1(Question::menuReturnCallback, this)
    );
    if (returnItem)
    {
        returnItem->setPosition(Vec2(
            origin.x + visibleSize.width - returnItem->getContentSize().width / 2 - 10,
            origin.y + returnItem->getContentSize().height / 2 + 10
        ));
        returnItem->setScale(0.5);  // 调整按钮大小
    }

    // 将按钮加入菜单
    auto menu = Menu::create(returnItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);  // 确保菜单在背景图片之上

    // 进入场景时立即触发随机事件
    this->scheduleOnce([this](float dt) {
        triggerRandomEvent();
        }, 0.1f, "trigger_event");

    return true;
}

void Question::menuReturnCallback(Ref* pSender)
{
    // 切换到地图场景
    auto scene = MyGame::Map::createScene();
    Director::getInstance()->replaceScene(scene);
}

void Question::triggerRandomEvent()
{
    // 创建随机数生成器
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    // 生成0到1之间的随机数
    float randomValue = dis(gen);

    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 创建事件描述标签
    auto eventLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", 36);
    eventLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(eventLabel, 2);

    // 根据随机数和概率分配不同事件
    if (randomValue < 0.2) { // 20% 概率进入战斗场景
        eventLabel->setString("Fighting now!");

        // 3秒后进入战斗场景
        this->scheduleOnce([](float dt) {
            auto scene = FightingScene::createScene();
            Director::getInstance()->replaceScene(scene);
            }, 3.0f, "goto_battle");
    }
    else if (randomValue < 0.3) { // 10% 概率进入商店
        eventLabel->setString("You find a shop");

        // 3秒后进入商店场景
        this->scheduleOnce([](float dt) {
            auto scene = MyGame::ShopScene::createScene();
            Director::getInstance()->replaceScene(scene);
            }, 3.0f, "goto_shop");
    }
    else if (randomValue < 0.6) { // 30% 概率回血最多30
        // 获取当前和最大血量
        int currentHealth = Hero::getCurrentHealth();
        int maxHealth = Hero::getMaxHealth();

        // 计算可以回复的血量（不超过30且不超过最大血量）
        int healAmount = std::min(30, maxHealth - currentHealth);

        // 回复血量
        Hero::healHealth(healAmount);

        eventLabel->setString("You recover " + std::to_string(healAmount) + " hp");
    }
    else if (randomValue < 0.8) { // 20% 概率获得50金币
        // 增加金币
        Hero::addCoins(50);

        eventLabel->setString("You gain 50 coins");
    }
    else { // 10% 概率传送到随机房间
        eventLabel->setString("You will be sent to a random room");

        // 3秒后返回地图（地图会自动随机选择一个房间）
        this->scheduleOnce([](float dt) {
            auto scene = MyGame::Map::createScene();
            Director::getInstance()->replaceScene(scene);
            }, 3.0f, "random_teleport");
    }

    // 更新英雄状态显示
    Hero::updateStatusDisplayStatic();

    // 创建一个动画效果，使标签先放大后恢复
    eventLabel->runAction(
        Sequence::create(
            ScaleTo::create(0.2f, 1.2f),
            ScaleTo::create(0.2f, 1.0f),
            nullptr
        )
    );
}
