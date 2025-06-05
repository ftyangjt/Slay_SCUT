// GameSaveManager.cpp
#include "GameSaveManager.h"
#include "CardLibrary.h"
#include "Card.h"
#include "base/CCUserDefault.h"
#include <sstream>

USING_NS_CC;

// �浵��
const char* SAVE_KEY_EXIST = "save_exist";
const char* SAVE_KEY_HEALTH = "save_health";
const char* SAVE_KEY_GOLD = "save_gold";
const char* SAVE_KEY_DECK = "save_deck";
const char* SAVE_KEY_MAP_ID = "save_map_id";
const char* SAVE_KEY_ROOM = "save_room";
// ������ֵ
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
    // ��ȡUserDefaultʵ��
    auto userDefault = UserDefault::getInstance();

    // ���л�����
    std::string deckString = serializeDeck(deck);

    // ���л���ͼ����
    std::string mapInfoString = serializeMapInfo(mapInfo);
    std::string connectionString = serializeConnections(connectionInfo);

    // ��������
    userDefault->setBoolForKey(SAVE_KEY_EXIST, true);
    userDefault->setIntegerForKey(SAVE_KEY_HEALTH, health);
    userDefault->setIntegerForKey(SAVE_KEY_GOLD, gold);
    userDefault->setStringForKey(SAVE_KEY_DECK, deckString);
    userDefault->setIntegerForKey(SAVE_KEY_MAP_ID, currentMapId);
    userDefault->setIntegerForKey(SAVE_KEY_ROOM, currentRoom);

    // ���������ĵ�ͼ����
    userDefault->setStringForKey(SAVE_KEY_MAP_INFO, mapInfoString);
    userDefault->setStringForKey(SAVE_KEY_CONNECTIONS, connectionString);
    userDefault->setIntegerForKey(SAVE_KEY_MAX_LAYER, maxLayer);
    userDefault->setFloatForKey(SAVE_KEY_ROOM_POS_X, currentRoomPos.x);
    userDefault->setFloatForKey(SAVE_KEY_ROOM_POS_Y, currentRoomPos.y);
    userDefault->setIntegerForKey(SAVE_KEY_ROOM_TYPE, static_cast<int>(roomType));
    userDefault->setBoolForKey(SAVE_KEY_COMPLETED, completed);

    // ȷ�����ݱ�д��
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
    // ��ȡUserDefaultʵ��
    auto userDefault = UserDefault::getInstance();

    // ����Ƿ��д浵
    if (!userDefault->getBoolForKey(SAVE_KEY_EXIST, false))
    {
        CCLOG("No saved game found");
        return false;
    }

    // ��������
    health = userDefault->getIntegerForKey(SAVE_KEY_HEALTH, 100);
    gold = userDefault->getIntegerForKey(SAVE_KEY_GOLD, 0);
    std::string deckString = userDefault->getStringForKey(SAVE_KEY_DECK, "");
    currentMapId = userDefault->getIntegerForKey(SAVE_KEY_MAP_ID, 0);
    currentRoom = userDefault->getIntegerForKey(SAVE_KEY_ROOM, 0);

    // ���������ĵ�ͼ����
    std::string mapInfoString = userDefault->getStringForKey(SAVE_KEY_MAP_INFO, "");
    std::string connectionString = userDefault->getStringForKey(SAVE_KEY_CONNECTIONS, "");
    maxLayer = userDefault->getIntegerForKey(SAVE_KEY_MAX_LAYER, 0);
    float posX = userDefault->getFloatForKey(SAVE_KEY_ROOM_POS_X, 0.0f);
    float posY = userDefault->getFloatForKey(SAVE_KEY_ROOM_POS_Y, 0.0f);
    currentRoomPos.set(posX, posY);
    int roomTypeInt = userDefault->getIntegerForKey(SAVE_KEY_ROOM_TYPE, 0);
    roomType = static_cast<MyGame::RoomType>(roomTypeInt);
    completed = userDefault->getBoolForKey(SAVE_KEY_COMPLETED, false);

    // �����л�����
    deck = deserializeDeck(deckString);

    // �����л���ͼ����
    if (!mapInfoString.empty()) {
        mapInfo = deserializeMapInfo(mapInfoString);
    }

    if (!connectionString.empty()) {
        connectionInfo = deserializeConnections(connectionString);
    }

    CCLOG("Game loaded successfully");
    return true;
}

