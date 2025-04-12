#ifndef __RANDOM_EVENT_H__
#define __RANDOM_EVENT_H__

#include "cocos2d.h"
#include "Hero.h"

// ����¼��Ļ���
class RandomEvent
{
public:
    virtual ~RandomEvent() {}

    // ��ȡ�¼�����
    virtual std::string getName() const = 0;

    // ��ȡ�¼�����
    virtual std::string getDescription() const = 0;

    // ִ���¼�Ч��
    virtual void execute(Hero* hero) = 0;

    // ��ʾ�¼�UI
    virtual void showEventUI(cocos2d::Scene* scene, Hero* hero) = 0;
};

// ������̳���¼�
class AltarEvent : public RandomEvent
{
public:
    std::string getName() const override { return "���ؼ�̳"; }
    std::string getDescription() const override { return "�㷢����һ�����ϵļ�̳��������׼�һЩ��������ȡ������"; }

    void execute(Hero* hero) override;
    void showEventUI(cocos2d::Scene* scene, Hero* hero) override;
};

// ���������¼�
class TreasureEvent : public RandomEvent
{
public:
    std::string getName() const override { return "���ر���"; }
    std::string getDescription() const override { return "�㷢����һ����������ı��䣬�������ܲ������塣"; }

    void execute(Hero* hero) override;
    void showEventUI(cocos2d::Scene* scene, Hero* hero) override;
};

// ����Ȫˮ�¼�
class HealingFountainEvent : public RandomEvent
{
public:
    std::string getName() const override { return "����Ȫˮ"; }
    std::string getDescription() const override { return "�㷢����һ��ɢ����������â��Ȫˮ��"; }

    void execute(Hero* hero) override;
    void showEventUI(cocos2d::Scene* scene, Hero* hero) override;
};

// �����¼�
class MerchantEvent : public RandomEvent
{
public:
    std::string getName() const override { return "��������"; }
    std::string getDescription() const override { return "һ�����ص��������㶵�۸�����ֵ���Ʒ��"; }

    void execute(Hero* hero) override;
    void showEventUI(cocos2d::Scene* scene, Hero* hero) override;
};

// ����¼������������ڴ�������¼�
class RandomEventManager
{
public:
    static RandomEventManager* getInstance();

    // ��ȡһ������¼�
    std::shared_ptr<RandomEvent> getRandomEvent();

    // ��ʼ���¼���
    void initEvents();

private:
    RandomEventManager() { initEvents(); }
    ~RandomEventManager() {}

    static RandomEventManager* _instance;
    std::vector<std::shared_ptr<RandomEvent>> _events;
};

#endif // __RANDOM_EVENT_H__
