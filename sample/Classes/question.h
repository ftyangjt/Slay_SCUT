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

    // implement the "static create()" method manually
    CREATE_FUNC(Question);
};

#endif // __QUESTION_SCENE_H__
