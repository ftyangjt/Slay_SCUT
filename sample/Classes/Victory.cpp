#include "Victory.h"
#include "SimpleAudioEngine.h"
#include "MainMenu.h" // 导入主菜单场景，根据实际情况调整文件名

USING_NS_CC;

namespace MyGame {

    Scene* VictoryScene::createScene() {
        return VictoryScene::create();
    }

    bool VictoryScene::init() {
        if (!Scene::init()) {
            return false;
        }

        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();

        // 播放胜利音乐
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/victory.mp3", false);

        // 黑色背景
        auto background = LayerColor::create(Color4B(0, 0, 0, 255));
        this->addChild(background);

        // 创建一个节点来包含所有滚动内容
        // 修改初始位置：放置在正好在屏幕底部以下，这样滚动时会自然进入屏幕
        auto scrollNode = Node::create();
        scrollNode->setPosition(Vec2(visibleSize.width / 2, -200)); // 改为水平居中，初始在屏幕底部
        scrollNode->setVisible(false); // 先隐藏滚动内容
        this->addChild(scrollNode);

        // 将 _scrollNode 设置为成员变量，以便在滚动方法中使用
        _scrollNode = scrollNode;

        // 删除胜利标志部分
        // 不再创建和添加 victoryLabel 和 _victoryLogo

        // 创建制作人员名单文本
        std::string credits =
            "Core Team\n\n"

            "Game designers\n"
            "Jingtian Yang\n\n"

            "Program design\n"
            "Tingtian Yang\n"
            "Zhiqirui Ye\n"
            "Baiding Dend\n\n"

            "Art Design\n"
            "Xingyi Pan\n\n"

            "Testing Support\n"
            "Tingtian Yang\n"
            "Zhiqirui Ye\n"
            "Baiding Dend\n"
            "Xingyi Pan\n\n\n"

            "Creative Background\n"
            "This project originated from the combination of the \n"
            "interest in C++ programming practice and game development.\n "
            "It aims to deeply master the application of the C++ language \n"
            "in practical projects by creating an interesting game, \n"
            "and at the same time explore the principles of game mechanism \n"
            "construction and gameplay design.\n "
            "Therefore, it was selected to develop this game as\n"
            "the C++ project assignment of South China University of Technology \n"
            "in 2025.\n\n\n"

            "Copyright Notice\n"
            "The game mechanics of this game refer to \n"
            "Slay the Spire. \n"
            "The story setting of this game is original. \n"
            "The art resources of this game are basically original, \n"
            "and some refer to online resources. \n"
            "This game will not be released externally\n"
            "and is not used for commercial purposes, \n\n"

            "This game is the C++ project assignment of \n"
            "South China University of Technology in 2025.\n"


            "All rights and interests related to this game \n"
            "are safeguarded in accordance with \n"
            "applicable laws and regulations.\n\n\n"

            "Special thanks\n"
            "Technical Support\n"
            "Cocos2d\n\n"

            "Game Inspiration\n"
            "Slay the Spire\n\n"

            "All the players participating in the test\n\n"

            "Thanks for your playing!\n\n"

            "2025 C++ homework";

        // 创建文本标签
        _creditsLabel = Label::createWithTTF(credits, "fonts/Marker Felt.ttf", 90);
        _creditsLabel->setAlignment(TextHAlignment::CENTER);
        _creditsLabel->setColor(Color3B::WHITE);

        // 直接将文本标签放在滚动节点的顶部
        _creditsLabel->setPosition(Vec2(0, 0));
        scrollNode->addChild(_creditsLabel);

        // 设置滚动持续时间
        _creditsDuration = 20.0f;

        // 创建一个大的胜利信息标签，显示在屏幕中央
        auto victoryInfoLabel = Label::createWithTTF("Congratulations!!!\nVictory!!!!", "fonts/Marker Felt.ttf", 120);
        victoryInfoLabel->setAlignment(TextHAlignment::CENTER);
        victoryInfoLabel->setColor(Color3B(255, 215, 0)); // 金色
        victoryInfoLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        victoryInfoLabel->setOpacity(0); // 开始时透明
        this->addChild(victoryInfoLabel, 10);
        _victoryInfoLabel = victoryInfoLabel;

        // 为胜利信息标签添加淡入淡出动画
        auto fadeIn = FadeIn::create(0.5f);
        auto delay = DelayTime::create(2.0f);
        auto fadeOut = FadeOut::create(0.5f);
        auto showCredits = CallFunc::create([this]() {
            // 显示滚动内容并开始制作人员名单滚动
            _scrollNode->setVisible(true);
            this->scrollCredits();
            });

        // 执行动画序列：淡入 -> 延迟2秒 -> 淡出 -> 显示制作人员名单
        victoryInfoLabel->runAction(Sequence::create(
            fadeIn,
            delay,
            fadeOut,
            showCredits,
            nullptr
        ));

        // 添加按键监听器，用于跳过字幕
        auto keyboardListener = EventListenerKeyboard::create();
        keyboardListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
            // 任意键可以跳过
            this->returnToMenu();
            };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

        // 添加触摸监听器，用于跳过字幕（适用于移动设备）
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->onTouchBegan = [this](Touch* touch, Event* event) -> bool {
            this->returnToMenu();
            return true;
            };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

        return true;
    }

    void VictoryScene::scrollCredits() {
        auto visibleSize = Director::getInstance()->getVisibleSize();

        // 计算滚动节点内容的总高度 - 现在只有制作人员名单文本
        float contentHeight = _creditsLabel->getContentSize().height;

        // 计算滚动距离：从屏幕底部滚动到内容完全离开屏幕顶部
        float startY = 0; // 初始位置在屏幕底部
        float endY = visibleSize.height + contentHeight; // 终点位置是内容完全离开屏幕顶部

        // 创建滚动动作：从当前位置（屏幕底部）滚动到屏幕上方（使内容完全显示后消失）
        auto scrollAction = MoveTo::create(_creditsDuration, Vec2(_scrollNode->getPositionX(), endY));

        // 使用更平滑的缓动效果
        auto easeAction = EaseInOut::create(scrollAction, 1.5f);

        // 在滚动结束后返回菜单
        auto sequence = Sequence::create(
            easeAction,
            DelayTime::create(1.0f), // 结束后等待1秒
            CallFunc::create(CC_CALLBACK_0(VictoryScene::returnToMenu, this)),
            nullptr
        );

        // 执行动作
        _scrollNode->runAction(sequence);
    }

    void VictoryScene::returnToMenu() {
        // 停止所有正在进行的动作
        this->stopAllActions();
        if (_scrollNode) {
            _scrollNode->stopAllActions();
        }

        // 切换回主菜单场景
        auto menuScene = MainMenu::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(2.0f, menuScene, Color3B(0, 0, 0)));
    }

} // namespace MyGame