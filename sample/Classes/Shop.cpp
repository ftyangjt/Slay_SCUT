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
            returnItem->setPosition(Vec2(winSize.width - 50, 50));
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

        // �Ƴ������Ѵ��ڵ���Ʒ����
        this->removeChildByName("GoodsColumn");

        // ������Ʒ����
        Node* goods = nullptr;
        auto goodsSprite = Sprite::create("column.jpg");

        if (goodsSprite) {
            goodsSprite->setName("GoodsColumn");
            // ����Ʒ���������ƶ�����¶������
            goodsSprite->setPosition(Vec2(winSize.width * 0.4f, winSize.height / 2));
            goodsSprite->setScale(3.0f);
            this->addChild(goodsSprite, 999);
            goods = goodsSprite;
        }
        else {
            log("GoodsColumn image missing! Creating a default one.");
            // ����һ��Ĭ�ϵ���Ʒ�б���
            auto goodsNode = DrawNode::create();
            goodsNode->setName("GoodsColumn");
            goodsNode->drawSolidRect(
                Vec2(-winSize.width * 0.4f, -winSize.height * 0.4f),
                Vec2(winSize.width * 0.4f, winSize.height * 0.4f),
                Color4F(0.2f, 0.2f, 0.3f, 0.9f)
            );
            // ���Ʊ߿�
            goodsNode->drawRect(
                Vec2(-winSize.width * 0.4f, -winSize.height * 0.4f),
                Vec2(winSize.width * 0.4f, winSize.height * 0.4f),
                Color4F(0.8f, 0.7f, 0.3f, 1.0f)
            );
            // ����Ʒ���������ƶ�����¶������
            goodsNode->setPosition(Vec2(winSize.width * 0.3f, winSize.height / 2));
            this->addChild(goodsNode, 999);
            goods = goodsNode;

            // ��ӱ���
            auto titleLabel = Label::createWithTTF("Goods", "fonts/Marker Felt.ttf", 50);
            titleLabel->setColor(Color3B(0, 0, 0));
            // Ҳ��Ҫ��������λ��
            titleLabel->setPosition(Vec2(winSize.width * 0.4f, winSize.height / 2 + winSize.height * 0.35f));
            this->addChild(titleLabel, 1000);
        }


        // ��վɿ�������
        _cards.clear();

        // ���Ʋ��ֲ���
        const int ROWS = 2;
        const int COLS_PER_ROW[ROWS] = { 6, 6 }; // ÿ������
        const float CARD_SCALE = 0.2f;
        const float HOVER_SCALE = 0.25f;
        const float MARGIN_X = 50.0f; // ������
        const float MARGIN_Y = 50.0f; // ������
        const float PADDING_LEFT = 40.0f;
        const float PADDING_BOTTOM = 60.0f;

        Size goodsSize;
        if (auto spriteGoods = dynamic_cast<Sprite*>(goods)) {
            goodsSize = spriteGoods->getContentSize() * goods->getScale();
        }
        else {
            // �����DrawNode��ʹ��Ԥ��ĳߴ�
            goodsSize = Size(winSize.width * 0.8f, winSize.height * 0.8f);
        }
        Vec2 goodsCenter = goods->getPosition();

        // ������ʼ�㣨���½� + �ڱ߾ࣩ
        Vec2 startPos(
            goodsCenter.x - goodsSize.width / 2.45 + PADDING_LEFT,
            goodsCenter.y - goodsSize.height / 3.2 + PADDING_BOTTOM
        );

        // ��ȡ����ģ��ߴ�
        auto cardTemplate = Sprite::create("card_front.png");
        float cardWidth, cardHeight;
        if (cardTemplate) {
            cardWidth = cardTemplate->getContentSize().width * CARD_SCALE;
            cardHeight = cardTemplate->getContentSize().height * CARD_SCALE;
        }
        else {
            log("Card template image missing! Using default size.");
            cardWidth = 100.0f * CARD_SCALE;  // Ĭ�Ͽ��
            cardHeight = 150.0f * CARD_SCALE; // Ĭ�ϸ߶�
        }

        // �洢��һ��ǰ���ſ��Ƶ�λ��
        Vec2 firstRowFirstCardPos;
        Vec2 firstRowSecondCardPos;

        // ��������
        int totalIndex = 0; // ȫ������
        for (int row = 0; row < ROWS; ++row) {
            int cols = COLS_PER_ROW[row]; // ��ǰ������

            // ���㵱ǰ�е���ʼXƫ�ƣ����ڶ�����Ҫ����ǰ���ţ�
            float rowStartX = startPos.x;
            if (row == 1 && !_cards.empty()) {
                firstRowFirstCardPos = _cards[0]->getPosition(); // ��һ�ſ���λ��
                if (_cards.size() > 1) {
                    firstRowSecondCardPos = _cards[1]->getPosition(); // �ڶ��ſ���λ��
                }
                else {
                    // ���ֻ��һ�ſ�������ڶ��ŵ�λ��
                    firstRowSecondCardPos = Vec2(
                        firstRowFirstCardPos.x + cardWidth + MARGIN_X,
                        firstRowFirstCardPos.y
                    );
                }
                rowStartX = firstRowFirstCardPos.x; // ���õ�һ�е�һ�ŵ�Xλ��
            }

            for (int col = 0; col < cols; ++col) {
                auto card = Sprite::create("card_front.png");
                if (!card) {
                    // �����ƬͼƬ����ʧ�ܣ�����һ�����ͼ��
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

                // ��һ�У���������λ��
                if (row == 0) {
                    float posX = startPos.x + col * (cardWidth + MARGIN_X);
                    float posY = startPos.y + (ROWS - 1 - row) * (cardHeight + MARGIN_Y);
                    card->setPosition(Vec2(posX, posY));

                    // ��¼ǰ���ſ���λ��
                    if (col == 0) firstRowFirstCardPos = card->getPosition();
                    if (col == 1) firstRowSecondCardPos = card->getPosition();
                }
                // �ڶ��У�ǰ���Ŷ��룬�����Ŷ���
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

                // ��ӿ�Ƭ����
                auto cardName = Label::createWithTTF("Card #" + std::to_string(totalIndex + 1),
                    "fonts/Marker Felt.ttf", 12);
                cardName->setPosition(Vec2(0, -cardHeight / 2 - 10));
                card->addChild(cardName);

                this->addChild(card, goods->getLocalZOrder() + 1);
                _cards.push_back(card); // ������ָ���������
                totalIndex++;
            }
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

        // ��ӹرհ�ť - ʹ��MenuItem���࣬��������ת������
        MenuItem* closeButton = nullptr;
        auto closeLabel = Label::createWithTTF("�ر�", "fonts/Marker Felt.ttf", 24);
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
