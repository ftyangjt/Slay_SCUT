#include "Rest.h"
#include "Map.h"  // ��Ӷ� Map ����������

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

        // �����Ϣ��������ͼƬ
        auto background = Sprite::create("rest_background.png");
        if (background)
        {
            background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
            this->addChild(background, 0);  // ������ͼƬ��ӵ���ײ�
        }

        // �������ָ�����ť
        auto restoreItem = MenuItemImage::create(
            "recover.jpg",  // ����״̬��ͼƬ
            "restore_selected.png",  // ѡ��״̬��ͼƬ
            CC_CALLBACK_1(Rest::menuRestoreCallback, this)
        );
        if (restoreItem)
        {
            restoreItem->setPosition(Vec2(
                origin.x + visibleSize.width / 2,
                origin.y + visibleSize.height / 2 + 100
            ));
            restoreItem->setScale(0.5);  // ������ť��С
        }

        // ���������ء���ť
        auto returnItem = MenuItemImage::create(
            "up.jpg",  // ����״̬��ͼƬ
            "return_selected.png",  // ѡ��״̬��ͼƬ
            CC_CALLBACK_1(Rest::menuReturnCallback, this)
        );
        if (returnItem)
        {
            returnItem->setPosition(Vec2(
                origin.x + visibleSize.width / 2,
                origin.y + visibleSize.height / 2 - 100
            ));
            returnItem->setScale(0.5);  // ������ť��С
        }

        // �����˵�
        auto menu = Menu::create(restoreItem, returnItem, nullptr);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, 1);

        return true;
    }

    void Rest::menuRestoreCallback(Ref* pSender)
    {
        // ��ʾ�ָ���Ϣ
        auto label = Label::createWithTTF("+30", "fonts/Marker Felt.ttf", 24);
        label->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
            Director::getInstance()->getVisibleSize().height / 2));
        label->setScale(3.0);
        this->addChild(label, 1);

        // �ӳ�һ��ʱ��󷵻ص�ͼ����
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
        // ���ص�ͼ����
        auto scene = Map::createScene();
        Director::getInstance()->replaceScene(scene);
    }

} // namespace MyGame

