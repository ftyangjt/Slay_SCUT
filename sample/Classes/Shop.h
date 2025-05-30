#pragma once
#include "cocos2d.h"
#include <vector>

namespace MyGame {

    class ShopScene : public cocos2d::Scene {
    public:
        static cocos2d::Scene* createScene();
        virtual bool init() override;
        void showGoodsColumn();
        virtual ~ShopScene(); // �����������������

    private:
        std::vector<cocos2d::Sprite*> _cards; // �洢���п���ָ��
        cocos2d::EventListenerMouse* _mouseListener = nullptr; // ����¼�������
        cocos2d::Sprite* _hoveredCard = nullptr; // ��ǰ��ͣ�Ŀ���

        CREATE_FUNC(ShopScene);
    };

} // namespace MyGame
