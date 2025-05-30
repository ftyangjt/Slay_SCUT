#pragma once
#include "cocos2d.h"

namespace MyGame {

    class VictoryScene : public cocos2d::Scene {
    public:
        static cocos2d::Scene* createScene();
        virtual bool init() override;

        // ����������Ա����
        void scrollCredits();

        // ���ز˵�����
        void returnToMenu();

        CREATE_FUNC(VictoryScene);

    private:
        cocos2d::Label* _creditsLabel;
        cocos2d::Sprite* _victoryLogo;
        cocos2d::Node* _scrollNode;  // ���ӹ����ڵ�
        float _creditsDuration;      // ����ʱ��
    };

} // namespace MyGame
