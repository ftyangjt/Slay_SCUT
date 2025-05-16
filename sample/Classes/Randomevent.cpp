#include "RandomEvent.h"
#include "ui/CocosGUI.h"
#include <random>

USING_NS_CC;

// ��̬ʵ����ʼ��
RandomEventManager* RandomEventManager::_instance = nullptr;

RandomEventManager* RandomEventManager::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new RandomEventManager();
    }
    return _instance;
}

void RandomEventManager::initEvents()
{
    // ע�������¼�����
    _events.push_back(std::make_shared<AltarEvent>());
    _events.push_back(std::make_shared<TreasureEvent>());
    _events.push_back(std::make_shared<HealingFountainEvent>());
    _events.push_back(std::make_shared<MerchantEvent>());
}

std::shared_ptr<RandomEvent> RandomEventManager::getRandomEvent()
{
    if (_events.empty())
    {
        return nullptr;
    }

    // ���ѡ��һ���¼�
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, _events.size() - 1);
    int index = dis(gen);

    return _events[index];
}

// ��̳�¼�ʵ��
void AltarEvent::execute(Hero* hero)
{
    // �۳�һЩѪ����������������
    int healthLoss = hero->getMaxHealth() * 0.1; // ����10%�����Ѫ��
    hero->setHealth(hero->getHealth() - healthLoss);

    // �����������Ӣ�۵��������߸���ĳ��Buff
    // ������һ�����������ķ���
    // hero->increasePower(2);
}

void AltarEvent::showEventUI(cocos2d::Scene* scene, Hero* hero)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // �����¼��Ի���
    auto eventBox = ui::Layout::create();
    eventBox->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    eventBox->setBackGroundColor(Color3B(50, 50, 50));
    eventBox->setContentSize(Size(visibleSize.width * 0.8f, visibleSize.height * 0.6f));
    eventBox->setPosition(Vec2(visibleSize.width * 0.1f, visibleSize.height * 0.2f));
    scene->addChild(eventBox, 10);

    // ����¼�����
    auto titleLabel = Label::createWithTTF(getName(), "fonts/Marker Felt.ttf", 36);
    titleLabel->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height - 40));
    eventBox->addChild(titleLabel);

    // ����¼�����
    auto descLabel = Label::createWithTTF(getDescription(), "fonts/Marker Felt.ttf", 24);
    descLabel->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height - 100));
    descLabel->setMaxLineWidth(eventBox->getContentSize().width * 0.8f);
    eventBox->addChild(descLabel);

    // ����׼�ѡ��
    auto sacrificeButton = ui::Button::create("button.png", "button_Selected.png");
    sacrificeButton->setTitleText("�׼� (��ʧ10%Ѫ�����������)");
    sacrificeButton->setTitleFontSize(20);
    sacrificeButton->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height / 2 - 50));
    sacrificeButton->addTouchEventListener([this, scene, hero, eventBox](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            execute(hero);
            eventBox->removeFromParent();

            // ���ص�ͼ����
            auto mapScene = MyGame::Map::createScene();
            Director::getInstance()->replaceScene(mapScene);
        }
        });
    eventBox->addChild(sacrificeButton);

    // ����뿪ѡ��
    auto leaveButton = ui::Button::create("button.png", "button_Selected.png");
    leaveButton->setTitleText("�뿪");
    leaveButton->setTitleFontSize(20);
    leaveButton->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height / 2 - 120));
    leaveButton->addTouchEventListener([scene, eventBox](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            eventBox->removeFromParent();

            // ���ص�ͼ����
            auto mapScene = MyGame::Map::createScene();
            Director::getInstance()->replaceScene(mapScene);
        }
        });
    eventBox->addChild(leaveButton);
}

