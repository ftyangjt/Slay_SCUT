#include "MainMenu.h"
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

    // ��ӱ���ͼƬ
    auto background = Sprite::create("background.webp");
    if (background)
    {
        background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        this->addChild(background, 0);  // ������ͼƬ��ӵ���ײ�
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
            origin.x + visibleSize.width - 50,
            origin.y + visibleSize.height / 2 - 300
        ));
        exitItem->setScale(3);  // ������ť��С
    }

    // �������˵������˵�
    auto menu = Menu::create(startItem, exitItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);  // ȷ���˵��ڱ���ͼƬ֮��

    return true;
}

void MainMenu::menuStartCallback(Ref* pSender)
{
    // �л�����ͼ����
    auto scene = MyGame::Map::createScene();
    Director::getInstance()->replaceScene(scene);
}

void MainMenu::menuCloseCallback(Ref* pSender)
{
    // �˳���Ϸ
    Director::getInstance()->end();
}
