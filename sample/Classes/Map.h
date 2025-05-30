// Map.h
#ifndef __MAP_H__
#define __MAP_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"  // ��� ScrollView ������
#include <vector>

namespace MyGame {
    // ���������Ϸ״̬�ĺ�������
    void resetGameState();

    enum class RoomType {
        START,  // ��� START ����
        BATTLE,
        QUESTION,
        REST,
        BOSS,// ��� BOSS ����
        ELITE,
        SHOP
    };
    // ����������ʼ��
    extern RoomType currentRoomType;

    // �� RoomInfo �� ConnectionInfo �Ƶ�����
    struct RoomInfo {
        RoomType type;
        cocos2d::Vec2 position;
    };

    // ������Ϣ�ṹ��
    struct ConnectionInfo {
        cocos2d::Vec2 start;
        cocos2d::Vec2 end;
    };

    struct Room {
        RoomType type;
        cocos2d::MenuItemImage* item;
    };

    // ������̬����
    extern std::vector<std::vector<RoomInfo>> staticMapInfo;
    extern std::vector<ConnectionInfo> staticConnectionInfo;
    extern int maxAccessibleLayer;
    extern cocos2d::Vec2 currentRoomPosition;
    extern bool roomCompleted;

    class Map : public cocos2d::Scene
    {
    public:
        static cocos2d::Scene* createScene();
        virtual bool init();
        CREATE_FUNC(Map);
        static int currentLayer;
        static int currentRoom;

    private:
        void menuBattleCallback(cocos2d::Ref* pSender);
        void menuQuestionCallback(cocos2d::Ref* pSender);
        void menuRestCallback(cocos2d::Ref* pSender);
        void menuBossCallback(cocos2d::Ref* pSender);  // ��� BOSS �ص�����
        void menuEliteCallback(cocos2d::Ref* pSender); // ��� ELITE �ص�����
        void menuShopCallback(cocos2d::Ref* pSender); // ��� SHOP �ص�����
        RoomType getRandomRoomType();
        void generateRandomMap(int layers, int roomsPerLayer);
        void generateMapFromSavedInfo();
        void connectRooms(const std::vector<std::vector<Room>>& map);
        void saveGame();
        void createSaveButton();

        // �����ķ���
        void createLayerLabels();    // �����㼶��ǩ
        void updateRoomAccessibility(); // ���·���ɷ�����

        std::vector<std::vector<Room>> mapLayers;
        cocos2d::ui::ScrollView* scrollView;  // ��� ScrollView ��Ա����
        
    };

} // namespace MyGame

#endif // __MAP_H__
