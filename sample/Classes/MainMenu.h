#ifndef __MAIN_MENU_H__
#define __MAIN_MENU_H__

#include "cocos2d.h"

class MainMenu : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    // ����ʼ��Ϸ����ť�ص�
    void menuStartCallback(cocos2d::Ref* pSender);

    // ���˳���Ϸ����ť�ص�
    void menuCloseCallback(cocos2d::Ref* pSender);

    CREATE_FUNC(MainMenu);
};

#endif // __MAIN_MENU_H__
