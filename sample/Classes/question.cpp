#include "question.h"
#include "Map.h"  // 引入地图场景头文件
#include "FightingScene.h" // 引入战斗场景头文件
#include "Shop.h" // 引入商店场景头文件
#include "Hero.h" // 引入英雄类头文件
#include <random>

USING_NS_CC;

Scene* Question::createScene()
{
    return Question::create();
}

bool Question::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // 初始化成员变量
    _isWheelSpinning = false;

    // 添加问号场景的背景图片
    auto background = Sprite::create("question_background.png");
    if (background)
    {
        background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        this->addChild(background, 0);  // 将背景图片添加到最底层
    }

    // 创建"返回"按钮
    auto returnItem = MenuItemImage::create(
        "back.jpg",  // 正常状态的图片
        "return_selected.png",  // 选中状态的图片
        CC_CALLBACK_1(Question::menuReturnCallback, this)
    );
    if (returnItem)
    {
        returnItem->setPosition(Vec2(
            origin.x + visibleSize.width - returnItem->getContentSize().width / 2 - 10,
            origin.y + returnItem->getContentSize().height / 2 + 10
        ));
        returnItem->setScale(0.5);  // 调整按钮大小
    }

    // 将按钮加入菜单
    auto menu = Menu::create(returnItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);  // 确保菜单在背景图片之上

    // 创建事件描述标签
    _eventLabel = Label::createWithTTF("Welcome to Mystery Room!", "fonts/Marker Felt.ttf", 36);
    _eventLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 50));
    this->addChild(_eventLabel, 2);

    // 进入场景时立即触发随机事件
    this->scheduleOnce([this](float dt) {
        triggerRandomEvent();
        }, 0.1f, "trigger_event");

    return true;
}

void Question::menuReturnCallback(Ref* pSender)
{
    // 切换到地图场景
    auto scene = MyGame::Map::createScene();
    Director::getInstance()->replaceScene(scene);
}

void Question::triggerRandomEvent()
{
    // 创建随机数生成器
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    // 生成0到1之间的随机数
    float randomValue = dis(gen);

    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 根据随机数和概率分配不同事件
    if (randomValue < 0.2) { // 20% 概率进入战斗场景
        _eventLabel->setString("Fighting now!");

        // 3秒后进入战斗场景
        this->scheduleOnce([](float dt) {
            auto scene = FightingScene::createScene();
            Director::getInstance()->replaceScene(scene);
            }, 1.0f, "goto_battle");
    }
    else if (randomValue < 0.3) { // 10% 概率进入商店
        _eventLabel->setString("You find a shop");

        // 3秒后进入商店场景
        this->scheduleOnce([](float dt) {
            auto scene = MyGame::ShopScene::createScene();
            Director::getInstance()->replaceScene(scene);
            }, 1.0f, "goto_shop");
    }
    else if (randomValue < 0.4) { // 10% 概率回血最多30
        // 获取当前和最大血量
        int currentHealth = Hero::getCurrentHealth();
        int maxHealth = Hero::getMaxHealth();

        // 计算可以回复的血量（不超过30且不超过最大血量）
        int healAmount = std::min(30, maxHealth - currentHealth);

        // 回复血量
        Hero::healHealth(healAmount);

        _eventLabel->setString("You recover " + std::to_string(healAmount) + " hp");
    }
    else if (randomValue < 0.6) { // 20% 概率获得50金币
        // 增加金币
        Hero::addCoins(50);

        _eventLabel->setString("You gain 50 coins");
    }
    else { // 40% 概率出现抽奖转盘
        _eventLabel->setString("Try your luck with the wheel of fortune!");

        // 创建抽奖转盘
        createLuckyWheel();
    }

    // 更新英雄状态显示
    Hero::updateStatusDisplayStatic();

    // 创建一个动画效果，使标签先放大后恢复
    _eventLabel->runAction(
        Sequence::create(
            ScaleTo::create(0.2f, 1.2f),
            ScaleTo::create(0.2f, 1.0f),
            nullptr
        )
    );
}

