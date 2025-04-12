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
        ELITE
    };

    struct Room {
        RoomType type;
        cocos2d::MenuItemImage* item;
    };

    // ������Ϣ�ṹ��
    struct ConnectionInfo {
        cocos2d::Vec2 start;
        cocos2d::Vec2 end;
    };

    class Map : public cocos2d::Scene
    {
    public:
        static cocos2d::Scene* createScene();
        virtual bool init();
        CREATE_FUNC(Map);

    private:
        void menuBattleCallback(cocos2d::Ref* pSender);
        void menuQuestionCallback(cocos2d::Ref* pSender);
        void menuRestCallback(cocos2d::Ref* pSender);
        void menuBossCallback(cocos2d::Ref* pSender);  // ��� BOSS �ص�����
		void menuEliteCallback(cocos2d::Ref* pSender); // ��� ELITE �ص�����
        RoomType getRandomRoomType();
        void generateRandomMap(int layers, int roomsPerLayer);
        void generateMapFromSavedInfo();
        void connectRooms(const std::vector<std::vector<Room>>& map);

        // �����ķ���
        void createLayerLabels();    // �����㼶��ǩ
        void updateRoomAccessibility(); // ���·���ɷ�����

        std::vector<std::vector<Room>> mapLayers;
        cocos2d::ui::ScrollView* scrollView;  // ��� ScrollView ��Ա����
        int currentLayer;  // ��ǰ��
        int currentRoom;  // ��ǰ����
    };

} // namespace MyGame

#endif // __MAP_H__
