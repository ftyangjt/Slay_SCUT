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

        // ���ú�ɫ����
        auto background = LayerColor::create(Color4B(0, 0, 0, 255));
        this->addChild(background);

        // ���ع�������ͼƬ
        _transitionImage = Sprite::create("transition_image.png");
        if (_transitionImage) {
            // ����ͼƬ��ʼλ������Ļ����
            _transitionImage->setPosition(Vec2(
                origin.x + visibleSize.width / 2,
                origin.y + visibleSize.height / 2
            ));

            // ������ʼ���ű�����ʹͼƬ�ʵ���ʾ����С��
            float initialScale = 0.8f;
            _transitionImage->setScale(initialScale);

            this->addChild(_transitionImage);

            // �������Ŷ�������ʱ��
            _moveDuration = 4.0f; // 8��Ļ�������

            // ����Ŀ�����ű������ϴ�
            float targetScale = 1.2f;

            // �������Ŷ���
            auto scaleAction = ScaleTo::create(_moveDuration, targetScale);
            auto easeAction = EaseInOut::create(scaleAction, 2.0f); // ʹ�û���Ч��ʹ���Ÿ���Ȼ

            // ������ɺ�����ͼ����
            auto callbackAction = CallFunc::create(CC_CALLBACK_0(TransitionScene::enterMapScene, this));

            // ִ�ж�������
            _transitionImage->runAction(Sequence::create(easeAction, callbackAction, nullptr));

            // ���Ź������֣�����У�
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/transition.mp3", false);
        }

        // ��ӵ����������������ҵ��������������
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->onTouchBegan = [this](Touch* touch, Event* event) -> bool {
            this->enterMapScene();
            return true;
            };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

        return true;
    }

    void TransitionScene::enterMapScene() {
        // ֹͣ���ж�������ֹ�ص�����δ���
        this->stopAllActions();
        if (_transitionImage) {
            _transitionImage->stopAllActions();
        }

        // �л�����ͼ����
        auto scene = MyGame::Map::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene, Color3B(0, 0, 0)));
    }

} // namespace MyGame