void Question::createLuckyWheel()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // 创建转盘背景 (如果没有wheel.png，可以使用其他圆形图像或者自己创建一个DrawNode作为转盘)
    _wheelSprite = Sprite::create("wheel.png");

    // 如果没有转盘图片，创建一个临时的圆形转盘
    if (!_wheelSprite)
    {
        // 创建一个圆形的DrawNode作为转盘
        auto drawNode = DrawNode::create();

        // 转盘半径
        float radius = 150.0f;

        // 画一个填充的圆作为背景
        drawNode->drawSolidCircle(Vec2::ZERO, radius, 0, 60, Color4F(0.8f, 0.8f, 0.8f, 1.0f));

        // 创建红色扇形区域（-20 HP）
        auto redDrawNode = DrawNode::create();
        for (int angle = 0; angle < 120; angle++) {
            float radians1 = CC_DEGREES_TO_RADIANS(angle);
            float radians2 = CC_DEGREES_TO_RADIANS(angle + 1);
            Vec2 v1(std::cos(radians1) * radius, std::sin(radians1) * radius);
            Vec2 v2(std::cos(radians2) * radius, std::sin(radians2) * radius);
            redDrawNode->drawTriangle(Vec2::ZERO, v1, v2, Color4F(1.0f, 0.0f, 0.0f, 1.0f));
        }
        redDrawNode->setPosition(Vec2::ZERO);
        drawNode->addChild(redDrawNode);

        // 创建绿色扇形区域（+20 HP）
        auto greenDrawNode = DrawNode::create();
        for (int angle = 120; angle < 240; angle++) {
            float radians1 = CC_DEGREES_TO_RADIANS(angle);
            float radians2 = CC_DEGREES_TO_RADIANS(angle + 1);
            Vec2 v1(std::cos(radians1) * radius, std::sin(radians1) * radius);
            Vec2 v2(std::cos(radians2) * radius, std::sin(radians2) * radius);
            greenDrawNode->drawTriangle(Vec2::ZERO, v1, v2, Color4F(0.0f, 1.0f, 0.0f, 1.0f));
        }
        greenDrawNode->setPosition(Vec2::ZERO);
        drawNode->addChild(greenDrawNode);

        // 创建黄色扇形区域（+40 Coins）
        auto yellowDrawNode = DrawNode::create();
        for (int angle = 240; angle < 360; angle++) {
            float radians1 = CC_DEGREES_TO_RADIANS(angle);
            float radians2 = CC_DEGREES_TO_RADIANS(angle + 1);
            Vec2 v1(std::cos(radians1) * radius, std::sin(radians1) * radius);
            Vec2 v2(std::cos(radians2) * radius, std::sin(radians2) * radius);
            yellowDrawNode->drawTriangle(Vec2::ZERO, v1, v2, Color4F(1.0f, 1.0f, 0.0f, 1.0f));
        }
        yellowDrawNode->setPosition(Vec2::ZERO);
        drawNode->addChild(yellowDrawNode);

        // 创建一个临时精灵包含DrawNode
        _wheelSprite = Sprite::create();
        auto rt = RenderTexture::create(radius * 2, radius * 2);
        rt->beginWithClear(0, 0, 0, 0);

        // 将drawNode添加到渲染纹理中
        drawNode->setPosition(Vec2(radius, radius));

        // 获取渲染器和当前的变换矩阵
        auto renderer = Director::getInstance()->getRenderer();
        auto& transform = Director::getInstance()->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

        // 正确调用visit函数，传递渲染器和变换矩阵
        drawNode->visit(renderer, transform, 0);

        rt->end();

        // 设置精灵的纹理
        _wheelSprite->setTexture(rt->getSprite()->getTexture());
        _wheelSprite->setTextureRect(Rect(0, 0, radius * 2, radius * 2));
    }

    // 设置转盘位置 - 放在右侧
    float wheelX = visibleSize.width / 2 + 150;
    _wheelSprite->setPosition(Vec2(wheelX, visibleSize.height / 2));
    this->addChild(_wheelSprite, 1);

    // 移除转盘上的文字标签，改为在场景中直接添加静态文字说明

    // 创建左侧的奖励说明文本框
    auto descriptionBox = LayerColor::create(Color4B(50, 50, 50, 180), 250, 300);
    descriptionBox->setPosition(Vec2(wheelX - 450, visibleSize.height / 2 - 150));
    this->addChild(descriptionBox, 1);

    // 添加标题
    auto titleLabel = Label::createWithTTF("Exlaination", "fonts/Marker Felt.ttf", 32);
    titleLabel->setPosition(Vec2(125, 270));
    titleLabel->setColor(Color3B::WHITE);
    descriptionBox->addChild(titleLabel);

    // 添加奖励描述
    auto redDescription = Label::createWithTTF("Red: +20 HP", "fonts/Marker Felt.ttf", 24);
    redDescription->setPosition(Vec2(125, 200));
    redDescription->setColor(Color3B::RED);
    descriptionBox->addChild(redDescription);

    auto greenDescription = Label::createWithTTF("Green: -20 HP", "fonts/Marker Felt.ttf", 24);
    greenDescription->setPosition(Vec2(125, 150));
    greenDescription->setColor(Color3B::GREEN);
    descriptionBox->addChild(greenDescription);

    auto yellowDescription = Label::createWithTTF("Yellow: +40 coins", "fonts/Marker Felt.ttf", 24);
    yellowDescription->setPosition(Vec2(125, 100));
    yellowDescription->setColor(Color3B::YELLOW);
    descriptionBox->addChild(yellowDescription);

    // 添加使用说明
    auto instructionLabel = Label::createWithTTF("click the button to start", "fonts/Marker Felt.ttf", 22);
    instructionLabel->setPosition(Vec2(125, 40));
    instructionLabel->setColor(Color3B::WHITE);
    descriptionBox->addChild(instructionLabel);

    // 创建指针
    _pointerSprite = Sprite::create("pointer.png");
    if (!_pointerSprite)
    {
        // 如果没有指针图片，创建一个临时的三角形指针
        auto drawNode = DrawNode::create();
        Vec2 points[3] = { Vec2(0, 30), Vec2(-15, 0), Vec2(15, 0) };
        drawNode->drawSolidPoly(points, 3, Color4F(1.0f, 0.5f, 0.0f, 1.0f));

        // 创建一个临时精灵包含DrawNode
        _pointerSprite = Sprite::create();
        auto rt = RenderTexture::create(30, 30);
        rt->beginWithClear(0, 0, 0, 0);

        // 获取渲染器和当前的变换矩阵
        auto renderer = Director::getInstance()->getRenderer();
        auto& transform = Director::getInstance()->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

        drawNode->setPosition(Vec2(15, 15));
        // 正确调用visit函数，传递渲染器和变换矩阵
        drawNode->visit(renderer, transform, 0);

        rt->end();
        _pointerSprite->setTexture(rt->getSprite()->getTexture());
        _pointerSprite->setTextureRect(Rect(0, 0, 30, 30));
    }

    // 设置指针位置
    _pointerSprite->setPosition(Vec2(wheelX, visibleSize.height / 2 + 170));
    this->addChild(_pointerSprite, 2);

    // 创建抽奖按钮
    _spinButton = MenuItemImage::create(
        "button.png",   // 正常状态的图片
        "button_selected.png",  // 选中状态的图片
        CC_CALLBACK_1(Question::onSpinButtonClicked, this)
    );

    if (!_spinButton)
    {
        // 如果没有按钮图片，创建一个临时按钮
        _spinButton = MenuItemImage::create();
        auto drawNode = DrawNode::create();
        drawNode->drawSolidRect(Vec2(-60, -20), Vec2(60, 20), Color4F(0.0f, 0.5f, 1.0f, 1.0f));

        auto rt = RenderTexture::create(120, 40);
        rt->beginWithClear(0, 0, 0, 0);

        // 获取渲染器和当前的变换矩阵
        auto renderer = Director::getInstance()->getRenderer();
        auto& transform = Director::getInstance()->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

        drawNode->setPosition(Vec2(60, 20));
        // 正确调用visit函数，传递渲染器和变换矩阵
        drawNode->visit(renderer, transform, 0);

        rt->end();

        _spinButton->setNormalImage(rt->getSprite());
        _spinButton->setCallback(CC_CALLBACK_1(Question::onSpinButtonClicked, this));
    }

    // 在按钮上添加文字
    auto spinButtonLabel = Label::createWithTTF("start to lottery", "fonts/Marker Felt.ttf", 60);
    spinButtonLabel->setPosition(Vec2(_spinButton->getContentSize().width / 2, _spinButton->getContentSize().height / 2));
    _spinButton->addChild(spinButtonLabel);

    // 设置按钮位置 - 放在转盘下方
    _spinButton->setPosition(Vec2(wheelX, visibleSize.height / 2 - 300));

    // 创建菜单并添加按钮
    auto menu = Menu::create(_spinButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 2);

    // 设置按钮大小
    _spinButton->setScale(0.3); // 放大按钮
}


