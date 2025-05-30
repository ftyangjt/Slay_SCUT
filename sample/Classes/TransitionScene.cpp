#include "TransitionScene.h"
#include "Map.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

namespace MyGame {

    Scene* TransitionScene::createScene() {
        return TransitionScene::create();
    }

    bool TransitionScene::init() {
        if (!Scene::init()) {
            return false;
        }

        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();

        // 设置黑色背景
        auto background = LayerColor::create(Color4B(0, 0, 0, 255));
        this->addChild(background);

        // 加载过场动画图片
        _transitionImage = Sprite::create("transition_image.png");
        if (_transitionImage) {
            // 设置图片初始位置在屏幕中央
            _transitionImage->setPosition(Vec2(
                origin.x + visibleSize.width / 2,
                origin.y + visibleSize.height / 2
            ));

            // 调整初始缩放比例，使图片适当显示（较小）
            float initialScale = 0.8f;
            _transitionImage->setScale(initialScale);

            this->addChild(_transitionImage);

            // 设置缩放动画持续时间
            _moveDuration = 4.0f; // 8秒的缓慢缩放

            // 计算目标缩放比例（较大）
            float targetScale = 1.2f;

            // 创建缩放动画
            auto scaleAction = ScaleTo::create(_moveDuration, targetScale);
            auto easeAction = EaseInOut::create(scaleAction, 2.0f); // 使用缓动效果使缩放更自然

            // 缩放完成后进入地图场景
            auto callbackAction = CallFunc::create(CC_CALLBACK_0(TransitionScene::enterMapScene, this));

            // 执行动画序列
            _transitionImage->runAction(Sequence::create(easeAction, callbackAction, nullptr));

            // 播放过场音乐（如果有）
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/transition.mp3", false);
        }

        // 添加点击监听器，允许玩家点击跳过过场动画
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->onTouchBegan = [this](Touch* touch, Event* event) -> bool {
            this->enterMapScene();
            return true;
            };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

        return true;
    }

    void TransitionScene::enterMapScene() {
        // 停止所有动作，防止回调被多次触发
        this->stopAllActions();
        if (_transitionImage) {
            _transitionImage->stopAllActions();
        }

        // 切换到地图场景
        auto scene = MyGame::Map::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene, Color3B(0, 0, 0)));
    }

} // namespace MyGame