#ifndef __REST_SCENE_H__
#define __REST_SCENE_H__

#include "cocos2d.h"

namespace MyGame {

    class Rest : public cocos2d::Scene
    {
    public:
        static cocos2d::Scene* createScene();
        virtual bool init();
        // ʵ�֡���̬ create() ������
        CREATE_FUNC(Rest);

    private:
        void menuRestoreCallback(cocos2d::Ref* pSender);
        void menuReturnCallback(cocos2d::Ref* pSender);
    };

} // namespace MyGame

#endif // __REST_SCENE_H__

