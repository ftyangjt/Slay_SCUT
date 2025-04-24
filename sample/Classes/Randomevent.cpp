#include "RandomEvent.h"
#include "ui/CocosGUI.h"
#include <random>

USING_NS_CC;

// 静态实例初始化
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
    // 注册所有事件类型
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

    // 随机选择一个事件
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, _events.size() - 1);
    int index = dis(gen);

    return _events[index];
}

// 祭坛事件实现
void AltarEvent::execute(Hero* hero)
{
    // 扣除一些血量，给予额外的力量
    int healthLoss = hero->getMaxHealth() * 0.1; // 减少10%的最大血量
    hero->setHealth(hero->getHealth() - healthLoss);

    // 这里可以增加英雄的力量或者给予某种Buff
    // 假设有一个增加力量的方法
    // hero->increasePower(2);
}

void AltarEvent::showEventUI(cocos2d::Scene* scene, Hero* hero)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // 创建事件对话框
    auto eventBox = ui::Layout::create();
    eventBox->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    eventBox->setBackGroundColor(Color3B(50, 50, 50));
    eventBox->setContentSize(Size(visibleSize.width * 0.8f, visibleSize.height * 0.6f));
    eventBox->setPosition(Vec2(visibleSize.width * 0.1f, visibleSize.height * 0.2f));
    scene->addChild(eventBox, 10);

    // 添加事件标题
    auto titleLabel = Label::createWithTTF(getName(), "fonts/Marker Felt.ttf", 36);
    titleLabel->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height - 40));
    eventBox->addChild(titleLabel);

    // 添加事件描述
    auto descLabel = Label::createWithTTF(getDescription(), "fonts/Marker Felt.ttf", 24);
    descLabel->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height - 100));
    descLabel->setMaxLineWidth(eventBox->getContentSize().width * 0.8f);
    eventBox->addChild(descLabel);

    // 添加献祭选项
    auto sacrificeButton = ui::Button::create("button.png", "button_Selected.png");
    sacrificeButton->setTitleText("献祭 (损失10%血量，获得力量)");
    sacrificeButton->setTitleFontSize(20);
    sacrificeButton->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height / 2 - 50));
    sacrificeButton->addTouchEventListener([this, scene, hero, eventBox](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            execute(hero);
            eventBox->removeFromParent();

            // 返回地图场景
            auto mapScene = MyGame::Map::createScene();
            Director::getInstance()->replaceScene(mapScene);
        }
        });
    eventBox->addChild(sacrificeButton);

    // 添加离开选项
    auto leaveButton = ui::Button::create("button.png", "button_Selected.png");
    leaveButton->setTitleText("离开");
    leaveButton->setTitleFontSize(20);
    leaveButton->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height / 2 - 120));
    leaveButton->addTouchEventListener([scene, eventBox](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            eventBox->removeFromParent();

            // 返回地图场景
            auto mapScene = MyGame::Map::createScene();
            Director::getInstance()->replaceScene(mapScene);
        }
        });
    eventBox->addChild(leaveButton);
}

// 宝箱事件实现
void TreasureEvent::execute(Hero* hero)
{
    // 随机决定是好事还是坏事
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10);
    int result = dis(gen);

    if (result <= 7) { // 70%几率获得好处
        // 这里可以给予英雄一些奖励，比如增加生命上限
        hero->setMaxHealth(hero->getMaxHealth() + 5);
        hero->setHealth(hero->getHealth() + 5);
    }
    else { // 30%几率受到伤害
        // 受到一些伤害
        int damage = hero->getMaxHealth() * 0.15; // 受到15%的最大生命值伤害
        hero->setHealth(hero->getHealth() - damage);
    }
}

void TreasureEvent::showEventUI(cocos2d::Scene* scene, Hero* hero)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // 创建事件对话框
    auto eventBox = ui::Layout::create();
    eventBox->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    eventBox->setBackGroundColor(Color3B(50, 50, 50));
    eventBox->setContentSize(Size(visibleSize.width * 0.8f, visibleSize.height * 0.6f));
    eventBox->setPosition(Vec2(visibleSize.width * 0.1f, visibleSize.height * 0.2f));
    scene->addChild(eventBox, 10);

    // 添加事件标题
    auto titleLabel = Label::createWithTTF(getName(), "fonts/Marker Felt.ttf", 36);
    titleLabel->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height - 40));
    eventBox->addChild(titleLabel);

    // 添加事件描述
    auto descLabel = Label::createWithTTF(getDescription(), "fonts/Marker Felt.ttf", 24);
    descLabel->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height - 100));
    descLabel->setMaxLineWidth(eventBox->getContentSize().width * 0.8f);
    eventBox->addChild(descLabel);

    // 添加打开宝箱选项
    auto openButton = ui::Button::create("button.png", "button_Selected.png");
    openButton->setTitleText("打开宝箱 (70%几率增加5点生命上限，30%几率损失15%血量)");
    openButton->setTitleFontSize(15);
    openButton->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height / 2 - 50));
    openButton->addTouchEventListener([this, scene, hero, eventBox](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            execute(hero);
            eventBox->removeFromParent();

            // 返回地图场景
            auto mapScene = MyGame::Map::createScene();
            Director::getInstance()->replaceScene(mapScene);
        }
        });
    eventBox->addChild(openButton);

    // 添加离开选项
    auto leaveButton = ui::Button::create("button.png", "button_Selected.png");
    leaveButton->setTitleText("离开");
    leaveButton->setTitleFontSize(20);
    leaveButton->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height / 2 - 120));
    leaveButton->addTouchEventListener([scene, eventBox](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            eventBox->removeFromParent();

            // 返回地图场景
            auto mapScene = MyGame::Map::createScene();
            Director::getInstance()->replaceScene(mapScene);
        }
        });
    eventBox->addChild(leaveButton);
}

