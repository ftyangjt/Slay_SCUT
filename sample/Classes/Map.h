// Map.h
#ifndef __MAP_H__
#define __MAP_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"  // 添加 ScrollView 的引用
#include <vector>

namespace MyGame {
    // 添加重置游戏状态的函数声明
    void resetGameState();

    enum class RoomType {
        START,  // 添加 START 类型
        BATTLE,
        QUESTION,
        REST,
        BOSS,// 添加 BOSS 类型
        ELITE,
        SHOP
    };
    // 仅声明不初始化
    extern RoomType currentRoomType;

    // 将 RoomInfo 和 ConnectionInfo 移到这里
    struct RoomInfo {
        RoomType type;
        cocos2d::Vec2 position;
    };

    // 连线信息结构体
    struct ConnectionInfo {
        cocos2d::Vec2 start;
        cocos2d::Vec2 end;
    };

    struct Room {
        RoomType type;
        cocos2d::MenuItemImage* item;
    };

    // 声明静态变量
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
        void menuBossCallback(cocos2d::Ref* pSender);  // 添加 BOSS 回调函数
        void menuEliteCallback(cocos2d::Ref* pSender); // 添加 ELITE 回调函数
        void menuShopCallback(cocos2d::Ref* pSender); // 添加 SHOP 回调函数
        RoomType getRandomRoomType();
        void generateRandomMap(int layers, int roomsPerLayer);
        void generateMapFromSavedInfo();
        void connectRooms(const std::vector<std::vector<Room>>& map);
        void saveGame();
        void createSaveButton();

        // 新增的方法
        void createLayerLabels();    // 创建层级标签
        void updateRoomAccessibility(); // 更新房间可访问性

        std::vector<std::vector<Room>> mapLayers;
        cocos2d::ui::ScrollView* scrollView;  // 添加 ScrollView 成员变量
        
    };

} // namespace MyGame

#endif // __MAP_H__