// �������л��ͷ����л��������ֲ���
// ���л�����
std::string GameSaveManager::serializeDeck(const std::vector<Card>& deck)
{
    std::string result = "";
    for (size_t i = 0; i < deck.size(); ++i)
    {
        // ֻ���濨�����ƣ��÷ֺŷָ�
        result += deck[i].getName();
        if (i < deck.size() - 1)
        {
            result += ";";
        }
    }
    return result;
}

// �����л�����
std::vector<Card> GameSaveManager::deserializeDeck(const std::string& deckString)
{
    std::vector<Card> deck;
    if (deckString.empty())
    {
        return deck;
    }

    // �ָ��ַ�������ȡ������������
    size_t pos = 0;
    std::string cardName;
    std::string str = deckString;
    while ((pos = str.find(";")) != std::string::npos)
    {
        cardName = str.substr(0, pos);
        deck.push_back(getCardByName(cardName));
        str.erase(0, pos + 1);
    }

    // ���һ����������
    if (!str.empty())
    {
        deck.push_back(getCardByName(str));
    }

    return deck;
}

// �������ƻ�ȡ����
Card GameSaveManager::getCardByName(const std::string& cardName)
{
    // ʹ��CardLibrary�ķ�����ȡ����
    return CardLibrary::getCardByName(cardName);
}


// ʵ�ֵ�ͼ�������л��ͷ����л�
std::string GameSaveManager::serializeMapInfo(const std::vector<std::vector<MyGame::RoomInfo>>& mapInfo)
{
    std::stringstream ss;

    // ��¼�ж��ٲ�
    ss << mapInfo.size() << ";";

    for (const auto& layer : mapInfo) {
        // ��¼��һ���ж��ٸ�����
        ss << layer.size() << ";";

        for (const auto& room : layer) {
            // ��¼�������ͺ�λ��
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

    // ��ȡ����
    if (!std::getline(ss, item, ';')) {
        return result;
    }

    int layerCount = std::stoi(item);
    result.resize(layerCount);

    for (int i = 0; i < layerCount; ++i) {
        // ��ȡ�ò㷿����
        if (!std::getline(ss, item, ';')) {
            break;
        }

        int roomCount = std::stoi(item);

        for (int j = 0; j < roomCount; ++j) {
            // ��ȡ������Ϣ
            if (!std::getline(ss, item, ';')) {
                break;
            }

            std::stringstream roomStream(item);
            std::string roomItem;

            // ��ȡ��������
            if (!std::getline(roomStream, roomItem, ',')) {
                continue;
            }
            int roomType = std::stoi(roomItem);

            // ��ȡx����
            if (!std::getline(roomStream, roomItem, ',')) {
                continue;
            }
            float x = std::stof(roomItem);

            // ��ȡy����
            if (!std::getline(roomStream, roomItem, ',')) {
                continue;
            }
            float y = std::stof(roomItem);

            // ����RoomInfo����ӵ������
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

    // ��¼��������
    ss << connections.size() << ";";

    for (const auto& conn : connections) {
        // ��¼�����յ�����
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

    // ��ȡ��������
    if (!std::getline(ss, item, ';')) {
        return result;
    }

    int connectionCount = std::stoi(item);
    result.resize(connectionCount);

    for (int i = 0; i < connectionCount; ++i) {
        // ��ȡ������Ϣ
        if (!std::getline(ss, item, ';')) {
            break;
        }

        std::stringstream connStream(item);
        std::string connItem;

        // ��ȡ���x����
        if (!std::getline(connStream, connItem, ',')) {
            continue;
        }
        float startX = std::stof(connItem);

        // ��ȡ���y����
        if (!std::getline(connStream, connItem, ',')) {
            continue;
        }
        float startY = std::stof(connItem);

        // ��ȡ�յ�x����
        if (!std::getline(connStream, connItem, ',')) {
            continue;
        }
        float endX = std::stof(connItem);

        // ��ȡ�յ�y����
        if (!std::getline(connStream, connItem, ',')) {
            continue;
        }
        float endY = std::stof(connItem);

        // ����ConnectionInfo����ӵ������
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