// Map.cpp
#include "Map.h"
#include "FightingScene.h"
#include "question.h"
#include "Rest.h"
#include "Shop.h"
#include "Hero.h"
#include "GameSaveManager.h"
#include <random>

USING_NS_CC;

namespace MyGame {
<<<<<<< Updated upstream
=======
    // ʵ�ʶ���ͳ�ʼ����̬����
    RoomType currentRoomType = RoomType::BATTLE;
>>>>>>> Stashed changes

    // �洢��ͼ���ݣ��������ͺ�λ����Ϣ�������洢ָ�룩
    struct RoomInfo {
        RoomType type;
        cocos2d::Vec2 position;
    };
    static std::vector<std::vector<RoomInfo>> staticMapInfo;

    // �洢��������
    static std::vector<ConnectionInfo> staticConnectionInfo;

    // �洢��ǰ�ɷ��ʵ���߲㼶����̬�������ڳ����л�ʱ���֣�
    static int maxAccessibleLayer = 0;

    // �Ƿ�ո������һ�����䣨���ڴӷ��䳡�����ص�ͼʱ�жϣ�
    static bool roomCompleted = false;

    // ��¼��ǰѡ��ķ���λ�ã������ж���һ����Щ������Է��ʣ�
    static cocos2d::Vec2 currentRoomPosition;

    // ����������Ϸ״̬�ĺ���ʵ��
    void resetGameState() {
        // ��յ�ͼ��Ϣ
        staticMapInfo.clear();

        // ���������Ϣ
        staticConnectionInfo.clear();

        // ���õ�ǰ����λ��
        currentRoomPosition = Vec2::ZERO;

        // ���ÿɷ��ʲ㼶
        maxAccessibleLayer = 0;

        // ���÷������״̬
        roomCompleted = false;
    }

    Scene* Map::createScene()
    {
        return Map::create();
    }

    bool Map::init()
    {
        if (!Scene::init())
        {
            return false;
        }

        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto origin = Director::getInstance()->getVisibleOrigin();

        // ��ʼ����ǰ��͵�ǰ����
        currentLayer = 0;
        currentRoom = -1;

        // ���� ScrollView
        scrollView = ui::ScrollView::create();
        scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
        scrollView->setContentSize(visibleSize);
        scrollView->setInnerContainerSize(Size(visibleSize.width, visibleSize.height * 3));  // ���ù�����Χ
        scrollView->setPosition(origin);
        this->addChild(scrollView);

        // ���ӵ�ͼ����ͼƬ
        // ���ӵ�ͼ����ͼƬ
        auto background = Sprite::create("map_background.jpg");
        if (background)
        {
            // ��ȡ����ͼƬԭʼ��С�͹���������С
            Size bgSize = background->getContentSize();
            Size containerSize = scrollView->getInnerContainerSize();

            // ������������ʹͼƬ������Ӧ��Ļ����
            float scaleX = visibleSize.width / bgSize.width;
            // ������������ʹͼƬ�߶���Ӧ��������߶�
            float scaleY = containerSize.height / bgSize.height;
            // ʹ�ýϴ������������ȷ��ͼƬ���������ɼ�����
            float scale = std::max(scaleX, scaleY);

            // ���ñ���ͼƬ����
            background->setScale(scale);

            // ���ñ���ͼƬλ�õ�������������
            background->setPosition(Vec2(visibleSize.width / 2, containerSize.height / 2));

            scrollView->addChild(background, 0);  // ������ͼƬ���ӵ���ײ�
        }

        // ����Ƿ��Ѿ������˵�ͼ����
        if (staticMapInfo.empty())
        {
            // ���ɶ���ͼ
            int layers = 18;  // ������10��
            int roomsPerLayer = 5;  // ÿ����3������
            generateRandomMap(layers, roomsPerLayer);
        }
        else
        {
            // ʹ���Ѵ洢�ĵ�ͼ�����������ɵ�ͼ
            generateMapFromSavedInfo();
        }

        // ����ո������һ�����䣬���ӿɷ��ʵĲ㼶
        if (roomCompleted)
        {
            maxAccessibleLayer++;
            roomCompleted = false;
        }

        // ���·���Ŀɷ�����
        updateRoomAccessibility();

        // ���Ӳ㼶��ʾ��ǩ
        createLayerLabels();
        createSaveButton();

        return true;
    }

