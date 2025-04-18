#include "Shop.h"
#include "Map.h"
#include "cocos2d.h"

USING_NS_CC;

namespace MyGame {

    Scene* ShopScene::createScene() {
        return ShopScene::create();
    }

    bool ShopScene::init() {
        if (!Scene::init()) {
            return false;
        }

        auto director = Director::getInstance();
        auto winSize = director->getWinSize();

        // 确保初始化类成员变量
        _hoveredCard = nullptr;
        _mouseListener = nullptr;

        // 背景设置 - 增加容错处理
        auto background = Sprite::create("store_background.jpg");
        if (background) {
            background->setPosition(winSize / 2);
            background->setScale(std::max(
                winSize.width / background->getContentSize().width,
                winSize.height / background->getContentSize().height
            ));
            this->addChild(background);
        }
        else {
            log("Background image missing! Using a default color background.");
            // 使用颜色层作为备选背景
            auto colorBg = LayerColor::create(Color4B(32, 32, 64, 255), winSize.width, winSize.height);
            this->addChild(colorBg);
        }

        // 添加商人（可点击）
        auto merchant = Sprite::create("shopman.jpg");
        if (merchant) {
            merchant->setPosition(Vec2(winSize.width * 0.9f, winSize.height * 0.35f));
            merchant->setName("Merchant"); // 给节点命名
            this->addChild(merchant, 1);   // 确保在背景上层

            // 启用触摸事件
            auto listener = EventListenerTouchOneByOne::create();
            listener->setSwallowTouches(true);

            listener->onTouchBegan = [=](Touch* touch, Event* event) {
                // 检测是否点击到商人
                Vec2 touchLocation = touch->getLocation();
                Rect merchantRect = merchant->getBoundingBox();
                if (merchantRect.containsPoint(touchLocation)) {
                    this->showGoodsColumn(); // 调用显示商品界面
                    return true; // 吞噬事件
                }
                return false;
                };
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, merchant);
            // 在正常商人显示时
            auto hintLabel = Label::createWithTTF("Click the shopman\nto see the products", "fonts/Marker Felt.ttf", 100);
            hintLabel->setColor(Color3B(255, 215, 0)); // 金色
            hintLabel->setAlignment(TextHAlignment::CENTER); // 文本居中对齐
            hintLabel->setPosition(Vec2(winSize.width * 0.5f, // 放在屏幕中央
                winSize.height * 0.7f));
            hintLabel->setName("HintLabel");
            this->addChild(hintLabel, 2);

        }
        else {
            log("Merchant image missing!");
            // 添加替代的商人图标
            auto merchantPlaceholder = DrawNode::create();
            merchantPlaceholder->drawSolidCircle(Vec2::ZERO, 50, 0, 50, Color4F(0.8f, 0.8f, 0.2f, 1.0f));
            merchantPlaceholder->setPosition(Vec2(winSize.width * 0.7f, winSize.height * 0.35f));
            merchantPlaceholder->setName("Merchant");
            this->addChild(merchantPlaceholder, 1);

            // 启用触摸事件
            auto listener = EventListenerTouchOneByOne::create();
            listener->setSwallowTouches(true);

            listener->onTouchBegan = [=](Touch* touch, Event* event) {
                Vec2 touchLocation = touch->getLocation();
                Rect merchantRect = merchantPlaceholder->getBoundingBox();
                if (merchantRect.containsPoint(touchLocation)) {
                    this->showGoodsColumn();
                    return true;
                }
                return false;
                };
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, merchantPlaceholder);

