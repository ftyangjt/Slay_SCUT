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

        // 背景设置
        auto background = Sprite::create("The_Merchant_in-game.png");
        if (!background) {
            log("Background image missing!");
            return false;
        }
        background->setPosition(winSize / 2);
        background->setScale(std::max(
            winSize.width / background->getContentSize().width,
            winSize.height / background->getContentSize().height
        ));
        this->addChild(background);

        // 添加商人（可点击）
        auto merchant = Sprite::create("Merchant.jpg");
        if (merchant) {
            merchant->setPosition(Vec2(winSize.width * 0.7f, winSize.height * 0.35f));
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
        }
        else {
            log("Merchant image missing!");
        }

        // 创建"返回"按钮
        auto returnItem = MenuItemImage::create(
            "return.png",  // 返回按钮图片
            "return_selected.png",  // 选中状态的图片
            [](Ref* sender) {
                // 返回地图场景
                auto scene = Map::createScene();
                Director::getInstance()->replaceScene(scene);
            }
        );
        if (returnItem)
        {
            returnItem->setPosition(Vec2(winSize.width - 50, 50));
            returnItem->setScale(0.5f);  // 调整按钮大小

            auto menu = Menu::create(returnItem, nullptr);
            menu->setPosition(Vec2::ZERO);
            this->addChild(menu, 10);
        }

        return true;
    }

    void ShopScene::showGoodsColumn() {
        auto winSize = Director::getInstance()->getWinSize();

        // 创建商品界面
        auto goods = Sprite::create("GoodsColumn.jpg");
        if (goods) {
            goods->setName("GoodsColumn"); // 设置节点名称
            goods->setPosition(winSize / 2);
            goods->setScale(1.2f);
            this->addChild(goods, 999);

            // 清空旧卡牌数据
            _cards.clear();

            // 卡牌布局参数
            const int ROWS = 2;
            const int COLS_PER_ROW[ROWS] = { 5, 3 }; // 每行列数
            const float CARD_SCALE = 0.2f;
            const float HOVER_SCALE = 0.25f;
            const float MARGIN_X = 25.0f; // 横向间距
            const float MARGIN_Y = 50.0f; // 纵向间距
            const float PADDING_LEFT = 40.0f;
            const float PADDING_BOTTOM = 60.0f;

            Size goodsSize = goods->getContentSize() * goods->getScale();
            Vec2 goodsCenter = goods->getPosition();

            // 计算起始点（左下角 + 内边距）
            Vec2 startPos(
                goodsCenter.x - goodsSize.width / 2.45 + PADDING_LEFT,
                goodsCenter.y - goodsSize.height / 3.2 + PADDING_BOTTOM
            );

            // 获取卡牌模板尺寸
            auto cardTemplate = Sprite::create("card_front.png");
            float cardWidth = cardTemplate->getContentSize().width * CARD_SCALE;
            float cardHeight = cardTemplate->getContentSize().height * CARD_SCALE;

            // 存储第一行前两张卡牌的位置
            Vec2 firstRowFirstCardPos;
            Vec2 firstRowSecondCardPos;

            // 创建卡牌
            int totalIndex = 0; // 全局索引
            for (int row = 0; row < ROWS; ++row) {
                int cols = COLS_PER_ROW[row]; // 当前行列数

                // 计算当前行的起始X偏移（仅第二行需要对齐前两张）
                float rowStartX = startPos.x;
                if (row == 1) {
                    rowStartX = firstRowFirstCardPos.x; // 复用第一行第一张的X位置
                }

                for (int col = 0; col < cols; ++col) {
                    auto card = Sprite::create("card_front.png");
                    if (card) {
                        card->setScale(CARD_SCALE);

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

                        this->addChild(card, goods->getLocalZOrder() + 1);
                        _cards.push_back(card); // 将卡牌指针存入容器
                        totalIndex++;
                    }
                }
            }

            // 添加鼠标事件监听
            if (!_mouseListener) {
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
            }
        }
        else {
            log("GoodsColumn image missing!");
        }
    }

} // namespace MyGame
