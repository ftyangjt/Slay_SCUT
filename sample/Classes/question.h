#ifndef __QUESTION_SCENE_H__
#define __QUESTION_SCENE_H__

#include "cocos2d.h"

class Question : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // ʵ�ַ��ذ�ť�Ļص�����
    void menuReturnCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(Question);
};

#endif // __QUESTION_SCENE_H__