            // 在备用商人显示时
            auto hintLabel = Label::createWithTTF("Click the shopman\nto see the products", "fonts/Marker Felt.ttf", 50);
            hintLabel->setColor(Color3B(255, 215, 0)); // 金色
            hintLabel->setAlignment(TextHAlignment::CENTER); // 文本居中对齐
            hintLabel->setPosition(Vec2(winSize.width * 0.5f, // 放在屏幕中央
                winSize.height * 0.7f));
            hintLabel->setName("HintLabel");
            this->addChild(hintLabel, 2);

        }

        // 创建"返回"按钮 - 修复类型不匹配问题
        // 使用基类指针来接收不同类型的菜单项
        MenuItem* returnItem = nullptr;

        // 尝试使用图片按钮
        auto imageButton = MenuItemImage::create(
            "back.jpg",
            "return_selected.png",
            [](Ref* sender) {
                // 返回地图场景
                auto scene = Map::createScene();
                Director::getInstance()->replaceScene(scene);
            }
        );

        if (imageButton) {
            returnItem = imageButton;
        }
        else {
            // 图片加载失败，使用文本按钮
            log("Return button images missing! Using text button instead.");
            auto returnLabel = Label::createWithTTF("返回", "fonts/Marker Felt.ttf", 32);
            returnItem = MenuItemLabel::create(returnLabel, [](Ref* sender) {
                auto scene = Map::createScene();
                Director::getInstance()->replaceScene(scene);
                });
        }

        if (returnItem) {
            returnItem->setPosition(Vec2(winSize.width - 50, 50));
            returnItem->setScale(0.5f);  // 调整按钮大小

            auto menu = Menu::create(returnItem, nullptr);
            menu->setPosition(Vec2::ZERO);
            this->addChild(menu, 10);
        }

        // 添加场景标题
        auto titleLabel = Label::createWithTTF("Store", "fonts/Marker Felt.ttf", 150);
        titleLabel->setColor(Color3B(0, 0, 0));
        titleLabel->setPosition(Vec2(winSize.width / 2, winSize.height - 50));
        this->addChild(titleLabel, 5);

        return true;
    }

    void ShopScene::showGoodsColumn() {
        auto winSize = Director::getInstance()->getWinSize();

        // 隐藏提示文本
        auto hintLabel = this->getChildByName("HintLabel");
        if (hintLabel) {
            hintLabel->setVisible(false);
        }

        // 移除可能已存在的商品界面
        this->removeChildByName("GoodsColumn");

        // 创建商品界面
        Node* goods = nullptr;
        auto goodsSprite = Sprite::create("column.jpg");

        if (goodsSprite) {
            goodsSprite->setName("GoodsColumn");
            // 将商品界面向左移动，以露出商人
            goodsSprite->setPosition(Vec2(winSize.width * 0.4f, winSize.height / 2));
            goodsSprite->setScale(3.0f);
            this->addChild(goodsSprite, 999);
            goods = goodsSprite;
        }
        else {
            log("GoodsColumn image missing! Creating a default one.");
            // 创建一个默认的商品列表背景
            auto goodsNode = DrawNode::create();
            goodsNode->setName("GoodsColumn");
            goodsNode->drawSolidRect(
                Vec2(-winSize.width * 0.4f, -winSize.height * 0.4f),
                Vec2(winSize.width * 0.4f, winSize.height * 0.4f),
                Color4F(0.2f, 0.2f, 0.3f, 0.9f)
            );
            // 绘制边框
            goodsNode->drawRect(
                Vec2(-winSize.width * 0.4f, -winSize.height * 0.4f),
                Vec2(winSize.width * 0.4f, winSize.height * 0.4f),
                Color4F(0.8f, 0.7f, 0.3f, 1.0f)
            );
            // 将商品界面向左移动，以露出商人
            goodsNode->setPosition(Vec2(winSize.width * 0.3f, winSize.height / 2));
            this->addChild(goodsNode, 999);
            goods = goodsNode;

            // 添加标题
            auto titleLabel = Label::createWithTTF("Goods", "fonts/Marker Felt.ttf", 50);
            titleLabel->setColor(Color3B(0, 0, 0));
            // 也需要调整标题位置
            titleLabel->setPosition(Vec2(winSize.width * 0.4f, winSize.height / 2 + winSize.height * 0.35f));
            this->addChild(titleLabel, 1000);
        }


        // 清空旧卡牌数据
        _cards.clear();

        // 卡牌布局参数
        const int ROWS = 2;
        const int COLS_PER_ROW[ROWS] = { 6, 6 }; // 每行列数
        const float CARD_SCALE = 0.2f;
        const float HOVER_SCALE = 0.25f;
        const float MARGIN_X = 50.0f; // 横向间距
        const float MARGIN_Y = 50.0f; // 纵向间距
        const float PADDING_LEFT = 40.0f;
        const float PADDING_BOTTOM = 60.0f;

        Size goodsSize;
        if (auto spriteGoods = dynamic_cast<Sprite*>(goods)) {
            goodsSize = spriteGoods->getContentSize() * goods->getScale();
        }
        else {
            // 如果是DrawNode，使用预设的尺寸
            goodsSize = Size(winSize.width * 0.8f, winSize.height * 0.8f);
        }
        Vec2 goodsCenter = goods->getPosition();

        // 计算起始点（左下角 + 内边距）
        Vec2 startPos(
            goodsCenter.x - goodsSize.width / 2.45 + PADDING_LEFT,
            goodsCenter.y - goodsSize.height / 3.2 + PADDING_BOTTOM
        );

        // 获取卡牌模板尺寸
        auto cardTemplate = Sprite::create("card_front.png");
        float cardWidth, cardHeight;
        if (cardTemplate) {
            cardWidth = cardTemplate->getContentSize().width * CARD_SCALE;
            cardHeight = cardTemplate->getContentSize().height * CARD_SCALE;
        }
        else {
            log("Card template image missing! Using default size.");
            cardWidth = 100.0f * CARD_SCALE;  // 默认宽度
            cardHeight = 150.0f * CARD_SCALE; // 默认高度
        }

        // 存储第一行前两张卡牌的位置
        Vec2 firstRowFirstCardPos;
        Vec2 firstRowSecondCardPos;

        // 创建卡牌
        int totalIndex = 0; // 全局索引
        for (int row = 0; row < ROWS; ++row) {
            int cols = COLS_PER_ROW[row]; // 当前行列数

            // 计算当前行的起始X偏移（仅第二行需要对齐前两张）
            float rowStartX = startPos.x;
            if (row == 1 && !_cards.empty()) {
                firstRowFirstCardPos = _cards[0]->getPosition(); // 第一张卡的位置
                if (_cards.size() > 1) {
                    firstRowSecondCardPos = _cards[1]->getPosition(); // 第二张卡的位置
                }
                else {
                    // 如果只有一张卡，计算第二张的位置
                    firstRowSecondCardPos = Vec2(
                        firstRowFirstCardPos.x + cardWidth + MARGIN_X,
                        firstRowFirstCardPos.y
                    );
                }
                rowStartX = firstRowFirstCardPos.x; // 复用第一行第一张的X位置
            }

            for (int col = 0; col < cols; ++col) {
                auto card = Sprite::create("card_front.png");
                if (!card) {
                    // 如果卡片图片加载失败，创建一个替代图形
                    card = Sprite::create();
                    auto cardNode = DrawNode::create();
                    cardNode->drawSolidRect(
                        Vec2(-cardWidth / 2, -cardHeight / 2),
                        Vec2(cardWidth / 2, cardHeight / 2),
                        Color4F(0.5f, 0.5f, 0.7f, 1.0f)
                    );
                    cardNode->drawRect(
                        Vec2(-cardWidth / 2, -cardHeight / 2),
                        Vec2(cardWidth / 2, cardHeight / 2),
                        Color4F(0.9f, 0.9f, 1.0f, 1.0f)
                    );
                    card->addChild(cardNode);
                }
                else {
                    card->setScale(CARD_SCALE);
                }

                // 第一行：正常计算位置
                if (row == 0) {
                    float posX = startPos.x + col * (cardWidth + MARGIN_X);
                    float posY = startPos.y + (ROWS - 1 - row) * (cardHeight + MARGIN_Y);
                    card->setPosition(Vec2(posX, posY));

                    // 记录前两张卡牌位置
                    if (col == 0) firstRowFirstCardPos = card->getPosition();
                    if (col == 1) firstRowSecondCardPos = card->getPosition();
                }
                // 第二行：前两张对齐，第三张独立
                else if (row == 1) {
                    float posX;
                    if (col < 2) {
                        posX = (col == 0) ? firstRowFirstCardPos.x : firstRowSecondCardPos.x;
                    }
                    else {
                        posX = firstRowSecondCardPos.x + (col - 1) * (cardWidth + MARGIN_X);
                    }
                    float posY = firstRowFirstCardPos.y - (cardHeight + MARGIN_Y);
                    card->setPosition(Vec2(posX, posY));
                }

                // 添加卡片名称
                auto cardName = Label::createWithTTF("Card #" + std::to_string(totalIndex + 1),
                    "fonts/Marker Felt.ttf", 12);
                cardName->setPosition(Vec2(0, -cardHeight / 2 - 10));
                card->addChild(cardName);

                this->addChild(card, goods->getLocalZOrder() + 1);
                _cards.push_back(card); // 将卡牌指针存入容器
                totalIndex++;
            }
        }

        // 添加鼠标事件监听
        if (_mouseListener) {
            _eventDispatcher->removeEventListener(_mouseListener);
            _mouseListener = nullptr;
        }

        _mouseListener = EventListenerMouse::create();
        _mouseListener->onMouseMove = [this, CARD_SCALE, HOVER_SCALE](EventMouse* event) {
            Vec2 mousePos = event->getLocationInView();

            // 遍历所有卡牌检测悬停
            for (auto& card : _cards) {
                Rect rect = card->getBoundingBox();
                if (rect.containsPoint(mousePos)) {
                    // 进入卡牌区域
                    if (_hoveredCard != card) {
                        // 恢复上一个悬停卡牌
                        if (_hoveredCard) {
                            _hoveredCard->stopAllActions();
                            _hoveredCard->runAction(ScaleTo::create(0.1f, CARD_SCALE));
                        }
                        // 放大当前卡牌
                        card->stopAllActions();
                        card->runAction(ScaleTo::create(0.1f, HOVER_SCALE));
                        _hoveredCard = card;
                    }
                    return;
                }
            }

            // 没有悬停卡牌时恢复
            if (_hoveredCard) {
                _hoveredCard->stopAllActions();
                _hoveredCard->runAction(ScaleTo::create(0.1f, CARD_SCALE));
                _hoveredCard = nullptr;
            }
            };

        _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

        // 添加关闭按钮 - 使用MenuItem基类，避免类型转换问题
        MenuItem* closeButton = nullptr;
        auto closeLabel = Label::createWithTTF("关闭", "fonts/Marker Felt.ttf", 24);
        closeButton = MenuItemLabel::create(closeLabel, [this](Ref* sender) {
            this->removeChildByName("GoodsColumn");
            if (_mouseListener) {
                _eventDispatcher->removeEventListener(_mouseListener);
                _mouseListener = nullptr;
            }
            });

        if (closeButton) {
            closeButton->setPosition(Vec2(goodsCenter.x, 0));

            auto menu = Menu::create(closeButton, nullptr);
            menu->setPosition(Vec2::ZERO);
            this->addChild(menu, 1000);
        }
    }

} // namespace MyGame
