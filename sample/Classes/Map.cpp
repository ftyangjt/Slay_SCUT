// Map.cpp
#include "Map.h"
#include "FightingScene.h"
#include "question.h"
#include "Rest.h"
#include <random>

USING_NS_CC;

namespace MyGame {

    // 存储地图数据（房间类型和位置信息，但不存储指针）
    struct RoomInfo {
        RoomType type;
        cocos2d::Vec2 position;
    };
    static std::vector<std::vector<RoomInfo>> staticMapInfo;

    // 存储当前可访问的最高层级（静态变量，在场景切换时保持）
    static int maxAccessibleLayer = 0;

    // 是否刚刚完成了一个房间（用于从房间场景返回地图时判断）
    static bool roomCompleted = false;

    // 添加重置游戏状态的函数实现
    void resetGameState() {
        // 清空地图信息
        staticMapInfo.clear();

        // 重置可访问层级
        maxAccessibleLayer = 0;

        // 重置房间完成状态
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

        // 初始化当前层和当前房间
        currentLayer = 0;
        currentRoom = -1;

        // 创建 ScrollView
        scrollView = ui::ScrollView::create();
        scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
        scrollView->setContentSize(visibleSize);
        scrollView->setInnerContainerSize(Size(visibleSize.width, visibleSize.height * 3));  // 设置滚动范围
        scrollView->setPosition(origin);
        this->addChild(scrollView);

        // 添加地图背景图片
        auto background = Sprite::create("map_background.png");
        if (background)
        {
            background->setPosition(Vec2(visibleSize.width / 2, scrollView->getInnerContainerSize().height / 2));
            scrollView->addChild(background, 0);  // 将背景图片添加到最底层
        }

        // 检查是否已经生成了地图数据
        if (staticMapInfo.empty())
        {
            // 生成多层地图
            int layers = 10;  // 假设有10层
            int roomsPerLayer = 3;  // 每层有3个房间
            generateRandomMap(layers, roomsPerLayer);
        }
        else
        {
            // 使用已存储的地图数据重新生成地图
            generateMapFromSavedInfo();
        }

        // 如果刚刚完成了一个房间，增加可访问的层级
        if (roomCompleted)
        {
            maxAccessibleLayer++;
            roomCompleted = false;
        }

        // 更新房间的可访问性
        updateRoomAccessibility();

        // 添加层级提示标签
        createLayerLabels();

        return true;
    }

