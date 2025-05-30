#ifndef __REST_SCENE_H__
#define __REST_SCENE_H__

#include "cocos2d.h"

namespace MyGame {

    class Rest : public cocos2d::Scene
    {
    public:
        static cocos2d::Scene* createScene();
        virtual bool init();
        // 实现。。。静态 create() 函数
        CREATE_FUNC(Rest);

    private:
        void menuRestoreCallback(cocos2d::Ref* pSender);
        void menuReturnCallback(cocos2d::Ref* pSender);
        bool m_restoreUsed; // 记录恢复血量操作是否已执行
        bool m_increaseMaxHealthUsed; // 记录增加最大血量操作是否已执行
    };

} // namespace MyGame
#endif // __REST_SCENE_H__