// GameSaveManager.h
#ifndef __GAME_SAVE_MANAGER_H__
#define __GAME_SAVE_MANAGER_H__

#include "cocos2d.h"
#include "Card.h"
#include "Map.h" // ���� Map.h ��ȡ RoomInfo �� ConnectionInfo ����
#include <vector>
#include <string>

class GameSaveManager
{
public:
    // ��չ���е� saveGame ��������ӵ�ͼ״̬����
    static bool saveGame(
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
    );

    // ��չ loadGame ���������ص�ͼ״̬
    static bool loadGame(
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
    );

    static Card getCardByName(const std::string& cardName);
    static bool hasSavedGame();
    static bool deleteSavedGame();

private:
    static std::string serializeDeck(const std::vector<Card>& deck);
    static std::vector<Card> deserializeDeck(const std::string& deckString);

    // ������л��ͷ����л���ͼ���ݵĸ�������
    static std::string serializeMapInfo(const std::vector<std::vector<MyGame::RoomInfo>>& mapInfo);
    static std::vector<std::vector<MyGame::RoomInfo>> deserializeMapInfo(const std::string& mapInfoString);

    static std::string serializeConnections(const std::vector<MyGame::ConnectionInfo>& connections);
    static std::vector<MyGame::ConnectionInfo> deserializeConnections(const std::string& connectionsString);
};

#endif // __GAME_SAVE_MANAGER_H__