// �����¼�ʵ��
void TreasureEvent::execute(Hero* hero)
{
    // ��������Ǻ��»��ǻ���
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10);
    int result = dis(gen);

    if (result <= 7) { // 70%���ʻ�úô�
        // ������Ը���Ӣ��һЩ����������������������
        hero->setMaxHealth(hero->getMaxHealth() + 5);
        hero->setHealth(hero->getHealth() + 5);
    }
    else { // 30%�����ܵ��˺�
        // �ܵ�һЩ�˺�
        int damage = hero->getMaxHealth() * 0.15; // �ܵ�15%���������ֵ�˺�
        hero->setHealth(hero->getHealth() - damage);
    }
}

void TreasureEvent::showEventUI(cocos2d::Scene* scene, Hero* hero)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // �����¼��Ի���
    auto eventBox = ui::Layout::create();
    eventBox->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    eventBox->setBackGroundColor(Color3B(50, 50, 50));
    eventBox->setContentSize(Size(visibleSize.width * 0.8f, visibleSize.height * 0.6f));
    eventBox->setPosition(Vec2(visibleSize.width * 0.1f, visibleSize.height * 0.2f));
    scene->addChild(eventBox, 10);

    // ����¼�����
    auto titleLabel = Label::createWithTTF(getName(), "fonts/Marker Felt.ttf", 36);
    titleLabel->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height - 40));
    eventBox->addChild(titleLabel);

    // ����¼�����
    auto descLabel = Label::createWithTTF(getDescription(), "fonts/Marker Felt.ttf", 24);
    descLabel->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height - 100));
    descLabel->setMaxLineWidth(eventBox->getContentSize().width * 0.8f);
    eventBox->addChild(descLabel);

    // ��Ӵ򿪱���ѡ��
    auto openButton = ui::Button::create("button.png", "button_Selected.png");
    openButton->setTitleText("�򿪱��� (70%��������5���������ޣ�30%������ʧ15%Ѫ��)");
    openButton->setTitleFontSize(15);
    openButton->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height / 2 - 50));
    openButton->addTouchEventListener([this, scene, hero, eventBox](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            execute(hero);
            eventBox->removeFromParent();

            // ���ص�ͼ����
            auto mapScene = MyGame::Map::createScene();
            Director::getInstance()->replaceScene(mapScene);
        }
        });
    eventBox->addChild(openButton);

    // ����뿪ѡ��
    auto leaveButton = ui::Button::create("button.png", "button_Selected.png");
    leaveButton->setTitleText("�뿪");
    leaveButton->setTitleFontSize(20);
    leaveButton->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height / 2 - 120));
    leaveButton->addTouchEventListener([scene, eventBox](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            eventBox->removeFromParent();

            // ���ص�ͼ����
            auto mapScene = MyGame::Map::createScene();
            Director::getInstance()->replaceScene(mapScene);
        }
        });
    eventBox->addChild(leaveButton);
}

// ����Ȫˮ�¼�ʵ��
void HealingFountainEvent::execute(Hero* hero)
{
    // �ָ���������ֵ
    int healing = hero->getMaxHealth() * 0.3; // �ָ�30%���������ֵ
    int newHealth = hero->getHealth() + healing;

    // ȷ���������������ֵ
    if (newHealth > hero->getMaxHealth()) {
        newHealth = hero->getMaxHealth();
    }

    hero->setHealth(newHealth);
}

