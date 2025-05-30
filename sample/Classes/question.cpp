#include "question.h"
#include "Map.h"  // �����ͼ����ͷ�ļ�

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

    // ����ʺų����ı���ͼƬ
    auto background = Sprite::create("question_background.png");
    if (background)
    {
        background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        this->addChild(background, 0);  // ������ͼƬ��ӵ���ײ�
    }

    // ���������ء���ť
    auto returnItem = MenuItemImage::create(
        "back.jpg",  // ����״̬��ͼƬ
        "return_selected.png",  // ѡ��״̬��ͼƬ
        CC_CALLBACK_1(Question::menuReturnCallback, this)
    );
    if (returnItem)
    {
        returnItem->setPosition(Vec2(
            origin.x + visibleSize.width - returnItem->getContentSize().width / 2 - 10,
            origin.y + returnItem->getContentSize().height / 2 + 10
        ));
        returnItem->setScale(0.5);  // ������ť��С
    }

    // ����ť����˵�
    auto menu = Menu::create(returnItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);  // ȷ���˵��ڱ���ͼƬ֮��

    return true;
}

void Question::menuReturnCallback(Ref* pSender)
{
    // �л�����ͼ����
    auto scene = MyGame::Map::createScene();
    Director::getInstance()->replaceScene(scene);
}

