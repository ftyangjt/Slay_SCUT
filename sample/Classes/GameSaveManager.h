// GameSaveManager.h
#ifndef __GAME_SAVE_MANAGER_H__
#define __GAME_SAVE_MANAGER_H__

#include "cocos2d.h"
#include "Card.h"
#include <vector>
#include <string>

class GameSaveManager
{
public:
    static bool saveGame(int health, int gold, const std::vector<Card>& deck, int currentMapId, int currentLevel);
    static bool loadGame(int& health, int& gold, std::vector<Card>& deck, int& currentMapId, int& currentLevel);
    static Card getCardByName(const std::string& cardName);
    static bool hasSavedGame();
    static bool deleteSavedGame();

private:
    static std::string serializeDeck(const std::vector<Card>& deck);
    static std::vector<Card> deserializeDeck(const std::string& deckString);
};

#endif // __GAME_SAVE_MANAGER_H__