// 治疗泉水事件实现
void HealingFountainEvent::execute(Hero* hero)
{
    // 恢复部分生命值
    int healing = hero->getMaxHealth() * 0.3; // 恢复30%的最大生命值
    int newHealth = hero->getHealth() + healing;

    // 确保不超过最大生命值
    if (newHealth > hero->getMaxHealth()) {
        newHealth = hero->getMaxHealth();
    }

    hero->setHealth(newHealth);
}

void HealingFountainEvent::showEventUI(cocos2d::Scene* scene, Hero* hero)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // 创建事件对话框
    auto eventBox = ui::Layout::create();
    eventBox->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    eventBox->setBackGroundColor(Color3B(50, 50, 50));
    eventBox->setContentSize(Size(visibleSize.width * 0.8f, visibleSize.height * 0.6f));
    eventBox->setPosition(Vec2(visibleSize.width * 0.1f, visibleSize.height * 0.2f));
    scene->addChild(eventBox, 10);

    // 添加事件标题
    auto titleLabel = Label::createWithTTF(getName(), "fonts/Marker Felt.ttf", 36);
    titleLabel->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height - 40));
    eventBox->addChild(titleLabel);

    // 添加事件描述
    auto descLabel = Label::createWithTTF(getDescription(), "fonts/Marker Felt.ttf", 24);
    descLabel->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height - 100));
    descLabel->setMaxLineWidth(eventBox->getContentSize().width * 0.8f);
    eventBox->addChild(descLabel);

    // 添加饮用泉水选项
    auto drinkButton = ui::Button::create("button.png", "button_Selected.png");
    drinkButton->setTitleText("饮用泉水 (恢复30%最大生命值)");
    drinkButton->setTitleFontSize(20);
    drinkButton->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height / 2 - 50));
    drinkButton->addTouchEventListener([this, scene, hero, eventBox](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            execute(hero);
            eventBox->removeFromParent();

            // 返回地图场景
            auto mapScene = MyGame::Map::createScene();
            Director::getInstance()->replaceScene(mapScene);
        }
        });
    eventBox->addChild(drinkButton);

    // 添加离开选项
    auto leaveButton = ui::Button::create("button.png", "button_Selected.png");
    leaveButton->setTitleText("离开");
    leaveButton->setTitleFontSize(20);
    leaveButton->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height / 2 - 120));
    leaveButton->addTouchEventListener([scene, eventBox](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            eventBox->removeFromParent();

            // 返回地图场景
            auto mapScene = MyGame::Map::createScene();
            Director::getInstance()->replaceScene(mapScene);
        }
        });
    eventBox->addChild(leaveButton);
}

// 商人事件实现
void MerchantEvent::execute(Hero* hero)
{
    // 花费一些金币，增加一些属性
    // 这里假设英雄有一个金币属性，如果没有可以根据需要添加
    // hero->setGold(hero->getGold() - 50);

    // 增加英雄的某些属性
    hero->setMaxHealth(hero->getMaxHealth() + 10);
    hero->setHealth(hero->getHealth() + 10);
}

void MerchantEvent::showEventUI(cocos2d::Scene* scene, Hero* hero)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // 创建事件对话框
    auto eventBox = ui::Layout::create();
    eventBox->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    eventBox->setBackGroundColor(Color3B(50, 50, 50));
    eventBox->setContentSize(Size(visibleSize.width * 0.8f, visibleSize.height * 0.6f));
    eventBox->setPosition(Vec2(visibleSize.width * 0.1f, visibleSize.height * 0.2f));
    scene->addChild(eventBox, 10);

    // 添加事件标题
    auto titleLabel = Label::createWithTTF(getName(), "fonts/Marker Felt.ttf", 36);
    titleLabel->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height - 40));
    eventBox->addChild(titleLabel);

    // 添加事件描述
    auto descLabel = Label::createWithTTF(getDescription(), "fonts/Marker Felt.ttf", 24);
    descLabel->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height - 100));
    descLabel->setMaxLineWidth(eventBox->getContentSize().width * 0.8f);
    eventBox->addChild(descLabel);

    // 添加购买选项
    auto buyButton = ui::Button::create("button.png", "button_Selected.png");
    buyButton->setTitleText("购买神秘药剂 (增加10点生命上限)");
    buyButton->setTitleFontSize(20);
    buyButton->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height / 2 - 50));
    buyButton->addTouchEventListener([this, scene, hero, eventBox](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            execute(hero);
            eventBox->removeFromParent();

            // 返回地图场景
            auto mapScene = MyGame::Map::createScene();
            Director::getInstance()->replaceScene(mapScene);
        }
        });
    eventBox->addChild(buyButton);

    // 添加离开选项
    auto leaveButton = ui::Button::create("button.png", "button_Selected.png");
    leaveButton->setTitleText("离开");
    leaveButton->setTitleFontSize(20);
    leaveButton->setPosition(Vec2(eventBox->getContentSize().width / 2, eventBox->getContentSize().height / 2 - 120));
    leaveButton->addTouchEventListener([scene, eventBox](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            eventBox->removeFromParent();

            // 返回地图场景
            auto mapScene = MyGame::Map::createScene();
            Director::getInstance()->replaceScene(mapScene);
        }
        });
    eventBox->addChild(leaveButton);
}
