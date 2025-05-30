#ifndef __TRANSITION_SCENE_H__
#define __TRANSITION_SCENE_H__

#include "cocos2d.h"

namespace MyGame {
    class TransitionScene : public cocos2d::Scene {
    public:
        static cocos2d::Scene* createScene();
        virtual bool init();

        // 图片移动完成后进入地图场景
        void enterMapScene();

        CREATE_FUNC(TransitionScene);

    private:
        cocos2d::Sprite* _transitionImage;
        float _moveDuration; // 移动动画持续时间
    };
}

#endif // __TRANSITION_SCENE_H__