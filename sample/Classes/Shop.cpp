// ../Classes/Shop.cpp 的修改实现

#include "Shop.h"
#include "Map.h"
#include "Hero.h"
#include "cocos2d.h"
#include "CardLibrary.h"

USING_NS_CC;

namespace MyGame {

    // 存储商品信息的结构体
    struct ShopItem {
        std::string name;      // 商品名称
        int price;             // 商品价格
        std::string image;     // 商品图片
        std::string description; // 商品描述
        std::function<void()> effect; // 购买后的效果
        bool purchased = false; // 记录商品是否已被购买
        float scale = 0.25f;    // 商品图标的缩放比例，默认为0.5
    };

    // 用于存储商品索引和原始缩放比例的结构 - 移到命名空间级别
    struct ItemData {
        int index;
        float scale;
    };

    // 修改 Shop.cpp 中的静态商品定义部分，添加三张从卡牌库随机选取的卡牌
// 在 shopItems 定义处添加以下内容

    // 定义商店商品
    static std::vector<ShopItem> shopItems = {
            {"Life potion", 20, "life_potion.png", "Restore 20 health points", []() { Hero::healHealth(20); }, false, 0.30f},
            // 修改 Shop.cpp 中的商品定义部分，替换神秘草莓的效果函数
            {"Mysterious strawberry", 100, "strawberry.jpg", "Increase maximum health by 10", []() {
                Hero::increaseMaxHealth(10);

                // 显示效果信息
                auto director = Director::getInstance();
                auto msg = Label::createWithTTF("Max Health increased by 10!",
                    "fonts/Marker Felt.ttf", 30);
                msg->setColor(Color3B(255, 105, 180)); // 粉色

                Size winSize = director->getWinSize();
                msg->setPosition(Vec2(winSize.width / 2, winSize.height / 2 + 250));

                director->getRunningScene()->addChild(msg, 2001);

                // 2秒后淡出提示
                auto fadeOut = FadeOut::create(2.0f);
                auto remove = RemoveSelf::create();
                msg->runAction(Sequence::create(fadeOut, remove, nullptr));
            }, false, 0.65f},
            {"Coin bag", 50, "coin_bag.png", "randomly gain 0-100 coins", []() {
            // 使用cocos2d-x的随机数函数
            int randomCoins = cocos2d::random(0, 100);
            Hero::addCoins(randomCoins);

            // 显示获得的金币数量
            auto director = Director::getInstance();
            auto coinMsg = Label::createWithTTF(StringUtils::format("Gain %d coins!", randomCoins),
                "fonts/Marker Felt.ttf", 60);
            coinMsg->setColor(Color3B(255, 215, 0)); // 金色

            // 修正位置计算方式
            Size winSize = director->getWinSize();
            coinMsg->setPosition(Vec2(winSize.width / 2, winSize.height / 2 + 50));

            director->getRunningScene()->addChild(coinMsg, 2001);

            // 2秒后淡出提示
            auto fadeOut = FadeOut::create(2.0f);
            auto remove = RemoveSelf::create();
            coinMsg->runAction(Sequence::create(fadeOut, remove, nullptr));
        }, false, 0.250f},
        // 添加三张随机卡牌，价格统一为80
        {"Random Card 1", 80, "cardBackground.jpg", "A random card from the library", []() {
            // 获取一张随机非初始非诅咒卡牌
            Card randomCard = CardLibrary::getRandomNonInitialNonCurseCard();
            std::vector<Card> currentDeck = Hero::getDeck();
            // 将卡牌添加到英雄牌库
            Hero::addCardToDeckStatic(randomCard);

            // 显示获得卡牌的信息
            auto director = Director::getInstance();
            auto cardMsg = Label::createWithTTF(StringUtils::format("Added %s to your deck!",
                randomCard.getName().c_str()),
                "fonts/Marker Felt.ttf", 50);
            cardMsg->setColor(Color3B(100, 200, 255)); // 淡蓝色

            Size winSize = director->getWinSize();
            cardMsg->setPosition(Vec2(winSize.width / 2, winSize.height / 2 + 50));

            director->getRunningScene()->addChild(cardMsg, 2001);

            // 2秒后淡出提示
            auto fadeOut = FadeOut::create(2.0f);
            auto remove = RemoveSelf::create();
            cardMsg->runAction(Sequence::create(fadeOut, remove, nullptr));
        }, false, 0.40f},
        {"Random Card 2", 80, "cardBackground.jpg", "A random card from the library", []() {
            // 获取一张随机非初始非诅咒卡牌
            Card randomCard = CardLibrary::getRandomNonInitialNonCurseCard();
            std::vector<Card> currentDeck = Hero::getDeck();
            // 将卡牌添加到英雄牌库
            Hero::addCardToDeckStatic(randomCard);

            // 显示获得卡牌的信息
            auto director = Director::getInstance();
            auto cardMsg = Label::createWithTTF(StringUtils::format("Added %s to your deck!",
                randomCard.getName().c_str()),
                "fonts/Marker Felt.ttf", 50);
            cardMsg->setColor(Color3B(100, 200, 255)); // 淡蓝色

            Size winSize = director->getWinSize();
            cardMsg->setPosition(Vec2(winSize.width / 2, winSize.height / 2 + 50));

            director->getRunningScene()->addChild(cardMsg, 2001);

            // 2秒后淡出提示
            auto fadeOut = FadeOut::create(2.0f);
            auto remove = RemoveSelf::create();
            cardMsg->runAction(Sequence::create(fadeOut, remove, nullptr));
        }, false, 0.40f},
        {"Random Card 3", 80, "cardBackground.jpg", "A random card from the library", []() {
            // 获取一张随机非初始非诅咒卡牌
            Card randomCard = CardLibrary::getRandomNonInitialNonCurseCard();
            std::vector<Card> currentDeck = Hero::getDeck();
            // 将卡牌添加到英雄牌库
            Hero::addCardToDeckStatic(randomCard);

            // 显示获得卡牌的信息
            auto director = Director::getInstance();
            auto cardMsg = Label::createWithTTF(StringUtils::format("Added %s to your deck!",
                randomCard.getName().c_str()),
                "fonts/Marker Felt.ttf", 50);
            cardMsg->setColor(Color3B(100, 200, 255)); // 淡蓝色

            Size winSize = director->getWinSize();
            cardMsg->setPosition(Vec2(winSize.width / 2, winSize.height / 2 + 50));

            director->getRunningScene()->addChild(cardMsg, 2001);

            // 2秒后淡出提示
            auto fadeOut = FadeOut::create(2.0f);
            auto remove = RemoveSelf::create();
            cardMsg->runAction(Sequence::create(fadeOut, remove, nullptr));
        }, false, 0.40f},
    };


