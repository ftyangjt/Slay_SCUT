// ../Classes/Shop.cpp ���޸�ʵ��

#include "Shop.h"
#include "Map.h"
#include "Hero.h"
#include "cocos2d.h"

USING_NS_CC;

namespace MyGame {

    // �洢��Ʒ��Ϣ�Ľṹ��
    struct ShopItem {
        std::string name;      // ��Ʒ����
        int price;             // ��Ʒ�۸�
        std::string image;     // ��ƷͼƬ
        std::string description; // ��Ʒ����
        std::function<void()> effect; // ������Ч��
    };

    // �����̵���Ʒ
    static std::vector<ShopItem> shopItems = {
        {"Life potion", 20, "life_potion.jpg", "Restore 20 health points", []() { Hero::healHealth(20); }},
        // �޸� Shop.cpp �е���Ʒ���岿�֣��滻���ز�ݮ��Ч������
        {"Mysterious strawberry", 100, "strawberry.jpg", "Increase maximum health by 10", []() {
            Hero::increaseMaxHealth(10);

            // ��ʾЧ����Ϣ
            auto director = Director::getInstance();
            auto msg = Label::createWithTTF("Max Health increased by 10!",
                "fonts/Marker Felt.ttf", 30);
            msg->setColor(Color3B(255, 105, 180)); // ��ɫ

            Size winSize = director->getWinSize();
            msg->setPosition(Vec2(winSize.width / 2, winSize.height / 2 + 250));

            director->getRunningScene()->addChild(msg, 2001);

            // 2��󵭳���ʾ
            auto fadeOut = FadeOut::create(2.0f);
            auto remove = RemoveSelf::create();
            msg->runAction(Sequence::create(fadeOut, remove, nullptr));
        }}
,
        {"Coin bag", 50, "coin_bag.png", "randomly gain 0-100 coins", []() {
            // ʹ��cocos2d-x�����������
            int randomCoins = cocos2d::random(0, 100);
            Hero::addCoins(randomCoins);

            // ��ʾ��õĽ������
            auto director = Director::getInstance();
            auto coinMsg = Label::createWithTTF(StringUtils::format("Gain %d coins!", randomCoins),
                "fonts/Marker Felt.ttf", 30);
            coinMsg->setColor(Color3B(255, 215, 0)); // ��ɫ

            // ����λ�ü��㷽ʽ
            Size winSize = director->getWinSize();
            coinMsg->setPosition(Vec2(winSize.width / 2, winSize.height / 2 + 50));

            director->getRunningScene()->addChild(coinMsg, 2001);

            // 2��󵭳���ʾ
            auto fadeOut = FadeOut::create(2.0f);
            auto remove = RemoveSelf::create();
            coinMsg->runAction(Sequence::create(fadeOut, remove, nullptr));
        }},
        {"���ؾ���", 60, "mystery_scroll.png", "ϰ�����⼼��", []() { /* ���⼼��Ч�� */ }}
    };


    Scene* ShopScene::createScene() {
        return ShopScene::create();
    }

    bool ShopScene::init() {
        if (!Scene::init()) {
            return false;
        }

        auto director = Director::getInstance();
        auto winSize = director->getWinSize();

        // ȷ����ʼ�����Ա����
        _hoveredCard = nullptr;
        _mouseListener = nullptr;

        // �������� - �����ݴ���
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
            // ʹ����ɫ����Ϊ��ѡ����
            auto colorBg = LayerColor::create(Color4B(32, 32, 64, 255), winSize.width, winSize.height);
            this->addChild(colorBg);
        }

