#pragma once
#include "cocos2d.h"

namespace MyGame {

    class VictoryScene : public cocos2d::Scene {
    public:
        static cocos2d::Scene* createScene();
        virtual bool init() override;

        // 滚动制作人员名单
        void scrollCredits();

        // 返回菜单界面
        void returnToMenu();

        CREATE_FUNC(VictoryScene);

    private:
        cocos2d::Label* _creditsLabel;
        cocos2d::Label* _victoryInfoLabel;
        cocos2d::Node* _scrollNode;  // 滚动节点
        float _creditsDuration;      // 滚动时间
    };

} // namespace MyGame