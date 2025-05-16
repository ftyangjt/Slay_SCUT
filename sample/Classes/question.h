#ifndef __QUESTION_SCENE_H__
#define __QUESTION_SCENE_H__

#include "cocos2d.h"

class Question : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // 实现返回按钮的回调函数
    void menuReturnCallback(cocos2d::Ref* pSender);

    // 触发随机事件的函数
    void triggerRandomEvent();

    // 创建抽奖转盘
    void createLuckyWheel();

    // 抽奖按钮回调
    void onSpinButtonClicked(cocos2d::Ref* pSender);

    // 执行转盘旋转动画
    void spinWheel(int result);

    // 显示抽奖结果
    void showWheelResult(int result);

    // implement the "static create()" method manually
    CREATE_FUNC(Question);

private:
    cocos2d::Sprite* _wheelSprite;            // 转盘精灵
    cocos2d::Sprite* _pointerSprite;          // 指针精灵
    cocos2d::MenuItemImage* _spinButton;      // 抽奖按钮
    cocos2d::Label* _eventLabel;              // 事件描述标签
    bool _isWheelSpinning;                    // 转盘是否正在旋转
};

#endif // __QUESTION_SCENE_H__
