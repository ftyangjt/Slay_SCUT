#pragma once
#include "cocos2d.h"
#include <vector>

namespace MyGame {

    class ShopScene : public cocos2d::Scene {
    public:
        static cocos2d::Scene* createScene();
        virtual bool init() override;
        void showGoodsColumn();
        virtual ~ShopScene(); // 添加析构函数的声明

    private:
        std::vector<cocos2d::Sprite*> _cards; // 存储所有卡牌指针
        cocos2d::EventListenerMouse* _mouseListener = nullptr; // 鼠标事件监听器
        cocos2d::Sprite* _hoveredCard = nullptr; // 当前悬停的卡牌

        CREATE_FUNC(ShopScene);
    };

} // namespace MyGame
