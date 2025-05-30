#ifndef __MAIN_MENU_H__
#define __MAIN_MENU_H__

#include "cocos2d.h"

class MainMenu : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    // “开始游戏”按钮回调
    void menuStartCallback(cocos2d::Ref* pSender);

    // “退出游戏”按钮回调
    void menuCloseCallback(cocos2d::Ref* pSender);

    void createContinueButton();
    void loadGame();

    CREATE_FUNC(MainMenu);
};

#endif // __MAIN_MENU_H__