    // 创建层级标签
    void Map::createLayerLabels()
    {
        auto visibleSize = Director::getInstance()->getVisibleSize();

        // 添加当前可访问层级提示
        auto accessLabel = Label::createWithTTF(
            "可访问层级: " + std::to_string(maxAccessibleLayer + 1),
            "fonts/Marker Felt.ttf",
            32
        );
        accessLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 50));
        this->addChild(accessLabel, 3);
    }

    // 更新房间的可访问性
    void Map::updateRoomAccessibility()
    {
        for (int i = 0; i < mapLayers.size(); i++)
        {
            for (auto& room : mapLayers[i])
            {
                // 如果当前层级大于最大可访问层级，则禁用点击
                if (i > maxAccessibleLayer)
                {
                    room.item->setEnabled(false);
                    // 设置灰色蒙版表示无法访问
                    room.item->setColor(Color3B(100, 100, 100));
                }
                else
                {
                    room.item->setEnabled(true);
                    room.item->setColor(Color3B(255, 255, 255));
                }
            }
        }
    }

    RoomType Map::getRandomRoomType()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());

        // 使用离散分布来设置不同房间类型的概率
        // BATTLE: 0.5, QUESTION: 0.3, REST: 0.2
        static std::discrete_distribution<> dis({ 0, 50, 40, 10 });  // 对应 START(0), BATTLE(1), QUESTION(2), REST(3)

        // 获取随机值并转换为RoomType
        int randomType = dis(gen);
        return static_cast<RoomType>(randomType);
    }


    void Map::generateRandomMap(int layers, int roomsPerLayer)
    {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto origin = Director::getInstance()->getVisibleOrigin();

        // 添加开始点图标
        auto startItem = MenuItemImage::create(
            "start.png",  // 使用特殊图案
            "start_selected.png",
            [](Ref*) {}  // 开始点不需要回调函数
        );
        if (startItem)
        {
            startItem->setPosition(Vec2(
                visibleSize.width / 2,
                scrollView->getInnerContainerSize().height - 100  // 设置开始点位置
            ));
            startItem->setScale(0.5);  // 调整图标大小
            auto menu = Menu::create(startItem, nullptr);
            menu->setPosition(Vec2::ZERO);
            scrollView->addChild(menu, 1);  // 确保图标在背景图片之上
        }

        // 清空数据
        mapLayers.clear();
        staticMapInfo.clear();

        for (int i = 0; i < layers; ++i)
        {
            std::vector<Room> layer;
            std::vector<RoomInfo> layerInfo;  // 存储当前层的房间信息
            int roomCount;  // 每层的房间数量

            // 根据不同层级设置不同的房间数量和类型
            if (i == layers - 1) {
                // 最后一层（Boss层）只有1个房间
                roomCount = 1;
            }
            else if (i == layers - 2) {
                // Boss层前一层至少有3个房间（都是休息房间）
                roomCount = roomsPerLayer >= 3 ? roomsPerLayer : 3;
            }
            else {
                // 其他层至少有3个房间
                roomCount = roomsPerLayer >= 3 ? roomsPerLayer : 3;
            }

            for (int j = 0; j < roomCount; ++j)
            {
                RoomType roomType;

                // 根据不同层级设置不同的房间类型
                if (i == layers - 1) {
                    roomType = RoomType::BOSS;  // 最后一层是 BOSS 房间
                }
                else if (i == layers - 2) {
                    roomType = RoomType::REST;  // Boss层前一层全部是休息房间
                }
                else {
                    roomType = getRandomRoomType();  // 其他层随机生成房间类型
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
                    break;
                case RoomType::QUESTION:
                    roomItem = MenuItemImage::create(
                        "question_normal.jpg",
                        "question_selected.png",
                        CC_CALLBACK_1(Map::menuQuestionCallback, this)
                    );
                    break;
                case RoomType::REST:
                    roomItem = MenuItemImage::create(
                        "rest.jpg",
                        "rest_selected.png",
                        CC_CALLBACK_1(Map::menuRestCallback, this)
                    );
                    break;
                case RoomType::BOSS:
                    roomItem = MenuItemImage::create(
                        "boss.png",  // 使用特殊图案
                        "boss_selected.png",
                        CC_CALLBACK_1(Map::menuBossCallback, this)
                    );
                    break;
                }

                if (roomItem)
                {
                    // 根据房间数量调整位置
                    Vec2 position;
                    if (roomCount == 1) {

                     // 如果只有一个房间，放在中间
                        position = Vec2(
                            visibleSize.width / 2,
                            scrollView->getInnerContainerSize().height - (i + 1) * 200  // 根据层数调整位置
                        );
                    }
                    else {
                        // 根据房间数量均匀分布
                        float spacing = visibleSize.width / (roomCount + 1);
                        position = Vec2(
                            origin.x + spacing * (j + 1),
                            scrollView->getInnerContainerSize().height - (i + 1) * 200  // 根据层数调整位置
                        );
                    }

                    roomItem->setPosition(position);
                    roomItem->setScale(0.5);  // 调整按钮大小
                    // 存储层级信息
                    roomItem->setTag(i);

                    auto menu = Menu::create(roomItem, nullptr);
                    menu->setPosition(Vec2::ZERO);
                    scrollView->addChild(menu, 1);  // 确保菜单在背景图片之上

                    layer.push_back({ roomType, roomItem });

                    // 保存位置信息以便后续重建
                    layerInfo.push_back({ roomType, position });
                }
            }
            mapLayers.push_back(layer);
            staticMapInfo.push_back(layerInfo);  // 存储该层信息
        }

        connectRooms(mapLayers);
    }

    void Map::generateMapFromSavedInfo()
    {
        // 清空现有地图数据
        mapLayers.clear();

        // 根据保存的地图信息重新创建房间
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
                    break;
                case RoomType::QUESTION:
                    roomItem = MenuItemImage::create(
                        "question_normal.jpg",
                        "question_selected.png",
                        CC_CALLBACK_1(Map::menuQuestionCallback, this)
                    );
                    break;
                case RoomType::REST:
                    roomItem = MenuItemImage::create(
                        "rest.jpg",
                        "rest_selected.png",
                        CC_CALLBACK_1(Map::menuRestCallback, this)
                    );
                    break;
                case RoomType::BOSS:
                    roomItem = MenuItemImage::create(
                        "boss.png",  // 使用特殊图案
                        "boss_selected.png",
                        CC_CALLBACK_1(Map::menuBossCallback, this)
                    );
                    break;
                }

                if (roomItem)
                {
                    roomItem->setPosition(roomInfo.position);
                    roomItem->setScale(0.5);  // 调整按钮大小
                    // 存储层级信息
                    roomItem->setTag(i);

                    auto menu = Menu::create(roomItem, nullptr);
                    menu->setPosition(Vec2::ZERO);
                    scrollView->addChild(menu, 1);  // 确保菜单在背景图片之上

                    layer.push_back({ roomInfo.type, roomItem });
                }
            }
            mapLayers.push_back(layer);
        }

        connectRooms(mapLayers);
    }

    void Map::connectRooms(const std::vector<std::vector<Room>>& map)
    {
        auto drawNode = DrawNode::create();
        scrollView->addChild(drawNode, 2);  // 确保连接线在按钮之上

        for (size_t i = 0; i < map.size() - 1; ++i)
        {
            const auto& currentLayer = map[i];
            const auto& nextLayer = map[i + 1];

            if (nextLayer.empty() || currentLayer.empty()) {
                continue;  // 确保层不为空
            }

            // 处理BOSS层的特殊情况（只有一个房间）
            if (nextLayer.size() == 1) {
                // 所有当前层房间都连接到下一层的BOSS房间
                auto bossPos = nextLayer[0].item->getPosition();
                for (const auto& currentRoom : currentLayer) {
                    auto startPos = currentRoom.item->getPosition();
                    drawNode->drawLine(startPos, bossPos, Color4F::WHITE);
                }
                continue;
            }

            // 确保每个房间至少有一个连接到下一层的房间
            for (const auto& currentRoom : currentLayer)
            {
                // 随机选择一个下一层的房间进行连接
                int nextRoomIndex = std::rand() % nextLayer.size();
                auto startPos = currentRoom.item->getPosition();
                auto endPos = nextLayer[nextRoomIndex].item->getPosition();
                drawNode->drawLine(startPos, endPos, Color4F::WHITE);
            }

            // 确保每个房间至少有一个连接到上一层的房间
            for (const auto& nextRoom : nextLayer)
            {
                // 随机选择一个当前层的房间进行连接
                int currentRoomIndex = std::rand() % currentLayer.size();
                auto startPos = currentLayer[currentRoomIndex].item->getPosition();
                auto endPos = nextRoom.item->getPosition();
                drawNode->drawLine(startPos, endPos, Color4F::WHITE);
            }

            // 随机添加额外的连接
            for (const auto& currentRoom : currentLayer)
            {
                for (const auto& nextRoom : nextLayer)
                {
                    if (std::rand() % 2 == 0)
                    {
                        auto startPos = currentRoom.item->getPosition();
                        auto endPos = nextRoom.item->getPosition();
                        drawNode->drawLine(startPos, endPos, Color4F::WHITE);
                    }
                }
            }
        }
    }

    void Map::menuBattleCallback(Ref* pSender)
    {
        auto item = static_cast<MenuItemImage*>(pSender);
        int layerIndex = item->getTag();

        // 检查是否可以访问这个层级
        if (layerIndex > maxAccessibleLayer)
        {
            // 创建提示标签
            auto visibleSize = Director::getInstance()->getVisibleSize();
            auto label = Label::createWithTTF("unable to enter now", "fonts/Marker Felt.ttf", 32);
            label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
            this->addChild(label, 5);

            // 2秒后移除提示
            label->runAction(Sequence::create(
                DelayTime::create(2.0f),
                RemoveSelf::create(),
                nullptr
            ));

            return;
        }

        // 设置当前选中的层和房间
        currentLayer = layerIndex;
        currentRoom = 0;

        // 标记为已完成房间
        roomCompleted = true;

        // 切换到战斗场景
        auto scene = FightingScene::createScene();
        Director::getInstance()->replaceScene(scene);
    }

    void Map::menuQuestionCallback(Ref* pSender)
    {
        auto item = static_cast<MenuItemImage*>(pSender);
        int layerIndex = item->getTag();

        // 检查是否可以访问这个层级
        if (layerIndex > maxAccessibleLayer)
        {
            // 创建提示标签
            auto visibleSize = Director::getInstance()->getVisibleSize();
            auto label = Label::createWithTTF("unable to enter now", "fonts/Marker Felt.ttf", 32);
            label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
            this->addChild(label, 5);

            // 2秒后移除提示
            label->runAction(Sequence::create(
                DelayTime::create(2.0f),
                RemoveSelf::create(),
                nullptr
            ));

            return;
        }

        // 设置当前选中的层和房间
        currentLayer = layerIndex;
        currentRoom = 0;

        // 标记为已完成房间
        roomCompleted = true;

        // 切换到问号场景
        auto scene = Question::createScene();
        Director::getInstance()->replaceScene(scene);
    }

    void Map::menuRestCallback(Ref* pSender)
    {
        auto item = static_cast<MenuItemImage*>(pSender);
        int layerIndex = item->getTag();

        // 检查是否可以访问这个层级
        if (layerIndex > maxAccessibleLayer)
        {
            // 创建提示标签
            auto visibleSize = Director::getInstance()->getVisibleSize();
            auto label = Label::createWithTTF("unable to enter now", "fonts/Marker Felt.ttf", 32);
            label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
            this->addChild(label, 5);

            // 2秒后移除提示
            label->runAction(Sequence::create(
                DelayTime::create(2.0f),
                RemoveSelf::create(),
                nullptr
            ));

            return;
        }

        // 设置当前选中的层和房间
        currentLayer = layerIndex;
        currentRoom = 0;

        // 标记为已完成房间
        roomCompleted = true;

        // 切换到休息场景
        auto scene = Rest::createScene();
        Director::getInstance()->replaceScene(scene);
    }

    void Map::menuBossCallback(Ref* pSender)
    {
        auto item = static_cast<MenuItemImage*>(pSender);
        int layerIndex = item->getTag();

        // 检查是否可以访问这个层级
        if (layerIndex > maxAccessibleLayer)
        {
            // 创建提示标签
            auto visibleSize = Director::getInstance()->getVisibleSize();
            auto label = Label::createWithTTF("Unable to enter now", "fonts/Marker Felt.ttf", 32);
            label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
            this->addChild(label, 5);

            // 2秒后移除提示
            label->runAction(Sequence::create(
                DelayTime::create(2.0f),
                RemoveSelf::create(),
                nullptr
            ));

            return;
        }

        // 设置当前选中的层和房间
        currentLayer = layerIndex;
        currentRoom = 0;

        // 标记为已完成房间
        roomCompleted = true;

        // 切换到 BOSS 战斗场景
        auto scene = FightingScene::createScene();
        Director::getInstance()->replaceScene(scene);
    }

} // namespace MyGame