        // ������ˣ��ɵ����
        auto merchant = Sprite::create("shopman.jpg");
        if (merchant) {
            merchant->setPosition(Vec2(winSize.width * 0.9f, winSize.height * 0.35f));
            merchant->setName("Merchant"); // ���ڵ�����
            this->addChild(merchant, 1);   // ȷ���ڱ����ϲ�

            // ���ô����¼�
            auto listener = EventListenerTouchOneByOne::create();
            listener->setSwallowTouches(true);

            listener->onTouchBegan = [=](Touch* touch, Event* event) {
                // ����Ƿ���������
                Vec2 touchLocation = touch->getLocation();
                Rect merchantRect = merchant->getBoundingBox();
                if (merchantRect.containsPoint(touchLocation)) {
                    this->showGoodsColumn(); // ������ʾ��Ʒ����
                    return true; // �����¼�
                }
                return false;
                };
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, merchant);
            // ������������ʾʱ
            auto hintLabel = Label::createWithTTF("Click the shopman\nto see the products", "fonts/Marker Felt.ttf", 100);
            hintLabel->setColor(Color3B(255, 215, 0)); // ��ɫ
            hintLabel->setAlignment(TextHAlignment::CENTER); // �ı����ж���
            hintLabel->setPosition(Vec2(winSize.width * 0.5f, // ������Ļ����
                winSize.height * 0.7f));
            hintLabel->setName("HintLabel");
            this->addChild(hintLabel, 2);

        }
        else {
            log("Merchant image missing!");
            // ������������ͼ��
            auto merchantPlaceholder = DrawNode::create();
            merchantPlaceholder->drawSolidCircle(Vec2::ZERO, 50, 0, 50, Color4F(0.8f, 0.8f, 0.2f, 1.0f));
            merchantPlaceholder->setPosition(Vec2(winSize.width * 0.7f, winSize.height * 0.35f));
            merchantPlaceholder->setName("Merchant");
            this->addChild(merchantPlaceholder, 1);

            // ���ô����¼�
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

            // �ڱ���������ʾʱ
            auto hintLabel = Label::createWithTTF("Click the shopman\nto see the products", "fonts/Marker Felt.ttf", 50);
            hintLabel->setColor(Color3B(255, 215, 0)); // ��ɫ
            hintLabel->setAlignment(TextHAlignment::CENTER); // �ı����ж���
            hintLabel->setPosition(Vec2(winSize.width * 0.5f, // ������Ļ����
                winSize.height * 0.7f));
            hintLabel->setName("HintLabel");
            this->addChild(hintLabel, 2);
        }

        // ��ӽ����ʾ
        auto coinIcon = Sprite::create("coin_icon.png");
        if (!coinIcon) {
            // ���ͼ��ȱʧ���������ͼ��
            coinIcon = Sprite::create();
            auto coinDrawNode = DrawNode::create();
            coinDrawNode->drawSolidCircle(Vec2::ZERO, 15, 0, 20, Color4F(1.0f, 0.85f, 0.0f, 1.0f));
            coinIcon->addChild(coinDrawNode);
        }
        coinIcon->setPosition(Vec2(80, winSize.height - 40));
        coinIcon->setScale(0.5f);
        this->addChild(coinIcon, 5);

        // ��ʾ��ǰ�������
        auto coinLabel = Label::createWithTTF(StringUtils::format("Gold: %d", Hero::getCoins()),
            "fonts/Marker Felt.ttf", 30);
        coinLabel->setColor(Color3B(255, 215, 0)); // ��ɫ
        coinLabel->setPosition(Vec2(150, winSize.height - 40));
        coinLabel->setName("CoinLabel");
        this->addChild(coinLabel, 5);

        // ����"����"��ť - �޸����Ͳ�ƥ������
        // ʹ�û���ָ�������ղ�ͬ���͵Ĳ˵���
        MenuItem* returnItem = nullptr;

        // ����ʹ��ͼƬ��ť
        auto imageButton = MenuItemImage::create(
            "back.jpg",
            "return_selected.png",
            [](Ref* sender) {
                // ���ص�ͼ����
                auto scene = Map::createScene();
                Director::getInstance()->replaceScene(scene);
            }
        );

        if (imageButton) {
            returnItem = imageButton;
        }
        else {
            // ͼƬ����ʧ�ܣ�ʹ���ı���ť
            log("Return button images missing! Using text button instead.");
            auto returnLabel = Label::createWithTTF("����", "fonts/Marker Felt.ttf", 32);
            returnItem = MenuItemLabel::create(returnLabel, [](Ref* sender) {
                auto scene = Map::createScene();
                Director::getInstance()->replaceScene(scene);
                });
        }

        if (returnItem) {
            returnItem->setPosition(Vec2(winSize.width - 150, 50));
            returnItem->setScale(0.5f);  // ������ť��С

            auto menu = Menu::create(returnItem, nullptr);
            menu->setPosition(Vec2::ZERO);
            this->addChild(menu, 10);
        }

        // ��ӳ�������
        auto titleLabel = Label::createWithTTF("Store", "fonts/Marker Felt.ttf", 150);
        titleLabel->setColor(Color3B(0, 0, 0));
        titleLabel->setPosition(Vec2(winSize.width / 2, winSize.height - 50));
        this->addChild(titleLabel, 5);

        return true;
    }

    void ShopScene::showGoodsColumn() {
        auto winSize = Director::getInstance()->getWinSize();

        // ������ʾ�ı�
        auto hintLabel = this->getChildByName("HintLabel");
        if (hintLabel) {
            hintLabel->setVisible(false);
        }

        // ���½����ʾ
        auto coinLabel = this->getChildByName("CoinLabel");
        if (coinLabel) {
            static_cast<Label*>(coinLabel)->setString(StringUtils::format("Gold: %d", Hero::getCoins()));
        }

        // �Ƴ������Ѵ��ڵ���Ʒ����
        this->removeChildByName("GoodsColumn");

        // ������Ʒ���汳��
        Node* goods = nullptr;
        auto goodsSprite = Sprite::create("column.jpg");

        if (goodsSprite) {
            goodsSprite->setName("GoodsColumn");
            // �Ŵ���Ʒ��������Ӧ��Ʒ��������
            goodsSprite->setPosition(Vec2(winSize.width * 0.5f, winSize.height / 2));
            goodsSprite->setScale(3.0f); // ���ӱ����ߴ������ɺ������е���Ʒ
            this->addChild(goodsSprite, 999);
            goods = goodsSprite;
        }
        else {
            log("GoodsColumn image missing! Creating a default one.");
            // ����һ��Ĭ�ϵ���Ʒ�б���
            auto goodsNode = DrawNode::create();
            goodsNode->setName("GoodsColumn");
            goodsNode->drawSolidRect(
                Vec2(-winSize.width * 0.45f, -winSize.height * 0.35f),
                Vec2(winSize.width * 0.45f, winSize.height * 0.35f),
                Color4F(0.2f, 0.2f, 0.3f, 0.9f)
            );
            // ���Ʊ߿�
            goodsNode->drawRect(
                Vec2(-winSize.width * 0.45f, -winSize.height * 0.35f),
                Vec2(winSize.width * 0.45f, winSize.height * 0.35f),
                Color4F(0.8f, 0.7f, 0.3f, 1.0f)
            );
            // ����Ʒ�������
            goodsNode->setPosition(Vec2(winSize.width * 0.5f, winSize.height / 2));
            this->addChild(goodsNode, 999);
            goods = goodsNode;

            // ��ӱ���
            auto titleLabel = Label::createWithTTF("Goods", "fonts/Marker Felt.ttf", 50);
            titleLabel->setColor(Color3B(0, 0, 0));
            // ��������λ�õ���������
            titleLabel->setPosition(Vec2(winSize.width * 0.5f, winSize.height / 2 + winSize.height * 0.25f));
            this->addChild(titleLabel, 1000);
        }

        // ��վɿ�������
        _cards.clear();

        // �µĿ��Ʋ��ֲ���
        const float CARD_SCALE = 0.5f;  // ���ӿ��ƴ�С
        const float HOVER_SCALE = 0.6f; // ��ͣʱ�ķŴ����
        const int ITEMS_PER_ROW = 3;    // ÿ��3����Ʒ
        const int ROWS = 2;             // 2����Ʒ
        const float HORIZONTAL_SPACING = 500.0f; // ˮƽ���
        const float VERTICAL_SPACING = 280.0f;   // ��ֱ���
        const float START_X = goods->getPosition().x - HORIZONTAL_SPACING; // ��ʼX����
        const float START_Y = goods->getPosition().y + VERTICAL_SPACING * 0.5f; // ��ʼY����

        // ��ȡ��Ʒ�����������ʾ6��
        int itemCount = std::min((int)shopItems.size(), ITEMS_PER_ROW * ROWS);

        // ���������ÿ���
        for (int i = 0; i < itemCount; ++i) {
            // ��������λ��
            int row = i / ITEMS_PER_ROW;
            int col = i % ITEMS_PER_ROW;

            // ������Ʒ����
            auto card = Sprite::create(shopItems[i].image);
            if (!card) {
                // �����ƬͼƬ����ʧ�ܣ�����һ�����ͼ��
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
            card->setScale(CARD_SCALE);

            // ���ÿ���λ�� - �������г�����
            float posX = START_X + col * HORIZONTAL_SPACING;
            float posY = START_Y - row * VERTICAL_SPACING;
            card->setPosition(Vec2(posX, posY));

            card->setUserData(new int(i)); // �洢��Ʒ����
            card->setName("Item_" + std::to_string(i)); // �����������ڱ�ʶ

            // �����Ʒ����
            auto nameLabel = Label::createWithTTF(shopItems[i].name, "fonts/Marker Felt.ttf", 60);
            nameLabel->setPosition(Vec2(0, 85));
            nameLabel->setColor(Color3B::BLACK);
            card->addChild(nameLabel);

            // �����Ʒ�۸�
            auto priceLabel = Label::createWithTTF(StringUtils::format("%d Gold", shopItems[i].price),
                "fonts/Marker Felt.ttf", 60);
            priceLabel->setPosition(Vec2(0, -85));
            priceLabel->setColor(Color3B(255, 215, 0)); // ��ɫ
            card->addChild(priceLabel);

            // �����Ʒ����
            auto descLabel = Label::createWithTTF(shopItems[i].description, "fonts/Marker Felt.ttf", 50);
            descLabel->setPosition(Vec2(0, -150));
            descLabel->setColor(Color3B::BLACK);
            card->addChild(descLabel);

            this->addChild(card, 1000);
            _cards.push_back(card);

            // ��ӵ���¼�������
            auto listener = EventListenerTouchOneByOne::create();
            listener->setSwallowTouches(true);

            listener->onTouchBegan = [this, i](Touch* touch, Event* event) {
                auto target = static_cast<Sprite*>(event->getCurrentTarget());
                Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
                Size s = target->getContentSize();
                Rect rect = Rect(0, 0, s.width, s.height);

                // �����Ƿ��ڿ�����
                if (rect.containsPoint(locationInNode)) {
                    return true;
                }
                return false;
                };

            listener->onTouchEnded = [this, i](Touch* touch, Event* event) {
                // ��ȡ��ǰ�������
                int currentCoins = Hero::getCoins();
                int itemPrice = shopItems[i].price;

                // ����Ƿ����㹻�Ľ�ҹ���
                if (currentCoins >= itemPrice) {
                    // �۳����
                    Hero::setCoins(currentCoins - itemPrice);

                    // Ӧ����ƷЧ��
                    shopItems[i].effect();

                    // ���½����ʾ
                    auto coinLabel = this->getChildByName("CoinLabel");
                    if (coinLabel) {
                        static_cast<Label*>(coinLabel)->setString(StringUtils::format("Gold: %d", Hero::getCoins()));
                    }

                    // ��ʾ����ɹ���ʾ
                    auto successMsg = Label::createWithTTF("Bought successfully", "fonts/Marker Felt.ttf", 40);
                    successMsg->setColor(Color3B(0, 255, 0));
                    successMsg->setPosition(Director::getInstance()->getWinSize() / 2);
                    this->addChild(successMsg, 2000);

                    // 2��󵭳���ʾ
                    auto fadeOut = FadeOut::create(2.0f);
                    auto remove = RemoveSelf::create();
                    successMsg->runAction(Sequence::create(fadeOut, remove, nullptr));
                }
                else {
                    // ��ʾ��Ҳ�����ʾ
                    auto failMsg = Label::createWithTTF("You don't have enough coins", "fonts/Marker Felt.ttf", 40);
                    failMsg->setColor(Color3B(255, 0, 0));
                    failMsg->setPosition(Director::getInstance()->getWinSize() / 2);
                    this->addChild(failMsg, 2000);

                    // 2��󵭳���ʾ
                    auto fadeOut = FadeOut::create(2.0f);
                    auto remove = RemoveSelf::create();
                    failMsg->runAction(Sequence::create(fadeOut, remove, nullptr));
                }
                };

            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, card);
        }

        // �������¼�����
        if (_mouseListener) {
            _eventDispatcher->removeEventListener(_mouseListener);
            _mouseListener = nullptr;
        }

        _mouseListener = EventListenerMouse::create();
        _mouseListener->onMouseMove = [this, CARD_SCALE, HOVER_SCALE](EventMouse* event) {
            Vec2 mousePos = event->getLocationInView();

            // �������п��Ƽ����ͣ
            for (auto& card : _cards) {
                Rect rect = card->getBoundingBox();
                if (rect.containsPoint(mousePos)) {
                    // ���뿨������
                    if (_hoveredCard != card) {
                        // �ָ���һ����ͣ����
                        if (_hoveredCard) {
                            _hoveredCard->stopAllActions();
                            _hoveredCard->runAction(ScaleTo::create(0.1f, CARD_SCALE));
                        }
                        // �Ŵ�ǰ����
                        card->stopAllActions();
                        card->runAction(ScaleTo::create(0.1f, HOVER_SCALE));
                        _hoveredCard = card;
                    }
                    return;
                }
            }

            // û����ͣ����ʱ�ָ�
            if (_hoveredCard) {
                _hoveredCard->stopAllActions();
                _hoveredCard->runAction(ScaleTo::create(0.1f, CARD_SCALE));
                _hoveredCard = nullptr;
            }
            };

        _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

        // ��ӹرհ�ť
        MenuItem* closeButton = nullptr;
        auto closeLabel = Label::createWithTTF("Close", "fonts/Marker Felt.ttf", 100);
        closeLabel->setColor(Color3B(0, 255, 0));
        closeButton = MenuItemLabel::create(closeLabel, [this](Ref* sender) {
            this->removeChildByName("GoodsColumn");
            if (_mouseListener) {
                _eventDispatcher->removeEventListener(_mouseListener);
                _mouseListener = nullptr;
            }

            // �Ƴ�������Ʒ����
            for (auto& card : _cards) {
                card->removeFromParent();
            }
            _cards.clear();

            // ��ʾ��ʾ�ı�
            auto hintLabel = this->getChildByName("HintLabel");
            if (hintLabel) {
                hintLabel->setVisible(true);
            }
            });

        if (closeButton) {
            // ���رհ�ť������Ʒ�б�ײ�����
            closeButton->setPosition(Vec2(goods->getPosition().x, goods->getPosition().y - winSize.height * 0.25f));

            auto menu = Menu::create(closeButton, nullptr);
            menu->setPosition(Vec2::ZERO);
            this->addChild(menu, 1000);
        }
    }


} // namespace MyGame