void Question::onSpinButtonClicked(Ref* pSender)
{
    if (_isWheelSpinning)
        return;

    _isWheelSpinning = true;

    // 禁用抽奖按钮
    _spinButton->setEnabled(false);

    // 生成随机结果
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> wheelDis(0, 2);
    int wheelResult = wheelDis(gen);

    // 执行转盘旋转动画
    spinWheel(wheelResult);
}

void Question::spinWheel(int result)
{
    // 计算最终旋转角度
    // 根据结果设置最终角度：0=失去生命(-20HP), 1=获得生命(+20HP), 2=获得金币(+40币)
    float finalAngle = 0;
    switch (result) {
    case 0: finalAngle = 60.0f; break;  // 指向红色区域(失去生命)
    case 1: finalAngle = 180.0f; break; // 指向绿色区域(获得生命)
    case 2: finalAngle = 300.0f; break; // 指向黄色区域(获得金币)
    }

    // 添加一些随机旋转圈数，使旋转更自然
    int numRotations = 5 + (std::rand() % 3); // 5-7圈
    float totalAngle = 360.0f * numRotations + finalAngle;

    // 执行旋转动画
    auto rotateAction = RotateBy::create(3.0f, totalAngle);
    auto easeAction = EaseOut::create(rotateAction, 2.0f); // 使用减速效果

    // 动画结束后显示结果
    auto showResultCallback = CallFunc::create([this, result]() {
        _isWheelSpinning = false;
        showWheelResult(result);
        });

    // 执行动画序列
    _wheelSprite->runAction(Sequence::create(easeAction, showResultCallback, nullptr));
}

