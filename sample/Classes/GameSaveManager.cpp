// GameSaveManager.cpp
#include "GameSaveManager.h"
#include "CardLibrary.h"
#include "Card.h"
#include "base/CCUserDefault.h"

USING_NS_CC;

// �浵��
const char* SAVE_KEY_EXIST = "save_exist";
const char* SAVE_KEY_HEALTH = "save_health";
const char* SAVE_KEY_GOLD = "save_gold";
const char* SAVE_KEY_DECK = "save_deck";
const char* SAVE_KEY_MAP_ID = "save_map_id";
const char* SAVE_KEY_LEVEL = "save_level";

// �洢��Ϸ״̬
bool GameSaveManager::saveGame(int health, int gold, const std::vector<Card>& deck, int currentMapId, int currentLevel)
{
    // ��ȡUserDefaultʵ��
    auto userDefault = UserDefault::getInstance();

    // ���л�����
    std::string deckString = serializeDeck(deck);

    // ��������
    userDefault->setBoolForKey(SAVE_KEY_EXIST, true);
    userDefault->setIntegerForKey(SAVE_KEY_HEALTH, health);
    userDefault->setIntegerForKey(SAVE_KEY_GOLD, gold);
    userDefault->setStringForKey(SAVE_KEY_DECK, deckString);
    userDefault->setIntegerForKey(SAVE_KEY_MAP_ID, currentMapId);
    userDefault->setIntegerForKey(SAVE_KEY_LEVEL, currentLevel);

    // ȷ�����ݱ�д��
    userDefault->flush();

    CCLOG("Game saved successfully");
    return true;
}

// ������Ϸ״̬
bool GameSaveManager::loadGame(int& health, int& gold, std::vector<Card>& deck, int& currentMapId, int& currentLevel)
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
    currentLevel = userDefault->getIntegerForKey(SAVE_KEY_LEVEL, 1);

    // �����л�����
    deck = deserializeDeck(deckString);

    CCLOG("Game loaded successfully");
    return true;
}

// ����Ƿ��д浵
bool GameSaveManager::hasSavedGame()
{
    return UserDefault::getInstance()->getBoolForKey(SAVE_KEY_EXIST, false);
}

// ɾ���浵
bool GameSaveManager::deleteSavedGame()
{
    auto userDefault = UserDefault::getInstance();
    userDefault->setBoolForKey(SAVE_KEY_EXIST, false);
    userDefault->flush();
    CCLOG("Saved game deleted");
    return true;
}

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
