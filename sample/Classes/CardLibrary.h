// CardLibrary.h
#ifndef __CARD_LIBRARY_H__
#define __CARD_LIBRARY_H__

#include "cocos2d.h"
#include "Card.h"
#include <vector>
#include <random>

namespace CardLibrary {
    // ��ȡ���п��ÿ���
    std::vector<Card> getAllCards();

    // ��ȡ��ʼ����
    std::vector<Card> getStarterDeck();

    // �������ƻ�ȡ����
    Card getCardByName(const std::string& cardName);

    // ��ȡ�������
    Card getRandomCard();

    // �����ͻ�ȡ����
    std::vector<Card> getCardsByType(Card::Type type);
}

#endif // __CARD_LIBRARY_H__