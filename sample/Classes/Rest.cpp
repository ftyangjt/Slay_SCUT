#include "Rest.h"
#include "Map.h"  // 添加对 Map 场景的引用

USING_NS_CC;

namespace MyGame {

    Scene* Rest::createScene()
    {
        return Rest::create();
    }

    bool Rest::init()
    {
        if (!Scene::init())
        {
            return false;
        }

        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto origin = Director::getInstance()->getVisibleOrigin();

        // 添加休息场景背景图片
        auto background = Sprite::create("rest_background.png");
        if (background)
        {
            background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
            this->addChild(background, 0);  // 将背景图片添加到最底层
        }

        // 创建“恢复”按钮
        auto restoreItem = MenuItemImage::create(
            "recover.jpg",  // 正常状态的图片
            "restore_selected.png",  // 选中状态的图片
            CC_CALLBACK_1(Rest::menuRestoreCallback, this)
        );
        if (restoreItem)
        {
            restoreItem->setPosition(Vec2(
                origin.x + visibleSize.width / 2,
                origin.y + visibleSize.height / 2 + 100
            ));
            restoreItem->setScale(0.5);  // 调整按钮大小
        }

        // 创建“返回”按钮
        auto returnItem = MenuItemImage::create(
            "up.jpg",  // 正常状态的图片
            "return_selected.png",  // 选中状态的图片
            CC_CALLBACK_1(Rest::menuReturnCallback, this)
        );
        if (returnItem)
        {
            returnItem->setPosition(Vec2(
                origin.x + visibleSize.width / 2,
                origin.y + visibleSize.height / 2 - 100
            ));
            returnItem->setScale(0.5);  // 调整按钮大小
        }

        // 创建菜单
        auto menu = Menu::create(restoreItem, returnItem, nullptr);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, 1);

        return true;
    }

    void Rest::menuRestoreCallback(Ref* pSender)
    {
        // 显示恢复信息
        auto label = Label::createWithTTF("+30", "fonts/Marker Felt.ttf", 24);
        label->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
            Director::getInstance()->getVisibleSize().height / 2));
        label->setScale(3.0);
        this->addChild(label, 1);

        // 延迟一段时间后返回地图场景
        this->runAction(Sequence::create(
            DelayTime::create(2.0f),
            CallFunc::create([]() {
                auto scene = Map::createScene();
                Director::getInstance()->replaceScene(scene);
                }),
            nullptr
        ));
    }

    void Rest::menuReturnCallback(Ref* pSender)
    {
        // 返回地图场景
        auto scene = Map::createScene();
        Director::getInstance()->replaceScene(scene);
    }

} // namespace MyGame

