#ifndef __FIGHTING_SCENE_H__
#define __FIGHTING_SCENE_H__

#include "cocos2d.h"
#include "Card.h"
#include "Hero.h"
#include "Monster.h"

class FightingScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(FightingScene);

private:
    void createBackground();
    void createCharacters();
    void updateHandDisplay(); // 更新手牌显示状态
    bool onCardTouchBegan(cocos2d::Sprite* cardSprite, int index, cocos2d::Touch* touch, cocos2d::Event* event);
    void selectCard(cocos2d::Sprite* card, int index);
    void playCard(int index);
    void updateHealthLabels();
    void startPlayerTurn();
    void startMonsterTurn();
    void endTurn();
    void checkBattleEnd();
    void resetCards(int numCards = 5);
    void drawCard(); // 抽一张牌
    void discardCard(int index); // 弃一张牌
    void shuffleDrawPile(); // 洗牌
    void initializeDrawPile(); // 初始化抽牌堆
    void endPlayerTurn();
    void removeExistingCards();
    void displayDrawnCards(int numCards, const cocos2d::Size& visibleSize, const cocos2d::Vec2& origin);
    void setupCardSprite(cocos2d::Sprite* cardSprite, int index, float cardWidth, float cardHeight, const cocos2d::Vec2& origin);
    void addCardEffectLabel(cocos2d::Sprite* cardSprite, const std::string& effect);
    void addCardTouchListener(cocos2d::Sprite* cardSprite, int index);

    std::vector<Card> _drawPile; // 抽牌堆
    std::vector<Card> _discardPile; // 弃牌堆
    bool _isPlayerTurn;
    int _turnCount;
    std::vector<Card> _cards;
    cocos2d::Sprite* _selectedCard;
    int _selectedCardIndex;
    Hero* _hero; // 添加Hero对象
    Monster* _monster; // 添加Monster对象
    cocos2d::Label* _heroHealthLabel; // 添加显示英雄血量的标签
    cocos2d::Label* _monsterHealthLabel; // 添加显示怪物血量的标签
};

#endif // __FIGHTING_SCENE_H__

