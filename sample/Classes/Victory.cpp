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
        auto scrollNode = Node::create();
        scrollNode->setPosition(Vec2(visibleSize.width / 2, -visibleSize.height)); // 初始位置在屏幕下方
        this->addChild(scrollNode);

        // 将 _scrollNode 设置为成员变量，以便在滚动方法中使用
        _scrollNode = scrollNode;

        // 创建胜利标志（直接添加到滚动节点而不是场景）
        Label* victoryLabel = nullptr;
        _victoryLogo = Sprite::create("victory_logo.png");

        if (!_victoryLogo) {
            // 如果图片不存在，创建文字代替
            victoryLabel = Label::createWithTTF(" ", "fonts/Marker Felt.ttf", 150); // 更大的字体
            victoryLabel->setColor(Color3B(255, 215, 0)); // 金色
            // 将垂直位置调整得更高
            victoryLabel->setPosition(Vec2(0, visibleSize.height * 1.2f));
            scrollNode->addChild(victoryLabel);
        }
        else {
            // 将垂直位置调整得更高
            _victoryLogo->setPosition(Vec2(0, visibleSize.height * 1.2f));
            _victoryLogo->setScale(1.2f); // 稍微放大
            scrollNode->addChild(_victoryLogo);

            // 添加缩放动画以吸引注意
            _victoryLogo->setScale(0.1f);
            auto scaleUp = ScaleTo::create(1.0f, 1.2f);
            auto elastic = EaseElasticOut::create(scaleUp);
            _victoryLogo->runAction(elastic);
        }

        // 创建制作人员名单文本
        std::string credits =
            "Victory!!!\n\n\n\n\n\n\n\n"

            "Relevant Instructions\n\n\n"

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

        // 创建文本标签（字体大小从36增加到60）
        _creditsLabel = Label::createWithTTF(credits, "fonts/Marker Felt.ttf", 72);
        _creditsLabel->setAlignment(TextHAlignment::CENTER);
        _creditsLabel->setColor(Color3B::WHITE);

        // 设置标签位置在"Victory"之下，增加间距以避免重叠
        float creditY = (victoryLabel) ?
            victoryLabel->getPositionY() - victoryLabel->getContentSize().height - 200.0f : // 增加间距
            _victoryLogo->getPositionY() - _victoryLogo->getContentSize().height - 200.0f; // 增加间距

        _creditsLabel->setPosition(Vec2(0, creditY));
        scrollNode->addChild(_creditsLabel);

        // 设置滚动持续时间（从15秒延长到30秒）
        _creditsDuration = 30.0f; // 30秒滚动时间，减慢滚动速度

        // 在短暂延迟后开始滚动制作人员名单
        this->runAction(Sequence::create(
            DelayTime::create(2.0f), // 等待2秒
            CallFunc::create(CC_CALLBACK_0(VictoryScene::scrollCredits, this)),
            nullptr
        ));

        return true;
    }

    void VictoryScene::scrollCredits() {
        auto visibleSize = Director::getInstance()->getVisibleSize();

        // 计算滚动节点的初始高度（包含胜利标志和制作人员文本的总高度）
        float contentHeight = _creditsLabel->getContentSize().height;
        if (_victoryLogo) {
            contentHeight += _victoryLogo->getContentSize().height + 200; // 增加间距以符合前面的设置
        }

        // 计算滚动的目标位置（屏幕顶部上方）
        float targetY = visibleSize.height * 2 + contentHeight / 2;

        // 创建滚动动作
        auto scrollAction = MoveTo::create(_creditsDuration, Vec2(_scrollNode->getPositionX(), targetY));
        auto easeAction = EaseInOut::create(scrollAction, 2.0f); // 缓动效果使滚动更自然

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
        // 切换回主菜单场景
        auto menuScene = MainMenu::createScene(); // 请确保这个类存在
        Director::getInstance()->replaceScene(TransitionFade::create(2.0f, menuScene, Color3B(0, 0, 0)));
    }

} // namespace MyGame
