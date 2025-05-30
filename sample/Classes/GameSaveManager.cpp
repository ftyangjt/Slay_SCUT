// GameSaveManager.cpp
#include "GameSaveManager.h"
#include "CardLibrary.h"
#include "Card.h"
#include "base/CCUserDefault.h"

USING_NS_CC;

// 存档键
const char* SAVE_KEY_EXIST = "save_exist";
const char* SAVE_KEY_HEALTH = "save_health";
const char* SAVE_KEY_GOLD = "save_gold";
const char* SAVE_KEY_DECK = "save_deck";
const char* SAVE_KEY_MAP_ID = "save_map_id";
const char* SAVE_KEY_LEVEL = "save_level";

// 存储游戏状态
bool GameSaveManager::saveGame(int health, int gold, const std::vector<Card>& deck, int currentMapId, int currentLevel)
{
    // 获取UserDefault实例
    auto userDefault = UserDefault::getInstance();

    // 序列化卡组
    std::string deckString = serializeDeck(deck);

    // 保存数据
    userDefault->setBoolForKey(SAVE_KEY_EXIST, true);
    userDefault->setIntegerForKey(SAVE_KEY_HEALTH, health);
    userDefault->setIntegerForKey(SAVE_KEY_GOLD, gold);
    userDefault->setStringForKey(SAVE_KEY_DECK, deckString);
    userDefault->setIntegerForKey(SAVE_KEY_MAP_ID, currentMapId);
    userDefault->setIntegerForKey(SAVE_KEY_LEVEL, currentLevel);

    // 确保数据被写入
    userDefault->flush();

    CCLOG("Game saved successfully");
    return true;
}

// 加载游戏状态
bool GameSaveManager::loadGame(int& health, int& gold, std::vector<Card>& deck, int& currentMapId, int& currentLevel)
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
    currentLevel = userDefault->getIntegerForKey(SAVE_KEY_LEVEL, 1);

    // 反序列化卡组
    deck = deserializeDeck(deckString);

    CCLOG("Game loaded successfully");
    return true;
}

// 检查是否有存档
bool GameSaveManager::hasSavedGame()
{
    return UserDefault::getInstance()->getBoolForKey(SAVE_KEY_EXIST, false);
}

// 删除存档
bool GameSaveManager::deleteSavedGame()
{
    auto userDefault = UserDefault::getInstance();
    userDefault->setBoolForKey(SAVE_KEY_EXIST, false);
    userDefault->flush();
    CCLOG("Saved game deleted");
    return true;
}

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
