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

    // ��������¼��ĺ���
    void triggerRandomEvent();

    // �����齱ת��
    void createLuckyWheel();

    // �齱��ť�ص�
    void onSpinButtonClicked(cocos2d::Ref* pSender);

    // ִ��ת����ת����
    void spinWheel(int result);

    // ��ʾ�齱���
    void showWheelResult(int result);

    // implement the "static create()" method manually
    CREATE_FUNC(Question);

private:
    cocos2d::Sprite* _wheelSprite;            // ת�̾���
    cocos2d::Sprite* _pointerSprite;          // ָ�뾫��
    cocos2d::MenuItemImage* _spinButton;      // �齱��ť
    cocos2d::Label* _eventLabel;              // �¼�������ǩ
    bool _isWheelSpinning;                    // ת���Ƿ�������ת
};

#endif // __QUESTION_SCENE_H__
