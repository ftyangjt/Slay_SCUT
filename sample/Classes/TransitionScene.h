#ifndef __TRANSITION_SCENE_H__
#define __TRANSITION_SCENE_H__

#include "cocos2d.h"

namespace MyGame {
    class TransitionScene : public cocos2d::Scene {
    public:
        static cocos2d::Scene* createScene();
        virtual bool init();

        // ͼƬ�ƶ���ɺ�����ͼ����
        void enterMapScene();

        CREATE_FUNC(TransitionScene);

    private:
        cocos2d::Sprite* _transitionImage;
        float _moveDuration; // �ƶ���������ʱ��
    };
}

#endif // __TRANSITION_SCENE_H__