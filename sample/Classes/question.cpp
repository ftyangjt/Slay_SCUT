#include "question.h"
#include "Map.h"  // 引入地图场景头文件

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

    // 创建“返回”按钮
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

    return true;
}

void Question::menuReturnCallback(Ref* pSender)
{
    // 切换到地图场景
    auto scene = MyGame::Map::createScene();
    Director::getInstance()->replaceScene(scene);
}

