// ../Classes/Shop.cpp ????????

#include "Shop.h"
#include "Map.h"
#include "Hero.h"
#include "cocos2d.h"

USING_NS_CC;

namespace MyGame {

    // ?›¥???????????
    struct ShopItem {
        std::string name;      // ???????
        int price;             // ??????
        std::string image;     // ?????
        std::string description; // ???????
        std::function<void()> effect; // ??????§¹??
        bool purchased = false; // ????????????????
        float scale = 0.25f;    // ????????????????????0.5
    };

    // ????›¥????????????????????? - ?????????Áæ??
    struct ItemData {
        int index;
        float scale;
    };

    // ??????????
    static std::vector<ShopItem> shopItems = {
        {"Life potion", 20, "life_potion.jpg", "Restore 20 health points", []() { Hero::healHealth(20); }, false, 0.5f},
        // ??? Shop.cpp ?§Ö???????¿ù????I????????§¹??????
        {"Mysterious strawberry", 100, "strawberry.jpg", "Increase maximum health by 10", []() {
            Hero::increaseMaxHealth(10);

            // ???§¹?????
            auto director = Director::getInstance();
            auto msg = Label::createWithTTF("Max Health increased by 10!",
                "fonts/Marker Felt.ttf", 30);
            msg->setColor(Color3B(255, 105, 180)); // ???

            Size winSize = director->getWinSize();
            msg->setPosition(Vec2(winSize.width / 2, winSize.height / 2 + 250));

            director->getRunningScene()->addChild(msg, 2001);

            // 2???????
            auto fadeOut = FadeOut::create(2.0f);
            auto remove = RemoveSelf::create();
            msg->runAction(Sequence::create(fadeOut, remove, nullptr));
        }, false, 0.5f},
        {"Coin bag", 50, "coin_bag.png", "randomly gain 0-100 coins", []() {
            // ???cocos2d-x???????????
            int randomCoins = cocos2d::random(0, 100);
            Hero::addCoins(randomCoins);

            // ?????????????
            auto director = Director::getInstance();
            auto coinMsg = Label::createWithTTF(StringUtils::format("Gain %d coins!", randomCoins),
                "fonts/Marker Felt.ttf", 30);
            coinMsg->setColor(Color3B(255, 215, 0)); // ???

            // ????¦Ë?¨¹????
            Size winSize = director->getWinSize();
            coinMsg->setPosition(Vec2(winSize.width / 2, winSize.height / 2 + 50));

            director->getRunningScene()->addChild(coinMsg, 2001);

            // 2???????
            auto fadeOut = FadeOut::create(2.0f);
            auto remove = RemoveSelf::create();
            coinMsg->runAction(Sequence::create(fadeOut, remove, nullptr));
        }, false, 0.25f},
        {"???????", 60, "mystery_scroll.png", "?????????", []() { /* ??????§¹?? */ }, false, 0.7f}
    };


    Scene* ShopScene::createScene() {
        return ShopScene::create();
    }

    bool ShopScene::init() {
        if (!Scene::init()) {
            return false;
        }

        // ??????????????????????????????????????????????????
        for (auto& item : shopItems) {
            item.purchased = false;
        }

        // ??§Ö?????????...
        auto director = Director::getInstance();
        auto winSize = director->getWinSize();

        // ????????????????
        _hoveredCard = nullptr;
        _mouseListener = nullptr;

        // ???????? - ???????????
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
            // ??????????????????
            auto colorBg = LayerColor::create(Color4B(32, 32, 64, 255), winSize.width, winSize.height);
            this->addChild(colorBg);
        }