    // �����㼶��ǩ
    void Map::createLayerLabels()
    {
        auto visibleSize = Director::getInstance()->getVisibleSize();

        // ���ӵ�ǰ�ɷ��ʲ㼶��ʾ
        auto accessLabel = Label::createWithTTF(
            "�ɷ��ʲ㼶: " + std::to_string(maxAccessibleLayer + 1),
            "fonts/Marker Felt.ttf",
            32
        );
        accessLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 50));
        this->addChild(accessLabel, 3);
    }

    // ���·���Ŀɷ�����
    void Map::updateRoomAccessibility()
    {
        // �Ƚ����з�����Ϊ����״̬
        for (int i = 0; i < mapLayers.size(); i++)
        {
            for (auto& room : mapLayers[i])
            {
                room.item->setEnabled(false);
                room.item->setColor(Color3B(100, 100, 100)); // ��ɫ
            }
        }

        // �����������Ϸ��ʼʱ����һ�����з��䶼�ɷ���
        if (maxAccessibleLayer == 0 && currentRoomPosition == Vec2::ZERO)
        {
            for (auto& room : mapLayers[0])
            {
                room.item->setEnabled(true);
                room.item->setColor(Color3B(255, 255, 255)); // ������ɫ
            }
            return;
        }

        // �����ÿɷ��ʲ㼶��ͨ��·�����ӵķ���
        int accessibleLayer = maxAccessibleLayer;

        // �����ǰû��ѡ�з��䣬���Ѿ�ͨ����һЩ�㼶
        if (currentRoomPosition == Vec2::ZERO && maxAccessibleLayer > 0) {
            // �����½���������з���
            for (auto& room : mapLayers[maxAccessibleLayer]) {
                room.item->setEnabled(true);
                room.item->setColor(Color3B(255, 255, 255)); // ������ɫ
            }
            return;
        }

        // ����������Ϣ���ҳ��ӵ�ǰ����λ�ÿ��Ե������һ�㷿��
        for (auto& connection : staticConnectionInfo) {
            // ������ߵ�����Ƿ��뵱ǰ����λ��ƥ��
            if (connection.start.equals(currentRoomPosition)) {
                // �ҵ��������յ�����Ӧ�ķ���
                for (auto& room : mapLayers[maxAccessibleLayer]) {
                    // �������λ���������յ�ƥ�䣬�����ø÷���
                    if (room.item->getPosition().equals(connection.end)) {
                        room.item->setEnabled(true);
                        room.item->setColor(Color3B(255, 255, 255)); // ������ɫ
                    }
                }
            }
        }
    }

    RoomType Map::getRandomRoomType()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());

        // ���¸��ʷֲ�������ELITE����
        // START(0), BATTLE(1), QUESTION(2), REST(3), BOSS(4), ELITE(5)
        static std::discrete_distribution<> dis({ 0, 45, 20, 10, 0, 15,10 });  // ��ELITE 15%�ĸ���

        // ��ȡ���ֵ��ת��ΪRoomType
        int randomType = dis(gen);
        return static_cast<RoomType>(randomType);
    }

    void Map::generateRandomMap(int layers, int roomsPerLayer)
    {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto origin = Director::getInstance()->getVisibleOrigin();

        // �����������
        static std::random_device rd;
        static std::mt19937 gen(rd());

        // ���ӿ�ʼ��ͼ��
        auto startItem = MenuItemImage::create(
            "start.png",  // ʹ������ͼ��
            "start_selected.png",
            [](Ref*) {}  // ��ʼ�㲻��Ҫ�ص�����
        );
        if (startItem)
        {
            startItem->setPosition(Vec2(
                visibleSize.width / 2,
                scrollView->getInnerContainerSize().height - 100  // ���ÿ�ʼ��λ��
            ));
            startItem->setScale(0.5);  // ����ͼ���С
            auto menu = Menu::create(startItem, nullptr);
            menu->setPosition(Vec2::ZERO);
            scrollView->addChild(menu, 1);  // ȷ��ͼ���ڱ���ͼƬ֮��
        }

        // �������
        mapLayers.clear();
        staticMapInfo.clear();

        // ������С������
        int minRooms = std::max(3, roomsPerLayer - 2); // �������Ϊ3���趨ֵ-2

        for (int i = 0; i < layers; ++i)
        {
            std::vector<Room> layer;
            std::vector<RoomInfo> layerInfo;  // �洢��ǰ��ķ�����Ϣ
            int roomCount;  // ÿ��ķ�������

            // ���ݲ�ͬ�㼶���ò�ͬ�ķ�������������
            if (i == layers - 1) {
                // ���һ�㣨Boss�㣩ֻ��1������
                roomCount = 1;
            }
            else if (i == layers - 2) {
                // Boss��ǰһ��������3�����䣨������Ϣ���䣩
                roomCount = roomsPerLayer;
            }
            else {
                // ������������ɷ�����������Сֵ��roomsPerLayer֮��
                std::uniform_int_distribution<> roomDist(minRooms, roomsPerLayer);
                roomCount = roomDist(gen);
            }

            for (int j = 0; j < roomCount; ++j)
            {
                RoomType roomType;

                // ���ݲ�ͬ�㼶���ò�ͬ�ķ�������
                if (i == layers - 1) {
                    roomType = RoomType::BOSS;  // ���һ���� BOSS ����
                }
                else if (i == layers - 2) {
                    roomType = RoomType::REST;  // Boss��ǰһ��ȫ������Ϣ����
                }
                else {
                    roomType = getRandomRoomType();  // ������������ɷ�������
                }

                MenuItemImage* roomItem = nullptr;

                switch (roomType)
                {
                case RoomType::BATTLE:
                    roomItem = MenuItemImage::create(
                        "battle_normal.png",
                        "battle_selected.png",
                        CC_CALLBACK_1(Map::menuBattleCallback, this)
                    );
                    roomItem->setScale(0.5);
                    break;
                case RoomType::QUESTION:
                    roomItem = MenuItemImage::create(
                        "question_normal.png",
                        "question_selected.png",
                        CC_CALLBACK_1(Map::menuQuestionCallback, this)
                    );
                    roomItem->setScale(0.15);
                    break;
                case RoomType::REST:
                    roomItem = MenuItemImage::create(
                        "rest.jpg",
                        "rest_selected.png",
                        CC_CALLBACK_1(Map::menuRestCallback, this)
                    );
                    roomItem->setScale(0.5);
                    break;
                case RoomType::BOSS:
                    roomItem = MenuItemImage::create(
                        "boss.png",  // ʹ������ͼ��
                        "boss_selected.png",
                        CC_CALLBACK_1(Map::menuBossCallback, this)
                    );
                    roomItem->setScale(0.5);
                    break;

                case RoomType::ELITE:
                    roomItem = MenuItemImage::create(
                        "elite_normal.png",  // ��Ҫ׼����Щ��ԴͼƬ
                        "elite_selected.png",
                        CC_CALLBACK_1(Map::menuEliteCallback, this)
                    );
                    roomItem->setScale(0.5);
                    break;
				case RoomType::SHOP:
					roomItem = MenuItemImage::create(
						"shop_normal.png",
						"shop_selected.png",
						CC_CALLBACK_1(Map::menuShopCallback, this)
					);
                    roomItem->setScale(0.15);
					break;
                }
                if (roomItem)
                {
                    // ���ݷ�����������λ��
                    Vec2 position;
                    if (roomCount == 1) {
                        // ���ֻ��һ�����䣬�����м�
                        position = Vec2(
                            visibleSize.width / 2,
                            scrollView->getInnerContainerSize().height - (i + 1) * 200  // ���ݲ�������λ��
                        );
                    }
                    else {
                        // ���ݷ����������ȷֲ�
                        float spacing = visibleSize.width / (roomCount + 1);
                        position = Vec2(
                            origin.x + spacing * (j + 1),
                            scrollView->getInnerContainerSize().height - (i + 1) * 200  // ���ݲ�������λ��
                        );
                    }

                    roomItem->setPosition(position);
                    // �洢�㼶��Ϣ
                    roomItem->setTag(i);

                    auto menu = Menu::create(roomItem, nullptr);
                    menu->setPosition(Vec2::ZERO);
                    scrollView->addChild(menu, 1);  // ȷ���˵��ڱ���ͼƬ֮��

                    layer.push_back({ roomType, roomItem });

                    // ����λ����Ϣ�Ա�����ؽ�
                    layerInfo.push_back({ roomType, position });
                }
            }
            mapLayers.push_back(layer);
            staticMapInfo.push_back(layerInfo);  // �洢�ò���Ϣ
        }

        connectRooms(mapLayers);
    }

    void Map::generateMapFromSavedInfo()
    {
        // ������е�ͼ����
        mapLayers.clear();

        // ���ݱ���ĵ�ͼ��Ϣ���´�������
        for (int i = 0; i < staticMapInfo.size(); i++)
        {
            const auto& layerInfo = staticMapInfo[i];
            std::vector<Room> layer;

            for (const auto& roomInfo : layerInfo)
            {
                MenuItemImage* roomItem = nullptr;

                switch (roomInfo.type)
                {
                case RoomType::BATTLE:
                    roomItem = MenuItemImage::create(
                        "battle_normal.png",
                        "battle_selected.png",
                        CC_CALLBACK_1(Map::menuBattleCallback, this)
                    );
                    roomItem->setScale(0.5);
                    break;
                case RoomType::QUESTION:
                    roomItem = MenuItemImage::create(
                        "question_normal.png",
                        "question_selected.png",
                        CC_CALLBACK_1(Map::menuQuestionCallback, this)
                    );
                    roomItem->setScale(0.15);
                    break;
                case RoomType::REST:
                    roomItem = MenuItemImage::create(
                        "rest.jpg",
                        "rest_selected.png",
                        CC_CALLBACK_1(Map::menuRestCallback, this)
                    );
                    roomItem->setScale(0.5);
                    break;
                case RoomType::BOSS:
                    roomItem = MenuItemImage::create(
                        "boss.png",  // ʹ������ͼ��
                        "boss_selected.png",
                        CC_CALLBACK_1(Map::menuBossCallback, this)
                    );
                    roomItem->setScale(0.5);
                    break;
                case RoomType::ELITE:
                    roomItem = MenuItemImage::create(
                        "elite_normal.png",  // ��Ҫ׼����Щ��ԴͼƬ
                        "elite_selected.png",
                        CC_CALLBACK_1(Map::menuEliteCallback, this)
                    );
                    roomItem->setScale(0.5);
                    break;
                case RoomType::SHOP:
                    roomItem = MenuItemImage::create(
                        "shop_normal.png",  // ʹ���̵�ͼ��
                        "shop_selected.png",
                        CC_CALLBACK_1(Map::menuShopCallback, this)
                    );
                    roomItem->setScale(0.15);
                    break;

                }

                if (roomItem)
                {
                    roomItem->setPosition(roomInfo.position);
                    // �洢�㼶��Ϣ
                    roomItem->setTag(i);

                    auto menu = Menu::create(roomItem, nullptr);
                    menu->setPosition(Vec2::ZERO);
                    scrollView->addChild(menu, 1);  // ȷ���˵��ڱ���ͼƬ֮��

                    layer.push_back({ roomInfo.type, roomItem });
                }
            }
            mapLayers.push_back(layer);
        }

        connectRooms(mapLayers);
    }

    void Map::connectRooms(const std::vector<std::vector<Room>>& map)
    {
        // ����һ��DrawNode���ڻ��������ߣ�����zOrderʹ��λ��ͼ���·�
        auto drawNode = DrawNode::create();
        scrollView->addChild(drawNode, 0);  // zOrderΪ0���ᱻzOrderΪ1�ķ���ͼ�긲��

        // ����Ѿ��б����������Ϣ��ֱ��ʹ��
        if (!staticConnectionInfo.empty()) {
            for (const auto& connection : staticConnectionInfo) {
                // ʹ�ý���ɫ������������㵽�յ���ǳ��ɫ���ɵ�����ɫ
                Color4F startColor(0.4f, 0.6f, 1.0f, 0.8f);  // ǳ��ɫ����͸��
                Color4F endColor(0.1f, 0.3f, 0.8f, 0.8f);    // ����ɫ����͸��

                // ���ƴ��߶Σ�ʹ�ý���Ч��
                float lineWidth = 3.0f;  // ������������

                // �Ȼ�����ӰЧ��
                drawNode->drawSegment(
                    connection.start,
                    connection.end,
                    lineWidth + 1.0f,
                    Color4F(0.0f, 0.0f, 0.0f, 0.3f)
                );

                // �ٻ�������
                drawNode->drawSegment(
                    connection.start,
                    connection.end,
                    lineWidth,
                    startColor
                );

                // �ڹؼ������СԲ�㣬�����Ӿ�Ч��
                drawNode->drawDot(connection.start, lineWidth * 1.2f, startColor);
                drawNode->drawDot(connection.end, lineWidth * 1.2f, endColor);
            }
            return;
        }

        // ���������Ϣ�Ա���������
        staticConnectionInfo.clear();

        for (size_t i = 0; i < map.size() - 1; ++i)
        {
            const auto& currentLayer = map[i];
            const auto& nextLayer = map[i + 1];

            if (nextLayer.empty() || currentLayer.empty()) {
                continue;  // ȷ���㲻Ϊ��
            }

            // ����BOSS������������ֻ��һ�����䣩
            if (nextLayer.size() == 1) {
                // ���е�ǰ�㷿�䶼���ӵ���һ���BOSS����
                auto bossPos = nextLayer[0].item->getPosition();
                for (const auto& currentRoom : currentLayer) {
                    auto startPos = currentRoom.item->getPosition();

                    // ʹ�ý���ɫ������
                    Color4F startColor(0.4f, 0.6f, 1.0f, 0.8f);
                    Color4F endColor(0.1f, 0.3f, 0.8f, 0.8f);

                    // �Ȼ�����Ӱ
                    drawNode->drawSegment(
                        startPos,
                        bossPos,
                        3.0f + 1.0f,  // ���߿���+��Ӱ����
                        Color4F(0.0f, 0.0f, 0.0f, 0.3f)
                    );

                    // �ٻ�������
                    drawNode->drawSegment(startPos, bossPos, 3.0f, startColor);

                    // ���ӵ�׺Ч��
                    drawNode->drawDot(startPos, 4.0f, startColor);
                    drawNode->drawDot(bossPos, 4.0f, endColor);

                    // ����������Ϣ
                    staticConnectionInfo.push_back({ startPos, bossPos });
                }
                continue;
            }

            // ��¼��һ�㷿��������״̬
            std::vector<bool> nextRoomConnected(nextLayer.size(), false);

            // Ϊÿ����ǰ��ķ��䴴�����ӣ�������Խһ������
            for (size_t j = 0; j < currentLayer.size(); j++)
            {
                auto startPos = currentLayer[j].item->getPosition();

                // ȷ����ǰ��������һ��������ӵķ���������Χ
                // ���������Խһ�����䣬�����ܿ�Խ����
                int maxConnections = 2; // ����ÿ���������������һ��ķ�����
                int connectionsCount = 0;

                // ���ݵ�ǰ�����һ��ķ���������ϵȷ�����Ӳ���
                if (nextLayer.size() <= 2) {
                    // �����һ�㷿������3�������з��䶼���Ի�������
                    for (size_t k = 0; k < nextLayer.size() && connectionsCount < maxConnections; k++) {
                        auto endPos = nextLayer[k].item->getPosition();

                        // ʹ�ý���ɫ������
                        Color4F startColor(0.4f, 0.6f, 1.0f, 0.8f);  // ǳ��ɫ
                        Color4F endColor(0.1f, 0.3f, 0.8f, 0.8f);    // ����ɫ

                        // �Ȼ�����Ӱ
                        drawNode->drawSegment(
                            startPos,
                            endPos,
                            3.0f + 1.0f,
                            Color4F(0.0f, 0.0f, 0.0f, 0.3f)
                        );

                        // �ٻ�������
                        drawNode->drawSegment(startPos, endPos, 3.0f, startColor);

                        // ���ӵ�׺Ч��
                        drawNode->drawDot(startPos, 4.0f, startColor);
                        drawNode->drawDot(endPos, 4.0f, endColor);

                        // ����������Ϣ
                        staticConnectionInfo.push_back({ startPos, endPos });
                        nextRoomConnected[k] = true;
                        connectionsCount++;
                    }
                }
                else {
                    // Ϊÿ�������ҵ�"����"������λ��
                    // ��ԭ������ͬ���߼�
                    int baseIndex;

                    if (currentLayer.size() <= nextLayer.size()) {
                        baseIndex = j * nextLayer.size() / currentLayer.size();
                    }
                    else {
                        baseIndex = j * nextLayer.size() / currentLayer.size();
                        baseIndex = std::min(baseIndex, (int)nextLayer.size() - 1);
                    }

                    int minIndex = std::max(0, baseIndex - 1);
                    int maxIndex = std::min((int)nextLayer.size() - 1, baseIndex + 1);

                    std::vector<int> connectIndexes;
                    connectIndexes.push_back(baseIndex);
                    nextRoomConnected[baseIndex] = true;
                    connectionsCount++;

                    if (std::rand() % 2 == 0 && connectionsCount < maxConnections) {
                        std::vector<int> remainingIndexes;
                        for (int k = minIndex; k <= maxIndex; k++) {
                            if (k != baseIndex) {
                                remainingIndexes.push_back(k);
                            }
                        }

                        if (!remainingIndexes.empty()) {
                            int randomIdx = remainingIndexes[std::rand() % remainingIndexes.size()];
                            connectIndexes.push_back(randomIdx);
                            nextRoomConnected[randomIdx] = true;
                            connectionsCount++;
                        }
                    }

                    // �����������ӣ�ʹ������Ч��
                    for (int idx : connectIndexes) {
                        auto endPos = nextLayer[idx].item->getPosition();

                        // ʹ�ý���ɫ������
                        Color4F startColor(0.4f, 0.6f, 1.0f, 0.8f);
                        Color4F endColor(0.1f, 0.3f, 0.8f, 0.8f);

                        // �Ȼ�����Ӱ
                        drawNode->drawSegment(
                            startPos,
                            endPos,
                            3.0f + 1.0f,
                            Color4F(0.0f, 0.0f, 0.0f, 0.3f)
                        );

                        // �ٻ�������
                        drawNode->drawSegment(startPos, endPos, 3.0f, startColor);

                        // ���ӵ�׺Ч��
                        drawNode->drawDot(startPos, 4.0f, startColor);
                        drawNode->drawDot(endPos, 4.0f, endColor);

                        // ����������Ϣ
                        staticConnectionInfo.push_back({ startPos, endPos });
                    }
                }
            }

            // ȷ�������²㷿�䶼������
            for (size_t j = 0; j < nextLayer.size(); j++) {
                if (!nextRoomConnected[j]) {
                    // �ҵ����λ��������ϲ㷿��
                    int closestRoomIndex = 0;
                    float minDistance = FLT_MAX;

                    for (size_t k = 0; k < currentLayer.size(); k++) {
                        float xDist = currentLayer[k].item->getPosition().x - nextLayer[j].item->getPosition().x;
                        float dist = fabs(xDist);  // ֻ����ˮƽ����

                        if (dist < minDistance) {
                            minDistance = dist;
                            closestRoomIndex = k;
                        }
                    }

                    auto startPos = currentLayer[closestRoomIndex].item->getPosition();
                    auto endPos = nextLayer[j].item->getPosition();

                    // ʹ�ý���ɫ������
                    Color4F startColor(0.4f, 0.6f, 1.0f, 0.8f);
                    Color4F endColor(0.1f, 0.3f, 0.8f, 0.8f);

                    // �Ȼ�����Ӱ
                    drawNode->drawSegment(
                        startPos,
                        endPos,
                        3.0f + 1.0f,
                        Color4F(0.0f, 0.0f, 0.0f, 0.3f)
                    );

                    // �ٻ�������
                    drawNode->drawSegment(startPos, endPos, 3.0f, startColor);

                    // ���ӵ�׺Ч��
                    drawNode->drawDot(startPos, 4.0f, startColor);
                    drawNode->drawDot(endPos, 4.0f, endColor);

                    // ����������Ϣ
                    staticConnectionInfo.push_back({ startPos, endPos });
                    nextRoomConnected[j] = true;
                }
            }
        }
    }


    void Map::menuBattleCallback(Ref* pSender)
    {
        auto item = static_cast<MenuItemImage*>(pSender);
        int layerIndex = item->getTag();

        // ����Ƿ���Է�������㼶
        if (layerIndex > maxAccessibleLayer)
        {
            // ������ʾ��ǩ
            auto visibleSize = Director::getInstance()->getVisibleSize();
            auto label = Label::createWithTTF("unable to enter now", "fonts/Marker Felt.ttf", 32);
            label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
            this->addChild(label, 5);

            // 2����Ƴ���ʾ
            label->runAction(Sequence::create(
                DelayTime::create(2.0f),
                RemoveSelf::create(),
                nullptr
            ));

            return;
        }

        // ���õ�ǰѡ�еĲ�ͷ���
        currentLayer = layerIndex;
        currentRoom = 0;

        // ��¼��ǰ����λ��
        currentRoomPosition = item->getPosition();

        currentRoomType = RoomType::BATTLE;

        // ���Ϊ����ɷ���
        roomCompleted = true;

        // �л���ս������
        auto scene = FightingScene::createScene();
        // ʹ�õ��뵭������Ч��������1��
        auto transition = TransitionFade::create(1.0f, scene, Color3B(0, 0, 0)); // ��ɫ���뵭��

        // �л���������Ч���ĵ�ͼ����
        Director::getInstance()->replaceScene(transition);
    }

    void Map::menuQuestionCallback(Ref* pSender)
    {
        auto item = static_cast<MenuItemImage*>(pSender);
        int layerIndex = item->getTag();

        // ����Ƿ���Է�������㼶
        if (layerIndex > maxAccessibleLayer)
        {
            // ������ʾ��ǩ
            auto visibleSize = Director::getInstance()->getVisibleSize();
            auto label = Label::createWithTTF("unable to enter now", "fonts/Marker Felt.ttf", 32);
            label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
            this->addChild(label, 5);

            // 2����Ƴ���ʾ
            label->runAction(Sequence::create(
                DelayTime::create(2.0f),
                RemoveSelf::create(),
                nullptr
            ));

            return;
        }

        // ���õ�ǰѡ�еĲ�ͷ���
        currentLayer = layerIndex;
        currentRoom = 0;
		currentRoomType = RoomType::QUESTION;

        // ��¼��ǰ����λ��
        currentRoomPosition = item->getPosition();

        // ������һ��
        currentRoomType = RoomType::QUESTION;

        // ���Ϊ����ɷ���
        roomCompleted = true;

        // �л����ʺų���
        auto scene = Question::createScene();
        // ʹ�õ��뵭������Ч��������1��
        auto transition = TransitionFade::create(1.0f, scene, Color3B(0, 0, 0)); // ��ɫ���뵭��
		Director::getInstance()->replaceScene(transition);
    }

    void Map::menuRestCallback(Ref* pSender)
    {
        auto item = static_cast<MenuItemImage*>(pSender);
        int layerIndex = item->getTag();

        // ����Ƿ���Է�������㼶
        if (layerIndex > maxAccessibleLayer)
        {
            // ������ʾ��ǩ
            auto visibleSize = Director::getInstance()->getVisibleSize();
            auto label = Label::createWithTTF("unable to enter now", "fonts/Marker Felt.ttf", 32);
            label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
            this->addChild(label, 5);

            // 2����Ƴ���ʾ
            label->runAction(Sequence::create(
                DelayTime::create(2.0f),
                RemoveSelf::create(),
                nullptr
            ));

            return;
        }

        // ���õ�ǰѡ�еĲ�ͷ���
        currentLayer = layerIndex;
        currentRoom = 0;
        currentRoomType = RoomType::REST;

        // ��¼��ǰ����λ��
        currentRoomPosition = item->getPosition();

        // ������һ��
        currentRoomType = RoomType::REST;

        // ���Ϊ����ɷ���
        roomCompleted = true;

        // �л�����Ϣ����
        auto scene = Rest::createScene();
        // ʹ�õ��뵭������Ч��������1��
        auto transition = TransitionFade::create(1.0f, scene, Color3B(0, 0, 0)); // ��ɫ���뵭��
        // �л���������Ч���ĵ�ͼ����
        Director::getInstance()->replaceScene(transition);
    }

    void Map::menuBossCallback(Ref* pSender)
    {
        auto item = static_cast<MenuItemImage*>(pSender);
        int layerIndex = item->getTag();

        // ����Ƿ���Է�������㼶
        if (layerIndex > maxAccessibleLayer)
        {
            // ������ʾ��ǩ
            auto visibleSize = Director::getInstance()->getVisibleSize();
            auto label = Label::createWithTTF("Unable to enter now", "fonts/Marker Felt.ttf", 32);
            label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
            this->addChild(label, 5);

            // 2����Ƴ���ʾ
            label->runAction(Sequence::create(
                DelayTime::create(2.0f),
                RemoveSelf::create(),
                nullptr
            ));

            return;
        }

        // ���õ�ǰѡ�еĲ�ͷ���
        currentLayer = layerIndex;
        currentRoom = 0;

        // ��¼��ǰ����λ��
        currentRoomPosition = item->getPosition();

		currentRoomType = RoomType::BOSS;

        // ���Ϊ����ɷ���
        roomCompleted = true;

        // �л��� BOSS ս������
        auto scene = FightingScene::createScene();
        // ʹ�õ��뵭������Ч��������1��
        auto transition = TransitionFade::create(1.0f, scene, Color3B(0, 0, 0)); // ��ɫ���뵭��
        // �л���������Ч���ĵ�ͼ����
        Director::getInstance()->replaceScene(transition);
    }

    void Map::menuEliteCallback(Ref* pSender)
    {
        auto item = static_cast<MenuItemImage*>(pSender);
        int layerIndex = item->getTag();

        // ����Ƿ���Է�������㼶
        if (layerIndex > maxAccessibleLayer)
        {
            // ������ʾ��ǩ
            auto visibleSize = Director::getInstance()->getVisibleSize();
            auto label = Label::createWithTTF("�޷�����˷���", "fonts/Marker Felt.ttf", 32);
            label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
            this->addChild(label, 5);

            // 2����Ƴ���ʾ
            label->runAction(Sequence::create(
                DelayTime::create(2.0f),
                RemoveSelf::create(),
                nullptr
            ));

            return;
        }

        // ���õ�ǰѡ�еĲ�ͷ���
        currentLayer = layerIndex;
        currentRoom = 0;

        // ��¼��ǰ����λ��
        currentRoomPosition = item->getPosition();

        currentRoomType = RoomType::ELITE;

        // ���Ϊ����ɷ���
        roomCompleted = true;

        // �л���ս������ - ʹ���������ص���ͬ�ķ�ʽ
        auto scene = FightingScene::createScene();

        auto transition = TransitionFade::create(1.0f, scene, Color3B(0, 0, 0)); // ��ɫ���뵭��
        // �л���������Ч���ĵ�ͼ����
        Director::getInstance()->replaceScene(transition);

    }

    void Map::menuShopCallback(Ref* pSender)
    {
        auto item = static_cast<MenuItemImage*>(pSender);
        int layerIndex = item->getTag();

        // ����Ƿ���Է�������㼶
        if (layerIndex > maxAccessibleLayer)
        {
            // ������ʾ��ǩ
            auto visibleSize = Director::getInstance()->getVisibleSize();
            auto label = Label::createWithTTF("�޷������̵�", "fonts/Marker Felt.ttf", 32);
            label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
            this->addChild(label, 5);

            // 2����Ƴ���ʾ
            label->runAction(Sequence::create(
                DelayTime::create(2.0f),
                RemoveSelf::create(),
                nullptr
            ));

            return;
        }

        // ���õ�ǰѡ�еĲ�ͷ���
        currentLayer = layerIndex;
        currentRoom = 0;

        // ��¼��ǰ����λ��
        currentRoomPosition = item->getPosition();

        // ������һ��
        currentRoomType = RoomType::SHOP;

        // ���Ϊ����ɷ���
        roomCompleted = true;

        // �л����̵곡��
        auto scene = ShopScene::createScene();

        auto transition = TransitionFade::create(1.0f, scene, Color3B(0, 0, 0)); // ��ɫ���뵭��
        // �л���������Ч���ĵ�ͼ����
        Director::getInstance()->replaceScene(transition);
    }

    // ��Map��������
    void Map::createSaveButton()
    {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();

        // �������水ť
        auto saveButton = ui::Button::create("button.png", "button_selected.png");
        saveButton->setPosition(Vec2(origin.x + visibleSize.width - 50, origin.y + 50));
        saveButton->setScale(0.5f);

        // ���ӵ���¼�
        saveButton->addClickEventListener([this](Ref* sender) {
            saveGame();
            });

        this->addChild(saveButton, 10);
    }

    void Map::saveGame()
    {
        // ��ȡ��ǰ��Ϸ״̬
        int health = Hero::getCurrentHealth();
        int gold = Hero::getCoins();
        const std::vector<Card>& deck = Hero::getDeck();

        // ������Ϸ�����ڴ������е�ͼ�������
        if (GameSaveManager::saveGame(
            health,
            gold,
            deck,
            currentLayer,
            currentRoom,
            staticMapInfo,
            staticConnectionInfo,
            maxAccessibleLayer,
            currentRoomPosition,
            currentRoomType,
            roomCompleted
        ))
        {
            // ��ʾ����ɹ���ʾ
            auto visibleSize = Director::getInstance()->getVisibleSize();
            Vec2 origin = Director::getInstance()->getVisibleOrigin();

            auto saveLabel = Label::createWithTTF("��Ϸ�ѱ���", "fonts/Marker Felt.ttf", 40);
            saveLabel->setColor(Color3B::GREEN);
            saveLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
            this->addChild(saveLabel, 100);

            // 2����Ƴ���ʾ
            saveLabel->runAction(Sequence::create(
                DelayTime::create(2.0f),
                FadeOut::create(0.5f),
                RemoveSelf::create(),
                nullptr
            ));
        }
    }


}