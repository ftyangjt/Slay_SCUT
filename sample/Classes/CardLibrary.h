// CardLibrary.h
#ifndef __CARD_LIBRARY_H__
#define __CARD_LIBRARY_H__

#include "cocos2d.h"
#include "Card.h"
#include <vector>
#include <random>

namespace CardLibrary {
    // 获取所有可用卡牌
    std::vector<Card> getAllCards();

    // 获取初始卡组
    std::vector<Card> getStarterDeck();

    // 根据名称获取卡牌
    Card getCardByName(const std::string& cardName);

    // 获取随机卡牌
    Card getRandomCard();

    // 按类型获取卡牌
    std::vector<Card> getCardsByType(Card::Type type);
}

#endif // __CARD_LIBRARY_H__