        // ???????????????
        auto merchant = Sprite::create("shopman.jpg");
        if (merchant) {
            merchant->setPosition(Vec2(winSize.width * 0.9f, winSize.height * 0.35f));
            merchant->setName("Merchant"); // ?????????
            this->addChild(merchant, 1);   // ???????????

            // ??????????
            auto listener = EventListenerTouchOneByOne::create();
            listener->setSwallowTouches(true);

            listener->onTouchBegan = [=](Touch* touch, Event* event) {
                // ??????????????
                Vec2 touchLocation = touch->getLocation();
                Rect merchantRect = merchant->getBoundingBox();
                if (merchantRect.containsPoint(touchLocation)) {
                    this->showGoodsColumn(); // ??????????????
                    return true; // ???????
                }
                return false;
                };
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, merchant);
            // ??????????????
            auto hintLabel = Label::createWithTTF("Click the shopman\nto see the products", "fonts/Marker Felt.ttf", 100);
            hintLabel->setColor(Color3B(255, 215, 0)); // ???
            hintLabel->setAlignment(TextHAlignment::CENTER); // ??????§Ø???
            hintLabel->setPosition(Vec2(winSize.width * 0.5f, // ???????????
                winSize.height * 0.7f));
            hintLabel->setName("HintLabel");
            this->addChild(hintLabel, 2);

        }
        else {
            log("Merchant image missing!");
        }

