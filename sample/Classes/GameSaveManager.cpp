// GameSaveManager.cpp
#include "GameSaveManager.h"
#include "CardLibrary.h"
#include "Card.h"
#include "base/CCUserDefault.h"
#include <sstream>

USING_NS_CC;

// 存档键
const char* SAVE_KEY_EXIST = "save_exist";
const char* SAVE_KEY_HEALTH = "save_health";
const char* SAVE_KEY_GOLD = "save_gold";
const char* SAVE_KEY_DECK = "save_deck";
const char* SAVE_KEY_MAP_ID = "save_map_id";
const char* SAVE_KEY_ROOM = "save_room";
// 新增键值
const char* SAVE_KEY_MAP_INFO = "save_map_info";
const char* SAVE_KEY_CONNECTIONS = "save_connections";
const char* SAVE_KEY_MAX_LAYER = "save_max_layer";
const char* SAVE_KEY_ROOM_POS_X = "save_room_pos_x";
const char* SAVE_KEY_ROOM_POS_Y = "save_room_pos_y";
const char* SAVE_KEY_ROOM_TYPE = "save_room_type";
const char* SAVE_KEY_COMPLETED = "save_completed";


bool GameSaveManager::saveGame(
    int health,
    int gold,
    const std::vector<Card>& deck,
    int currentMapId,
    int currentRoom,
    const std::vector<std::vector<MyGame::RoomInfo>>& mapInfo,
    const std::vector<MyGame::ConnectionInfo>& connectionInfo,
    int maxLayer,
    const cocos2d::Vec2& currentRoomPos,
    MyGame::RoomType roomType,
    bool completed
)
{
    // 获取UserDefault实例
    auto userDefault = UserDefault::getInstance();

    // 序列化卡组
    std::string deckString = serializeDeck(deck);

    // 序列化地图数据
    std::string mapInfoString = serializeMapInfo(mapInfo);
    std::string connectionString = serializeConnections(connectionInfo);

    // 保存数据
    userDefault->setBoolForKey(SAVE_KEY_EXIST, true);
    userDefault->setIntegerForKey(SAVE_KEY_HEALTH, health);
    userDefault->setIntegerForKey(SAVE_KEY_GOLD, gold);
    userDefault->setStringForKey(SAVE_KEY_DECK, deckString);
    userDefault->setIntegerForKey(SAVE_KEY_MAP_ID, currentMapId);
    userDefault->setIntegerForKey(SAVE_KEY_ROOM, currentRoom);

    // 保存新增的地图数据
    userDefault->setStringForKey(SAVE_KEY_MAP_INFO, mapInfoString);
    userDefault->setStringForKey(SAVE_KEY_CONNECTIONS, connectionString);
    userDefault->setIntegerForKey(SAVE_KEY_MAX_LAYER, maxLayer);
    userDefault->setFloatForKey(SAVE_KEY_ROOM_POS_X, currentRoomPos.x);
    userDefault->setFloatForKey(SAVE_KEY_ROOM_POS_Y, currentRoomPos.y);
    userDefault->setIntegerForKey(SAVE_KEY_ROOM_TYPE, static_cast<int>(roomType));
    userDefault->setBoolForKey(SAVE_KEY_COMPLETED, completed);

    // 确保数据被写入
    userDefault->flush();

    CCLOG("Game saved successfully");
    return true;
}

bool GameSaveManager::loadGame(
    int& health,
    int& gold,
    std::vector<Card>& deck,
    int& currentMapId,
    int& currentRoom,
    std::vector<std::vector<MyGame::RoomInfo>>& mapInfo,
    std::vector<MyGame::ConnectionInfo>& connectionInfo,
    int& maxLayer,
    cocos2d::Vec2& currentRoomPos,
    MyGame::RoomType& roomType,
    bool& completed
)
{
    // 获取UserDefault实例
    auto userDefault = UserDefault::getInstance();

    // 检查是否有存档
    if (!userDefault->getBoolForKey(SAVE_KEY_EXIST, false))
    {
        CCLOG("No saved game found");
        return false;
    }

    // 加载数据
    health = userDefault->getIntegerForKey(SAVE_KEY_HEALTH, 100);
    gold = userDefault->getIntegerForKey(SAVE_KEY_GOLD, 0);
    std::string deckString = userDefault->getStringForKey(SAVE_KEY_DECK, "");
    currentMapId = userDefault->getIntegerForKey(SAVE_KEY_MAP_ID, 0);
    currentRoom = userDefault->getIntegerForKey(SAVE_KEY_ROOM, 0);

    // 加载新增的地图数据
    std::string mapInfoString = userDefault->getStringForKey(SAVE_KEY_MAP_INFO, "");
    std::string connectionString = userDefault->getStringForKey(SAVE_KEY_CONNECTIONS, "");
    maxLayer = userDefault->getIntegerForKey(SAVE_KEY_MAX_LAYER, 0);
    float posX = userDefault->getFloatForKey(SAVE_KEY_ROOM_POS_X, 0.0f);
    float posY = userDefault->getFloatForKey(SAVE_KEY_ROOM_POS_Y, 0.0f);
    currentRoomPos.set(posX, posY);
    int roomTypeInt = userDefault->getIntegerForKey(SAVE_KEY_ROOM_TYPE, 0);
    roomType = static_cast<MyGame::RoomType>(roomTypeInt);
    completed = userDefault->getBoolForKey(SAVE_KEY_COMPLETED, false);

    // 反序列化卡组
    deck = deserializeDeck(deckString);

    // 反序列化地图数据
    if (!mapInfoString.empty()) {
        mapInfo = deserializeMapInfo(mapInfoString);
    }

    if (!connectionString.empty()) {
        connectionInfo = deserializeConnections(connectionString);
    }

    CCLOG("Game loaded successfully");
    return true;
}

