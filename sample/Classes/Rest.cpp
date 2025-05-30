#include "Rest.h"
#include "Map.h"  // ��Ӷ� Map ����������
#include "Hero.h" // ��Ӷ� Hero �������

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
            // ��ȡ����ͼ���ԭʼ��С
            Size originalSize = background->getContentSize();

            // �������ű�����ʹ����ͼ����ȫ������Ļ
            float scaleX = visibleSize.width / originalSize.width;
            float scaleY = visibleSize.height / originalSize.height;

            // ѡ��ϴ�����ű�����ȷ����������������Ļ
            float scale = (scaleX > scaleY) ? scaleX : scaleY;

            // Ӧ������
            background->setScale(scale);

            // ���ñ���ͼ��λ������Ļ����
            background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

            this->addChild(background, 0);  // ������ͼƬ��ӵ���ײ�
        }

        // ����"�ָ�"��ť
        auto restoreItem = MenuItemImage::create(
            "recover.jpg",  // ����״̬��ͼƬ
            "restore_selected.png",  // ѡ��״̬��ͼƬ
            CC_CALLBACK_1(Rest::menuRestoreCallback, this)
        );
        if (restoreItem)
        {
            restoreItem->setPosition(Vec2(
                origin.x + visibleSize.width / 2-400,
                origin.y + visibleSize.height / 2
            ));
            restoreItem->setScale(1.0);  // ������ť��С
        }

        // ����"����"��ť
        auto returnItem = MenuItemImage::create(
            "up.jpg",  // ����״̬��ͼƬ
            "return_selected.png",  // ѡ��״̬��ͼƬ
            CC_CALLBACK_1(Rest::menuReturnCallback, this)
        );
        if (returnItem)
        {
            returnItem->setPosition(Vec2(
                origin.x + visibleSize.width / 2+400,
                origin.y + visibleSize.height / 2
            ));
            returnItem->setScale(1.0);  // ������ť��С
        }

        // �����˵�
        auto menu = Menu::create(restoreItem, returnItem, nullptr);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, 1);

        return true;
    }

    void Rest::menuRestoreCallback(Ref* pSender)
    {
        // �ָ�Ӣ��30��Ѫ��
        int healAmount = 30;
        int oldHealth = Hero::getCurrentHealth();
        Hero::healHealth(healAmount);
        int actualHealed = Hero::getCurrentHealth() - oldHealth;

        // ��ʾ�ָ���Ϣ����ʾʵ�ʻָ���Ѫ��
        std::string healText = "+" + std::to_string(actualHealed);
        auto label = Label::createWithTTF(healText, "fonts/Marker Felt.ttf", 60);
        // �����ı���ɫΪ��ɫ
        label->setTextColor(Color4B::GREEN);
        label->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
            Director::getInstance()->getVisibleSize().height / 2 + 300));
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
        // ����10�����Ѫ������
        Hero::increaseMaxHealth(10);

        // ��ʾ���ӵ����Ѫ����Ϣ
        std::string maxHealthText = "+10 Max HP";
        auto label = Label::createWithTTF(maxHealthText, "fonts/Marker Felt.ttf", 60);
        // �����ı���ɫΪ��ɫ
        label->setTextColor(Color4B(255, 215, 0, 255));  // ��ɫ (255, 215, 0)
        label->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
            Director::getInstance()->getVisibleSize().height / 2 + 300));
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

} // namespace MyGame