void HealingFountainEvent::showEventUI(cocos2d::Scene* scene, Hero* hero)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // �����¼��Ի���
    auto eventBox = ui::Layout::create();
    eventBox->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    eventBox->setBackGroundColor(Color3B(50, 50, 50));
    eventBox->setContentSize(Size(visibleSize.width * 0.8f, visibleSize.height * 0.6f));
    eventBox->setPosition(Vec2(visibleSize.width * 0.1f, visibleSize.height * 0.2f));
    scene->addChild(eventBox, 10);

    // ����¼�����
    auto titleLabel = Label::createWithTTF(getName(), "fonts/Marker Felt.ttf", 36);
    titleLabel->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height - 40));
    eventBox->addChild(titleLabel);

    // ����¼�����
    auto descLabel = Label::createWithTTF(getDescription(), "fonts/Marker Felt.ttf", 24);
    descLabel->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height - 100));
    descLabel->setMaxLineWidth(eventBox->getContentSize().width * 0.8f);
    eventBox->addChild(descLabel);

    // �������Ȫˮѡ��
    auto drinkButton = ui::Button::create("button.png", "button_Selected.png");
    drinkButton->setTitleText("����Ȫˮ (�ָ�30%�������ֵ)");
    drinkButton->setTitleFontSize(20);
    drinkButton->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height / 2 - 50));
    drinkButton->addTouchEventListener([this, scene, hero, eventBox](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            execute(hero);
            eventBox->removeFromParent();

            // ���ص�ͼ����
            auto mapScene = MyGame::Map::createScene();
            Director::getInstance()->replaceScene(mapScene);
        }
        });
    eventBox->addChild(drinkButton);

    // ����뿪ѡ��
    auto leaveButton = ui::Button::create("button.png", "button_Selected.png");
    leaveButton->setTitleText("�뿪");
    leaveButton->setTitleFontSize(20);
    leaveButton->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height / 2 - 120));
    leaveButton->addTouchEventListener([scene, eventBox](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            eventBox->removeFromParent();

            // ���ص�ͼ����
            auto mapScene = MyGame::Map::createScene();
            Director::getInstance()->replaceScene(mapScene);
        }
        });
    eventBox->addChild(leaveButton);
}

// �����¼�ʵ��
void MerchantEvent::execute(Hero* hero)
{
    // ����һЩ��ң�����һЩ����
    // �������Ӣ����һ��������ԣ����û�п��Ը�����Ҫ���
    // hero->setGold(hero->getGold() - 50);

    // ����Ӣ�۵�ĳЩ����
    hero->setMaxHealth(hero->getMaxHealth() + 10);
    hero->setHealth(hero->getHealth() + 10);
}

void MerchantEvent::showEventUI(cocos2d::Scene* scene, Hero* hero)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // �����¼��Ի���
    auto eventBox = ui::Layout::create();
    eventBox->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    eventBox->setBackGroundColor(Color3B(50, 50, 50));
    eventBox->setContentSize(Size(visibleSize.width * 0.8f, visibleSize.height * 0.6f));
    eventBox->setPosition(Vec2(visibleSize.width * 0.1f, visibleSize.height * 0.2f));
    scene->addChild(eventBox, 10);

    // ����¼�����
    auto titleLabel = Label::createWithTTF(getName(), "fonts/Marker Felt.ttf", 36);
    titleLabel->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height - 40));
    eventBox->addChild(titleLabel);

    // ����¼�����
    auto descLabel = Label::createWithTTF(getDescription(), "fonts/Marker Felt.ttf", 24);
    descLabel->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height - 100));
    descLabel->setMaxLineWidth(eventBox->getContentSize().width * 0.8f);
    eventBox->addChild(descLabel);

    // ��ӹ���ѡ��
    auto buyButton = ui::Button::create("button.png", "button_Selected.png");
    buyButton->setTitleText("��������ҩ�� (����10����������)");
    buyButton->setTitleFontSize(20);
    buyButton->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height / 2 - 50));
    buyButton->addTouchEventListener([this, scene, hero, eventBox](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            execute(hero);
            eventBox->removeFromParent();

            // ���ص�ͼ����
            auto mapScene = MyGame::Map::createScene();
            Director::getInstance()->replaceScene(mapScene);
        }
        });
    eventBox->addChild(buyButton);

    // ����뿪ѡ��
    auto leaveButton = ui::Button::create("button.png", "button_Selected.png");
    leaveButton->setTitleText("�뿪");
    leaveButton->setTitleFontSize(20);
    leaveButton->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height / 2 - 120));
    leaveButton->addTouchEventListener([scene, eventBox](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            eventBox->removeFromParent();

            // ���ص�ͼ����
            auto mapScene = MyGame::Map::createScene();
            Director::getInstance()->replaceScene(mapScene);
        }
        });
    eventBox->addChild(leaveButton);
}