// 卡组序列化和反序列化方法保持不变
// 序列化卡组
std::string GameSaveManager::serializeDeck(const std::vector<Card>& deck)
{
    std::string result = "";
    for (size_t i = 0; i < deck.size(); ++i)
    {
        // 只保存卡牌名称，用分号分隔
        result += deck[i].getName();
        if (i < deck.size() - 1)
        {
            result += ";";
        }
    }
    return result;
}

// 反序列化卡组
std::vector<Card> GameSaveManager::deserializeDeck(const std::string& deckString)
{
    std::vector<Card> deck;
    if (deckString.empty())
    {
        return deck;
    }

    // 分割字符串，获取各个卡牌名称
    size_t pos = 0;
    std::string cardName;
    std::string str = deckString;
    while ((pos = str.find(";")) != std::string::npos)
    {
        cardName = str.substr(0, pos);
        deck.push_back(getCardByName(cardName));
        str.erase(0, pos + 1);
    }

    // 最后一个卡牌名称
    if (!str.empty())
    {
        deck.push_back(getCardByName(str));
    }

    return deck;
}

// 根据名称获取卡牌
Card GameSaveManager::getCardByName(const std::string& cardName)
{
    // 使用CardLibrary的方法获取卡牌
    return CardLibrary::getCardByName(cardName);
}


// 实现地图数据序列化和反序列化
std::string GameSaveManager::serializeMapInfo(const std::vector<std::vector<MyGame::RoomInfo>>& mapInfo)
{
    std::stringstream ss;

    // 记录有多少层
    ss << mapInfo.size() << ";";

    for (const auto& layer : mapInfo) {
        // 记录这一层有多少个房间
        ss << layer.size() << ";";

        for (const auto& room : layer) {
            // 记录房间类型和位置
            ss << static_cast<int>(room.type) << ","
                << room.position.x << ","
                << room.position.y << ";";
        }
    }

    return ss.str();
}

std::vector<std::vector<MyGame::RoomInfo>> GameSaveManager::deserializeMapInfo(const std::string& mapInfoString)
{
    std::vector<std::vector<MyGame::RoomInfo>> result;
    std::stringstream ss(mapInfoString);
    std::string item;

    // 读取层数
    if (!std::getline(ss, item, ';')) {
        return result;
    }

    int layerCount = std::stoi(item);
    result.resize(layerCount);

    for (int i = 0; i < layerCount; ++i) {
        // 读取该层房间数
        if (!std::getline(ss, item, ';')) {
            break;
        }

        int roomCount = std::stoi(item);

        for (int j = 0; j < roomCount; ++j) {
            // 读取房间信息
            if (!std::getline(ss, item, ';')) {
                break;
            }

            std::stringstream roomStream(item);
            std::string roomItem;

            // 读取房间类型
            if (!std::getline(roomStream, roomItem, ',')) {
                continue;
            }
            int roomType = std::stoi(roomItem);

            // 读取x坐标
            if (!std::getline(roomStream, roomItem, ',')) {
                continue;
            }
            float x = std::stof(roomItem);

            // 读取y坐标
            if (!std::getline(roomStream, roomItem, ',')) {
                continue;
            }
            float y = std::stof(roomItem);

            // 创建RoomInfo并添加到结果中
            MyGame::RoomInfo info;
            info.type = static_cast<MyGame::RoomType>(roomType);
            info.position.set(x, y);

            result[i].push_back(info);
        }
    }

    return result;
}

std::string GameSaveManager::serializeConnections(const std::vector<MyGame::ConnectionInfo>& connections)
{
    std::stringstream ss;

    // 记录连接数量
    ss << connections.size() << ";";

    for (const auto& conn : connections) {
        // 记录起点和终点坐标
        ss << conn.start.x << "," << conn.start.y << ","
            << conn.end.x << "," << conn.end.y << ";";
    }

    return ss.str();
}

std::vector<MyGame::ConnectionInfo> GameSaveManager::deserializeConnections(const std::string& connectionsString)
{
    std::vector<MyGame::ConnectionInfo> result;
    std::stringstream ss(connectionsString);
    std::string item;

    // 读取连接数量
    if (!std::getline(ss, item, ';')) {
        return result;
    }

    int connectionCount = std::stoi(item);
    result.resize(connectionCount);

    for (int i = 0; i < connectionCount; ++i) {
        // 读取连接信息
        if (!std::getline(ss, item, ';')) {
            break;
        }

        std::stringstream connStream(item);
        std::string connItem;

        // 读取起点x坐标
        if (!std::getline(connStream, connItem, ',')) {
            continue;
        }
        float startX = std::stof(connItem);

        // 读取起点y坐标
        if (!std::getline(connStream, connItem, ',')) {
            continue;
        }
        float startY = std::stof(connItem);

        // 读取终点x坐标
        if (!std::getline(connStream, connItem, ',')) {
            continue;
        }
        float endX = std::stof(connItem);

        // 读取终点y坐标
        if (!std::getline(connStream, connItem, ',')) {
            continue;
        }
        float endY = std::stof(connItem);

        // 创建ConnectionInfo并添加到结果中
        MyGame::ConnectionInfo info;
        info.start.set(startX, startY);
        info.end.set(endX, endY);

        result[i] = info;
    }

    return result;
}

bool GameSaveManager::hasSavedGame()
{
    return UserDefault::getInstance()->getBoolForKey(SAVE_KEY_EXIST, false);
}