    Scene* ShopScene::createScene() {
        return ShopScene::create();
    }

    bool ShopScene::init() {
        if (!Scene::init()) {
            return false;
        }

        // 重置所有商品的购买状态，使它们在每次重新进入商店时都可以购买
        for (auto& item : shopItems) {
            item.purchased = false;
        }

        // 原有的初始化代码...
        auto director = Director::getInstance();
        auto winSize = director->getWinSize();

        // 确保初始化类成员变量
        _hoveredCard = nullptr;
        _mouseListener = nullptr;

        // 创建两个背景图片
        auto background1 = Sprite::create("store_background1.png");
        auto background2 = Sprite::create("store_background2.png");

        // 检查两个背景图片是否都加载成功
        if (background1 && background2) {
            // 设置第一张背景图片
            background1->setPosition(winSize / 2);
            background1->setScale(std::max(
                winSize.width / background1->getContentSize().width,
                winSize.height / background1->getContentSize().height
            ));
            this->addChild(background1, 0, "Background1");

            // 设置第二张背景图片（初始时不可见）
            background2->setPosition(winSize / 2);
            background2->setScale(std::max(
                winSize.width / background2->getContentSize().width,
                winSize.height / background2->getContentSize().height
            ));
            background2->setOpacity(0); // 设为完全透明
            this->addChild(background2, 0, "Background2");

            // 1秒后切换背景图片
            this->runAction(Sequence::create(
                DelayTime::create(1.0f),
                CallFunc::create([=]() {
                    // 第一张背景淡出
                    background1->runAction(FadeOut::create(0.5f));
                    // 第二张背景淡入
                    background2->runAction(FadeIn::create(0.5f));
                    }),
                nullptr
            ));
        }
        else {
            // 如果至少有一张背景图片加载失败，则尝试使用单张背景图片
            auto background = Sprite::create("store_background.png");
            if (background) {
                background->setPosition(winSize / 2);
                background->setScale(std::max(
                    winSize.width / background->getContentSize().width,
                    winSize.height / background->getContentSize().height
                ));
                this->addChild(background);
            }
            else {
                log("Background images missing! Using a default color background.");
                // 使用颜色层作为备选背景
                auto colorBg = LayerColor::create(Color4B(32, 32, 64, 255), winSize.width, winSize.height);
                this->addChild(colorBg);
            }
        }


        // 添加商人（可点击）
        auto merchant = Sprite::create("shopman.jpg");
        if (merchant) {
            merchant->setPosition(Vec2(winSize.width * 0.9f - 400, winSize.height * 0.35f + 120));
            merchant->setName("Merchant"); // 给节点命名
            this->addChild(merchant, -1);   // 确保在背景上层

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
            auto hintLabel = Label::createWithTTF(" ", "fonts/Marker Felt.ttf", 100);
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
            "back.png",
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
            returnItem->setPosition(Vec2(winSize.width - 150, 50));
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

        // 创建商品界面背景
        Node* goods = nullptr;
        auto goodsSprite = Sprite::create("column.png");
        if (goodsSprite) {
            goodsSprite->setName("GoodsColumn");
            // 放大商品背景以适应商品横向排列
            goodsSprite->setPosition(Vec2(winSize.width * 0.5f, winSize.height / 2));
            goodsSprite->setScale(0.6f); // 增加背景尺寸以容纳横向排列的商品
            this->addChild(goodsSprite, 999);
            goods = goodsSprite;
        }
        else {
            log("GoodsColumn image missing! Creating a default one.");
            // 创建一个默认的商品列表背景
            auto goodsNode = DrawNode::create();
            goodsNode->setName("GoodsColumn");
            goodsNode->setScale(0.25f);
            goodsNode->drawSolidRect(
                Vec2(-winSize.width * 0.45f, -winSize.height * 0.35f),
                Vec2(winSize.width * 0.45f, winSize.height * 0.35f),
                Color4F(0.2f, 0.2f, 0.3f, 0.9f)
            );
            // 绘制边框
            goodsNode->drawRect(
                Vec2(-winSize.width * 0.45f, -winSize.height * 0.35f),
                Vec2(winSize.width * 0.45f, winSize.height * 0.35f),
                Color4F(0.8f, 0.7f, 0.3f, 1.0f)
            );
            // 将商品界面居中
            goodsNode->setPosition(Vec2(winSize.width * 0.5f, winSize.height / 2));
            this->addChild(goodsNode, 999);
            goods = goodsNode;

            // 添加标题
            auto titleLabel = Label::createWithTTF("Goods", "fonts/Marker Felt.ttf", 50);
            titleLabel->setColor(Color3B(0, 0, 0));
            // 调整标题位置到顶部中央
            titleLabel->setPosition(Vec2(winSize.width * 0.5f, winSize.height / 2 + winSize.height * 0.25f));
            this->addChild(titleLabel, 1000);
        }

        // 清空旧卡牌数据
        _cards.clear();

        // 确保悬停卡牌指针置空
        _hoveredCard = nullptr;

        // 卡牌布局参数
        const float HOVER_SCALE_MULTIPLIER = 1.2f; // 悬停时的放大倍数（相对于每个商品自己的缩放比例）
        const int ITEMS_PER_ROW = 3;    // 每行3个商品
        const int ROWS = 2;             // 2行商品
        const float HORIZONTAL_SPACING = 650.0f; // 水平间距增加到650（原来是500）
        const float VERTICAL_SPACING = 450.0f;
        const float START_X = goods->getPosition().x - HORIZONTAL_SPACING; // 起始X坐标
        const float START_Y = goods->getPosition().y + VERTICAL_SPACING * 0.7f; // 起始Y坐标

        // 获取可显示的未购买商品
        std::vector<int> availableItems;
        for (size_t i = 0; i < shopItems.size(); ++i) {
            if (!shopItems[i].purchased) {
                availableItems.push_back(i);
            }
        }

        // 计算有多少个可用的商品
        int itemCount = std::min((int)availableItems.size(), ITEMS_PER_ROW * ROWS);

        // 如果没有可用商品，显示一个消息
        if (itemCount == 0) {
            auto noItemsLabel = Label::createWithTTF("No items available!", "fonts/Marker Felt.ttf", 60);
            noItemsLabel->setColor(Color3B(255, 0, 0));
            noItemsLabel->setPosition(Vec2(winSize.width * 0.5f, winSize.height / 2));
            this->addChild(noItemsLabel, 1000);
        }

        // 创建并布置卡牌
        for (int i = 0; i < itemCount; ++i) {
            // 获取实际商品索引
            int itemIndex = availableItems[i];

            // 计算行列位置
            int row = i / ITEMS_PER_ROW;
            int col = i % ITEMS_PER_ROW;

            // 创建商品卡牌
            auto card = Sprite::create(shopItems[itemIndex].image);
            if (!card) {
                // 如果卡片图片加载失败，创建一个替代图形
                card = Sprite::create();
                auto cardNode = DrawNode::create();
                cardNode->drawSolidRect(
                    Vec2(-50, -70),
                    Vec2(50, 70),
                    Color4F(0.5f, 0.5f, 0.7f, 1.0f)
                );
                cardNode->drawRect(
                    Vec2(-50, -70),
                    Vec2(50, 70),
                    Color4F(0.9f, 0.9f, 1.0f, 1.0f)
                );
                card->addChild(cardNode);
            }

            // 使用商品自己的缩放比例
            float cardScale = shopItems[itemIndex].scale;
            card->setScale(cardScale);

            // 设置卡牌位置 - 横向排列成两行
            float posX = START_X + col * HORIZONTAL_SPACING;
            float posY = START_Y - row * VERTICAL_SPACING;
            card->setPosition(Vec2(posX, posY));

            // 存储商品索引和原始缩放比例到用户数据
            auto itemData = new ItemData{ itemIndex, cardScale };
            card->setUserData(itemData); // 存储商品索引和缩放比例
            card->setName("Item_" + std::to_string(itemIndex)); // 设置名称用于标识

            // 添加商品名称 - 针对特定商品增加字体大小
            auto nameLabel = Label::createWithTTF(shopItems[itemIndex].name, "fonts/Marker Felt.ttf",
                (shopItems[itemIndex].name == "Life potion" || shopItems[itemIndex].name == "Coin bag") ? 120 : 60);
            nameLabel->setPosition(Vec2(0, 200));
            nameLabel->setColor(Color3B::RED);
            card->addChild(nameLabel);

            // 添加商品价格 - 针对特定商品增加字体大小
            auto priceLabel = Label::createWithTTF(StringUtils::format("%d Gold", shopItems[itemIndex].price),
                "fonts/Marker Felt.ttf",
                (shopItems[itemIndex].name == "Life potion" || shopItems[itemIndex].name == "Coin bag") ? 120 : 60);
            priceLabel->setPosition(Vec2(0, 50));
            priceLabel->setColor(Color3B::RED);
            card->addChild(priceLabel);

            // 添加商品描述 - 针对特定商品增加字体大小
            auto descLabel = Label::createWithTTF(shopItems[itemIndex].description, "fonts/Marker Felt.ttf",
                (shopItems[itemIndex].name == "Life potion" || shopItems[itemIndex].name == "Coin bag") ? 120 : 50);
            descLabel->setPosition(Vec2(0, -50));
            descLabel->setColor(Color3B::RED);
            card->addChild(descLabel);
            this->addChild(card, 1000);
            _cards.push_back(card);

            // 添加点击事件监听器
            auto listener = EventListenerTouchOneByOne::create();
            listener->setSwallowTouches(true);

            listener->onTouchBegan = [this, card](Touch* touch, Event* event) {
                auto target = static_cast<Sprite*>(event->getCurrentTarget());
                Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
                Size s = target->getContentSize();
                Rect rect = Rect(0, 0, s.width, s.height);

                // 检测点是否在卡牌内
                if (rect.containsPoint(locationInNode)) {
                    return true;
                }
                return false;
                };

            listener->onTouchEnded = [this, card](Touch* touch, Event* event) {
                // 检查卡牌是否依然有效
                if (!card || !card->getParent()) {
                    return;
                }

                // 获取商品索引
                auto itemData = static_cast<ItemData*>(card->getUserData());
                if (!itemData) {
                    return;
                }
                int itemIndex = itemData->index;

                // 获取当前金币数量
                int currentCoins = Hero::getCoins();
                int itemPrice = shopItems[itemIndex].price;

                // 检查是否有足够的金币购买
                if (currentCoins >= itemPrice) {
                    // 如果当前卡牌是悬停卡牌，先清除引用
                    if (_hoveredCard == card) {
                        _hoveredCard = nullptr;
                    }

                    // 扣除金币
                    Hero::setCoins(currentCoins - itemPrice);

                    // 应用商品效果
                    shopItems[itemIndex].effect();

                    // 标记商品为已购买
                    shopItems[itemIndex].purchased = true;

                    // 显示购买成功提示
                    auto successMsg = Label::createWithTTF("Bought successfully", "fonts/Marker Felt.ttf", 40);
                    successMsg->setColor(Color3B(0, 255, 0));
                    successMsg->setPosition(Director::getInstance()->getWinSize() / 2);
                    this->addChild(successMsg, 2000);

                    // 2秒后淡出提示
                    auto fadeOut = FadeOut::create(2.0f);
                    auto remove = RemoveSelf::create();
                    successMsg->runAction(Sequence::create(fadeOut, remove, nullptr));

                    // 释放用户数据内存
                    delete itemData;
                    card->setUserData(nullptr);

                    // 从卡牌列表中移除
                    auto it = std::find(_cards.begin(), _cards.end(), card);
                    if (it != _cards.end()) {
                        _cards.erase(it);
                    }

                    // 从界面中移除已购买的商品
                    card->removeFromParent();

                    // 如果所有商品都已购买，显示消息
                    bool allPurchased = true;
                    for (const auto& item : shopItems) {
                        if (!item.purchased) {
                            allPurchased = false;
                            break;
                        }
                    }

                    if (allPurchased) {
                        auto noItemsLabel = Label::createWithTTF("All items sold out!", "fonts/Marker Felt.ttf", 60);
                        noItemsLabel->setColor(Color3B(255, 0, 0));
                        noItemsLabel->setPosition(Vec2(Director::getInstance()->getWinSize().width * 0.5f,
                            Director::getInstance()->getWinSize().height / 2));
                        this->addChild(noItemsLabel, 1000);
                    }
                }
                else {
                    // 显示金币不足提示
                    auto failMsg = Label::createWithTTF("You don't have enough coins", "fonts/Marker Felt.ttf", 40);
                    failMsg->setColor(Color3B(255, 0, 0));
                    failMsg->setPosition(Director::getInstance()->getWinSize() / 2);
                    this->addChild(failMsg, 2000);

                    // 2秒后淡出提示
                    auto fadeOut = FadeOut::create(2.0f);
                    auto remove = RemoveSelf::create();
                    failMsg->runAction(Sequence::create(fadeOut, remove, nullptr));
                }
                };

            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, card);
        }

        // 添加鼠标事件监听
        if (_mouseListener) {
            _eventDispatcher->removeEventListener(_mouseListener);
            _mouseListener = nullptr;
        }

        _mouseListener = EventListenerMouse::create();
        _mouseListener->onMouseMove = [this, HOVER_SCALE_MULTIPLIER](EventMouse* event) {
            Vec2 mousePos = event->getLocationInView();
            bool foundHovered = false;

            // 遍历所有卡牌检测悬停
            for (auto& card : _cards) {
                // 确保卡牌仍然有效
                if (!card || !card->getParent()) {
                    continue;
                }

                Rect rect = card->getBoundingBox();
                if (rect.containsPoint(mousePos)) {
                    // 进入卡牌区域
                    if (_hoveredCard != card) {
                        // 恢复上一个悬停卡牌
                        if (_hoveredCard && _hoveredCard->getParent()) {
                            _hoveredCard->stopAllActions();
                            // 获取原始缩放比例
                            auto prevItemData = static_cast<ItemData*>(_hoveredCard->getUserData());
                            if (prevItemData) {
                                _hoveredCard->runAction(ScaleTo::create(0.1f, prevItemData->scale));
                            }
                        }
                        // 放大当前卡牌
                        card->stopAllActions();
                        // 获取原始缩放比例并应用悬停倍数
                        auto itemData = static_cast<ItemData*>(card->getUserData());
                        if (itemData) {
                            card->runAction(ScaleTo::create(0.1f, itemData->scale * HOVER_SCALE_MULTIPLIER));
                        }
                        _hoveredCard = card;
                    }
                    foundHovered = true;
                    break;
                }
            }

            // 没有悬停卡牌时恢复
            if (!foundHovered && _hoveredCard) {
                // 确保卡牌仍然有效
                if (_hoveredCard->getParent()) {
                    _hoveredCard->stopAllActions();
                    // 获取原始缩放比例
                    auto itemData = static_cast<ItemData*>(_hoveredCard->getUserData());
                    if (itemData) {
                        _hoveredCard->runAction(ScaleTo::create(0.1f, itemData->scale));
                    }
                }
                _hoveredCard = nullptr;
            }
            };

        _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

        // 添加关闭按钮
        MenuItem* closeButton = nullptr;
        auto closeLabel = Label::createWithTTF("Close", "fonts/Marker Felt.ttf", 100);
        closeLabel->setColor(Color3B(0, 255, 0));
        closeButton = MenuItemLabel::create(closeLabel, [this](Ref* sender) {
            // 清除悬停卡牌引用
            _hoveredCard = nullptr;

            this->removeChildByName("GoodsColumn");
            if (_mouseListener) {
                _eventDispatcher->removeEventListener(_mouseListener);
                _mouseListener = nullptr;
            }

            // 移除所有商品卡牌并释放用户数据
            for (auto& card : _cards) {
                if (card && card->getUserData()) {
                    delete static_cast<ItemData*>(card->getUserData());
                    card->setUserData(nullptr);
                }
                if (card && card->getParent()) {
                    card->removeFromParent();
                }
            }
            _cards.clear();

            // 显示提示文本
            auto hintLabel = this->getChildByName("HintLabel");
            if (hintLabel) {
                hintLabel->setVisible(true);
            }
            });

        if (closeButton) {
            // 将关闭按钮放在商品列表底部中央
            closeButton->setPosition(Vec2(goods->getPosition().x, goods->getPosition().y - winSize.height * 0.25f - 100));

            auto menu = Menu::create(closeButton, nullptr);
            menu->setPosition(Vec2::ZERO);
            this->addChild(menu, 1000);
        }
    }

    // 析构函数中清理内存
    ShopScene::~ShopScene() {
        // 确保释放所有用户数据内存
        for (auto& card : _cards) {
            if (card && card->getUserData()) {
                delete static_cast<ItemData*>(card->getUserData());
                card->setUserData(nullptr);
            }
        }
        _cards.clear();
        _hoveredCard = nullptr;
    }

} // namespace MyGame