#ifndef __RANDOM_EVENT_H__
#define __RANDOM_EVENT_H__

#include "cocos2d.h"
#include "Hero.h"

// 随机事件的基类
class RandomEvent
{
public:
    virtual ~RandomEvent() {}

    // 获取事件名称
    virtual std::string getName() const = 0;

    // 获取事件描述
    virtual std::string getDescription() const = 0;

    // 执行事件效果
    virtual void execute(Hero* hero) = 0;

    // 显示事件UI
    virtual void showEventUI(cocos2d::Scene* scene, Hero* hero) = 0;
};

// 遭遇祭坛的事件
class AltarEvent : public RandomEvent
{
public:
    std::string getName() const override { return "神秘祭坛"; }
    std::string getDescription() const override { return "你发现了一个古老的祭坛，你可以献祭一些生命来获取力量。"; }

    void execute(Hero* hero) override;
    void showEventUI(cocos2d::Scene* scene, Hero* hero) override;
};

// 遭遇宝箱事件
class TreasureEvent : public RandomEvent
{
public:
    std::string getName() const override { return "神秘宝箱"; }
    std::string getDescription() const override { return "你发现了一个闪闪发光的宝箱，但它可能藏有陷阱。"; }

    void execute(Hero* hero) override;
    void showEventUI(cocos2d::Scene* scene, Hero* hero) override;
};

// 治疗泉水事件
class HealingFountainEvent : public RandomEvent
{
public:
    std::string getName() const override { return "治疗泉水"; }
    std::string getDescription() const override { return "你发现了一个散发着治愈光芒的泉水。"; }

    void execute(Hero* hero) override;
    void showEventUI(cocos2d::Scene* scene, Hero* hero) override;
};

// 商人事件
class MerchantEvent : public RandomEvent
{
public:
    std::string getName() const override { return "流浪商人"; }
    std::string getDescription() const override { return "一个神秘的商人向你兜售各种奇怪的物品。"; }

    void execute(Hero* hero) override;
    void showEventUI(cocos2d::Scene* scene, Hero* hero) override;
};

// 随机事件管理器，用于创建随机事件
class RandomEventManager
{
public:
    static RandomEventManager* getInstance();

    // 获取一个随机事件
    std::shared_ptr<RandomEvent> getRandomEvent();

    // 初始化事件集
    void initEvents();

private:
    RandomEventManager() { initEvents(); }
    ~RandomEventManager() {}

    static RandomEventManager* _instance;
    std::vector<std::shared_ptr<RandomEvent>> _events;
};

#endif // __RANDOM_EVENT_H__
