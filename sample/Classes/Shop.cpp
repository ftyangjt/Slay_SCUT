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

        // ��������
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

        // ������ˣ��ɵ����
        auto merchant = Sprite::create("Merchant.jpg");
        if (merchant) {
            merchant->setPosition(Vec2(winSize.width * 0.7f, winSize.height * 0.35f));
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
        }
        else {
            log("Merchant image missing!");
        }

        // ����"����"��ť
        auto returnItem = MenuItemImage::create(
            "return.png",  // ���ذ�ťͼƬ
            "return_selected.png",  // ѡ��״̬��ͼƬ
            [](Ref* sender) {
                // ���ص�ͼ����
                auto scene = Map::createScene();
                Director::getInstance()->replaceScene(scene);
            }
        );
        if (returnItem)
        {
            returnItem->setPosition(Vec2(winSize.width - 50, 50));
            returnItem->setScale(0.5f);  // ������ť��С

            auto menu = Menu::create(returnItem, nullptr);
            menu->setPosition(Vec2::ZERO);
            this->addChild(menu, 10);
        }

        return true;
    }

    void ShopScene::showGoodsColumn() {
        auto winSize = Director::getInstance()->getWinSize();

        // ������Ʒ����
        auto goods = Sprite::create("GoodsColumn.jpg");
        if (goods) {
            goods->setName("GoodsColumn"); // ���ýڵ�����
            goods->setPosition(winSize / 2);
            goods->setScale(1.2f);
            this->addChild(goods, 999);

            // ��վɿ�������
            _cards.clear();

            // ���Ʋ��ֲ���
            const int ROWS = 2;
            const int COLS_PER_ROW[ROWS] = { 5, 3 }; // ÿ������
            const float CARD_SCALE = 0.2f;
            const float HOVER_SCALE = 0.25f;
            const float MARGIN_X = 25.0f; // ������
            const float MARGIN_Y = 50.0f; // ������
            const float PADDING_LEFT = 40.0f;
            const float PADDING_BOTTOM = 60.0f;

            Size goodsSize = goods->getContentSize() * goods->getScale();
            Vec2 goodsCenter = goods->getPosition();

            // ������ʼ�㣨���½� + �ڱ߾ࣩ
            Vec2 startPos(
                goodsCenter.x - goodsSize.width / 2.45 + PADDING_LEFT,
                goodsCenter.y - goodsSize.height / 3.2 + PADDING_BOTTOM
            );

            // ��ȡ����ģ��ߴ�
            auto cardTemplate = Sprite::create("card_front.png");
            float cardWidth = cardTemplate->getContentSize().width * CARD_SCALE;
            float cardHeight = cardTemplate->getContentSize().height * CARD_SCALE;

            // �洢��һ��ǰ���ſ��Ƶ�λ��
            Vec2 firstRowFirstCardPos;
            Vec2 firstRowSecondCardPos;

            // ��������
            int totalIndex = 0; // ȫ������
            for (int row = 0; row < ROWS; ++row) {
                int cols = COLS_PER_ROW[row]; // ��ǰ������

                // ���㵱ǰ�е���ʼXƫ�ƣ����ڶ�����Ҫ����ǰ���ţ�
                float rowStartX = startPos.x;
                if (row == 1) {
                    rowStartX = firstRowFirstCardPos.x; // ���õ�һ�е�һ�ŵ�Xλ��
                }

                for (int col = 0; col < cols; ++col) {
                    auto card = Sprite::create("card_front.png");
                    if (card) {
                        card->setScale(CARD_SCALE);

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

                        this->addChild(card, goods->getLocalZOrder() + 1);
                        _cards.push_back(card); // ������ָ���������
                        totalIndex++;
                    }
                }
            }

            // �������¼�����
            if (!_mouseListener) {
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
            }
        }
        else {
            log("GoodsColumn image missing!");
        }
    }

} // namespace MyGame
