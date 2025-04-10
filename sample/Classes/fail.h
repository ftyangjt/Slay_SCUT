#ifndef __FAIL_SCENE_H__
#define __FAIL_SCENE_H__

#include "cocos2d.h"

class FailScene : public cocos2d::Scene
{
public:
    // 创建场景的静态方法
    static cocos2d::Scene* createScene();

    // 初始化方法
    virtual bool init();

    // 实现"static create()"方法
    CREATE_FUNC(FailScene);

private:
    // 创建背景
    void createBackground();

    // 创建失败提示和返回按钮
    void createUI();
};

#endif // __FAIL_SCENE_H__