<<<<<<< Updated upstream
        // ????"????"???
        auto returnItem = MenuItemImage::create(
            "return.png",  // ????????
            "return_selected.png",  // ?????????
=======
        // ?????????
        auto coinIcon = Sprite::create("coin_icon.png");
        if (!coinIcon) {
            // ????????????????????
            coinIcon = Sprite::create();
            auto coinDrawNode = DrawNode::create();
            coinDrawNode->drawSolidCircle(Vec2::ZERO, 15, 0, 20, Color4F(1.0f, 0.85f, 0.0f, 1.0f));
            coinIcon->addChild(coinDrawNode);
        }
        coinIcon->setPosition(Vec2(80, winSize.height - 40));
        coinIcon->setScale(0.5f);
        this->addChild(coinIcon, 5);

        // ?????????????
        auto coinLabel = Label::createWithTTF(StringUtils::format("Gold: %d", Hero::getCoins()),
            "fonts/Marker Felt.ttf", 30);
        coinLabel->setColor(Color3B(255, 215, 0)); // ???
        coinLabel->setPosition(Vec2(150, winSize.height - 40));
        coinLabel->setName("CoinLabel");
        this->addChild(coinLabel, 5);

        // ????"????"??? - ???????????????
        // ??????????????????????????
        MenuItem* returnItem = nullptr;

        // ????????????
        auto imageButton = MenuItemImage::create(
            "back.png",
            "return_selected.png",
>>>>>>> Stashed changes
            [](Ref* sender) {
                // ??????????
                auto scene = Map::createScene();
                Director::getInstance()->replaceScene(scene);
            }
        );

        if (imageButton) {
            returnItem = imageButton;
        }
        else {
            // ???????????????????
            log("Return button images missing! Using text button instead.");
            auto returnLabel = Label::createWithTTF("????", "fonts/Marker Felt.ttf", 32);
            returnItem = MenuItemLabel::create(returnLabel, [](Ref* sender) {
                auto scene = Map::createScene();
                Director::getInstance()->replaceScene(scene);
                });
        }

        if (returnItem) {
            returnItem->setPosition(Vec2(winSize.width - 150, 50));
            returnItem->setScale(0.5f);  // ?????????§³

            auto menu = Menu::create(returnItem, nullptr);
            menu->setPosition(Vec2::ZERO);
            this->addChild(menu, 10);
        }

        // ???????????
        auto titleLabel = Label::createWithTTF("Store", "fonts/Marker Felt.ttf", 150);
        titleLabel->setColor(Color3B(0, 0, 0));
        titleLabel->setPosition(Vec2(winSize.width / 2, winSize.height - 50));
        this->addChild(titleLabel, 5);

        return true;
    }

    void ShopScene::showGoodsColumn() {
        auto winSize = Director::getInstance()->getWinSize();

        // ??????????
        auto hintLabel = this->getChildByName("HintLabel");
        if (hintLabel) {
            hintLabel->setVisible(false);
        }

        // ?????????
        auto coinLabel = this->getChildByName("CoinLabel");
        if (coinLabel) {
            static_cast<Label*>(coinLabel)->setString(StringUtils::format("Gold: %d", Hero::getCoins()));
        }

        // ????????????????????
        this->removeChildByName("GoodsColumn");

        // ??????????›M??
        Node* goods = nullptr;
        auto goodsSprite = Sprite::create("column.jpg");

        if (goodsSprite) {
            goodsSprite->setName("GoodsColumn");
            // ??????????????????????????
            goodsSprite->setPosition(Vec2(winSize.width * 0.5f, winSize.height / 2));
            goodsSprite->setScale(3.0f); // ??????????????????????§Ö????
            this->addChild(goodsSprite, 999);
            goods = goodsSprite;
        }
        else {
            log("GoodsColumn image missing! Creating a default one.");
            // ???????????????§Ò?????
            auto goodsNode = DrawNode::create();
            goodsNode->setName("GoodsColumn");
            goodsNode->drawSolidRect(
                Vec2(-winSize.width * 0.45f, -winSize.height * 0.35f),
                Vec2(winSize.width * 0.45f, winSize.height * 0.35f),
                Color4F(0.2f, 0.2f, 0.3f, 0.9f)
            );
            // ??????
            goodsNode->drawRect(
                Vec2(-winSize.width * 0.45f, -winSize.height * 0.35f),
                Vec2(winSize.width * 0.45f, winSize.height * 0.35f),
                Color4F(0.8f, 0.7f, 0.3f, 1.0f)
            );
            // ????????????
            goodsNode->setPosition(Vec2(winSize.width * 0.5f, winSize.height / 2));
            this->addChild(goodsNode, 999);
            goods = goodsNode;

            // ???????
            auto titleLabel = Label::createWithTTF("Goods", "fonts/Marker Felt.ttf", 50);
            titleLabel->setColor(Color3B(0, 0, 0));
            // ????????¦Ë???????????
            titleLabel->setPosition(Vec2(winSize.width * 0.5f, winSize.height / 2 + winSize.height * 0.25f));
            this->addChild(titleLabel, 1000);
        }

        // ???????????
        _cards.clear();

        // ????????????????
        _hoveredCard = nullptr;

        // ??????????
        const float HOVER_SCALE_MULTIPLIER = 1.2f; // ?????????????????????????????????????
        const int ITEMS_PER_ROW = 3;    // ???3?????
        const int ROWS = 2;             // 2?????
        const float HORIZONTAL_SPACING = 500.0f; // ?????
        const float VERTICAL_SPACING = 280.0f;   // ??????
        const float START_X = goods->getPosition().x - HORIZONTAL_SPACING; // ???X????
        const float START_Y = goods->getPosition().y + VERTICAL_SPACING * 0.5f; // ???Y????

        // ??????????¦Ä???????
        std::vector<int> availableItems;
        for (size_t i = 0; i < shopItems.size(); ++i) {
            if (!shopItems[i].purchased) {
                availableItems.push_back(i);
            }
        }

        // ?????§Ø????????????
        int itemCount = std::min((int)availableItems.size(), ITEMS_PER_ROW * ROWS);

        // ?????§á?????????????????
        if (itemCount == 0) {
            auto noItemsLabel = Label::createWithTTF("No items available!", "fonts/Marker Felt.ttf", 60);
            noItemsLabel->setColor(Color3B(255, 0, 0));
            noItemsLabel->setPosition(Vec2(winSize.width * 0.5f, winSize.height / 2));
            this->addChild(noItemsLabel, 1000);
        }

        // ?????????????
        for (int i = 0; i < itemCount; ++i) {
            // ?????????????
            int itemIndex = availableItems[i];

            // ????????¦Ë??
            int row = i / ITEMS_PER_ROW;
            int col = i % ITEMS_PER_ROW;

            // ???????????
            auto card = Sprite::create(shopItems[itemIndex].image);
            if (!card) {
                // ?????????????????????????????
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

            // ??????????????????
            float cardScale = shopItems[itemIndex].scale;
            card->setScale(cardScale);

            // ???????¦Ë?? - ???????§Ô?????
            float posX = START_X + col * HORIZONTAL_SPACING;
            float posY = START_Y - row * VERTICAL_SPACING;
            card->setPosition(Vec2(posX, posY));

            // ?›¥???????????????????????????
            auto itemData = new ItemData{ itemIndex, cardScale };
            card->setUserData(itemData); // ?›¥????????????????
            card->setName("Item_" + std::to_string(itemIndex)); // ??????????????

            // ???????????
            auto nameLabel = Label::createWithTTF(shopItems[itemIndex].name, "fonts/Marker Felt.ttf", 60);
            nameLabel->setPosition(Vec2(0, 85));
            nameLabel->setColor(Color3B::BLACK);
            card->addChild(nameLabel);

            // ??????????
            auto priceLabel = Label::createWithTTF(StringUtils::format("%d Gold", shopItems[itemIndex].price),
                "fonts/Marker Felt.ttf", 60);
            priceLabel->setPosition(Vec2(0, -85));
            priceLabel->setColor(Color3B::BLACK); // ???
            card->addChild(priceLabel);

            // ???????????
            auto descLabel = Label::createWithTTF(shopItems[itemIndex].description, "fonts/Marker Felt.ttf", 50);
            descLabel->setPosition(Vec2(0, -150));
            descLabel->setColor(Color3B::BLACK);
            card->addChild(descLabel);

            this->addChild(card, 1000);
            _cards.push_back(card);

            // ???????????????
            auto listener = EventListenerTouchOneByOne::create();
            listener->setSwallowTouches(true);

            listener->onTouchBegan = [this, card](Touch* touch, Event* event) {
                auto target = static_cast<Sprite*>(event->getCurrentTarget());
                Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
                Size s = target->getContentSize();
                Rect rect = Rect(0, 0, s.width, s.height);

                // ??????????????
                if (rect.containsPoint(locationInNode)) {
                    return true;
                }
                return false;
                };

            listener->onTouchEnded = [this, card](Touch* touch, Event* event) {
                // ?????????????§¹
                if (!card || !card->getParent()) {
                    return;
                }

                // ??????????
                auto itemData = static_cast<ItemData*>(card->getUserData());
                if (!itemData) {
                    return;
                }
                int itemIndex = itemData->index;

                // ?????????????
                int currentCoins = Hero::getCoins();
                int itemPrice = shopItems[itemIndex].price;

                // ?????????????????
                if (currentCoins >= itemPrice) {
                    // ?????????????????????????????
                    if (_hoveredCard == card) {
                        _hoveredCard = nullptr;
                    }

                    // ??????
                    Hero::setCoins(currentCoins - itemPrice);

                    // ??????§¹??
                    shopItems[itemIndex].effect();

                    // ????????????
                    shopItems[itemIndex].purchased = true;

                    // ?????????
                    auto coinLabel = this->getChildByName("CoinLabel");
                    if (coinLabel) {
                        static_cast<Label*>(coinLabel)->setString(StringUtils::format("Gold: %d", Hero::getCoins()));
                    }

                    // ????????????
                    auto successMsg = Label::createWithTTF("Bought successfully", "fonts/Marker Felt.ttf", 40);
                    successMsg->setColor(Color3B(0, 255, 0));
                    successMsg->setPosition(Director::getInstance()->getWinSize() / 2);
                    this->addChild(successMsg, 2000);

                    // 2???????
                    auto fadeOut = FadeOut::create(2.0f);
                    auto remove = RemoveSelf::create();
                    successMsg->runAction(Sequence::create(fadeOut, remove, nullptr));

                    // ?????????????
                    delete itemData;
                    card->setUserData(nullptr);

                    // ??????§Ò??????
                    auto it = std::find(_cards.begin(), _cards.end(), card);
                    if (it != _cards.end()) {
                        _cards.erase(it);
                    }

                    // ???????????????????
                    card->removeFromParent();

                    // ???????????????????????
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
                    // ????????????
                    auto failMsg = Label::createWithTTF("You don't have enough coins", "fonts/Marker Felt.ttf", 40);
                    failMsg->setColor(Color3B(255, 0, 0));
                    failMsg->setPosition(Director::getInstance()->getWinSize() / 2);
                    this->addChild(failMsg, 2000);

                    // 2???????
                    auto fadeOut = FadeOut::create(2.0f);
                    auto remove = RemoveSelf::create();
                    failMsg->runAction(Sequence::create(fadeOut, remove, nullptr));
                }
                };

            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, card);
        }

        // ??????????????
        if (_mouseListener) {
            _eventDispatcher->removeEventListener(_mouseListener);
            _mouseListener = nullptr;
        }

        _mouseListener = EventListenerMouse::create();
        _mouseListener->onMouseMove = [this, HOVER_SCALE_MULTIPLIER](EventMouse* event) {
            Vec2 mousePos = event->getLocationInView();
            bool foundHovered = false;

            // ???????§á????????
            for (auto& card : _cards) {
                // ????????????§¹
                if (!card || !card->getParent()) {
                    continue;
                }

                Rect rect = card->getBoundingBox();
                if (rect.containsPoint(mousePos)) {
                    // ??????????
                    if (_hoveredCard != card) {
                        // ???????????????
                        if (_hoveredCard && _hoveredCard->getParent()) {
                            _hoveredCard->stopAllActions();
                            // ????????????
                            auto prevItemData = static_cast<ItemData*>(_hoveredCard->getUserData());
                            if (prevItemData) {
                                _hoveredCard->runAction(ScaleTo::create(0.1f, prevItemData->scale));
                            }
                        }
                        // ????????
                        card->stopAllActions();
                        // ????????????????????????
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

            // ??????????????
            if (!foundHovered && _hoveredCard) {
                // ????????????§¹
                if (_hoveredCard->getParent()) {
                    _hoveredCard->stopAllActions();
                    // ????????????
                    auto itemData = static_cast<ItemData*>(_hoveredCard->getUserData());
                    if (itemData) {
                        _hoveredCard->runAction(ScaleTo::create(0.1f, itemData->scale));
                    }
                }
                _hoveredCard = nullptr;
            }
            };

        _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

        // ????????
        MenuItem* closeButton = nullptr;
        auto closeLabel = Label::createWithTTF("Close", "fonts/Marker Felt.ttf", 100);
        closeLabel->setColor(Color3B(0, 255, 0));
        closeButton = MenuItemLabel::create(closeLabel, [this](Ref* sender) {
            // ??????????????
            _hoveredCard = nullptr;

            this->removeChildByName("GoodsColumn");
            if (_mouseListener) {
                _eventDispatcher->removeEventListener(_mouseListener);
                _mouseListener = nullptr;
            }

            // ?????????????????????????
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

            // ?????????
            auto hintLabel = this->getChildByName("HintLabel");
            if (hintLabel) {
                hintLabel->setVisible(true);
            }
            });

        if (closeButton) {
            // ???????????????§Ò????????
            closeButton->setPosition(Vec2(goods->getPosition().x, goods->getPosition().y - winSize.height * 0.25f));

            auto menu = Menu::create(closeButton, nullptr);
            menu->setPosition(Vec2::ZERO);
            this->addChild(menu, 1000);
        }
    }

    // ?????????????????
    ShopScene::~ShopScene() {
        // ????????????????????
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