void Question::showWheelResult(int result)
{
    switch (result) {
    case 0: { // 失去20生命
        int currentHealth = Hero::getCurrentHealth();
        // 确保生命值不会低于1
        int loseAmount = std::min(currentHealth - 1, 20);
        Hero::healHealth(-loseAmount); // 使用负数表示扣除生命值
        _eventLabel->setString("Bad luck! You lose " + std::to_string(loseAmount) + " health");
        _eventLabel->setTextColor(Color4B::RED);
        break;
    }
    case 1: { // 获得20生命
        int currentHealth = Hero::getCurrentHealth();
        int maxHealth = Hero::getMaxHealth();
        int healAmount = std::min(20, maxHealth - currentHealth);
        Hero::healHealth(healAmount);
        _eventLabel->setString("Good luck! You gain " + std::to_string(healAmount) + " health");
        _eventLabel->setTextColor(Color4B::GREEN);
        break;
    }
    case 2: { // 获得40金币
        Hero::addCoins(40);
        _eventLabel->setString("Jackpot! You gain 40 coins");
        _eventLabel->setTextColor(Color4B::YELLOW);
        break;
    }
    }

    // 创建一个动画效果，使标签先放大后恢复
    _eventLabel->runAction(
        Sequence::create(
            ScaleTo::create(0.2f, 1.2f),
            ScaleTo::create(0.2f, 1.0f),
            nullptr
        )
    );

    // 3秒后返回地图
    this->scheduleOnce([](float dt) {
        auto scene = MyGame::Map::createScene();
        Director::getInstance()->replaceScene(scene);
        }, 3.0f, "goto_map");
}
