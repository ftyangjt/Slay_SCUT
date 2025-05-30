#include "Rest.h"
#include "Map.h"  // 添加对 Map 场景的引用
#include "Hero.h" // 添加对 Hero 类的引用

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
            // 获取背景图像的原始大小
            Size originalSize = background->getContentSize();

            // 计算缩放比例，使背景图像完全覆盖屏幕
            float scaleX = visibleSize.width / originalSize.width;
            float scaleY = visibleSize.height / originalSize.height;

            // 选择较大的缩放比例以确保背景覆盖整个屏幕
            float scale = (scaleX > scaleY) ? scaleX : scaleY;

            // 应用缩放
            background->setScale(scale);

            // 设置背景图像位置在屏幕中心
            background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

            this->addChild(background, 0);  // 将背景图片添加到最底层
        }

        // 创建"恢复"按钮
        auto restoreItem = MenuItemImage::create(
            "recover.jpg",  // 正常状态的图片
            "restore_selected.png",  // 选中状态的图片
            CC_CALLBACK_1(Rest::menuRestoreCallback, this)
        );
        if (restoreItem)
        {
            restoreItem->setPosition(Vec2(
                origin.x + visibleSize.width / 2-400,
                origin.y + visibleSize.height / 2
            ));
            restoreItem->setScale(1.0);  // 调整按钮大小
        }

        // 创建"返回"按钮
        auto returnItem = MenuItemImage::create(
            "up.jpg",  // 正常状态的图片
            "return_selected.png",  // 选中状态的图片
            CC_CALLBACK_1(Rest::menuReturnCallback, this)
        );
        if (returnItem)
        {
            returnItem->setPosition(Vec2(
                origin.x + visibleSize.width / 2+400,
                origin.y + visibleSize.height / 2
            ));
            returnItem->setScale(1.0);  // 调整按钮大小
        }

        // 创建菜单
        auto menu = Menu::create(restoreItem, returnItem, nullptr);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, 1);

        return true;
    }

    void Rest::menuRestoreCallback(Ref* pSender)
    {
        // 恢复英雄30点血量
        int healAmount = 30;
        int oldHealth = Hero::getCurrentHealth();
        Hero::healHealth(healAmount);
        int actualHealed = Hero::getCurrentHealth() - oldHealth;

        // 显示恢复信息，显示实际恢复的血量
        std::string healText = "+" + std::to_string(actualHealed);
        auto label = Label::createWithTTF(healText, "fonts/Marker Felt.ttf", 60);
        // 设置文本颜色为绿色
        label->setTextColor(Color4B::GREEN);
        label->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
            Director::getInstance()->getVisibleSize().height / 2 + 300));
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
        // 增加10点最大血量上限
        Hero::increaseMaxHealth(10);

        // 显示增加的最大血量信息
        std::string maxHealthText = "+10 Max HP";
        auto label = Label::createWithTTF(maxHealthText, "fonts/Marker Felt.ttf", 60);
        // 设置文本颜色为金色
        label->setTextColor(Color4B(255, 215, 0, 255));  // 金色 (255, 215, 0)
        label->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
            Director::getInstance()->getVisibleSize().height / 2 + 300));
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

} // namespace MyGame