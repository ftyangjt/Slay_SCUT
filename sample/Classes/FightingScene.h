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
    void updateHealthAndBlockLabels();
    void startPlayerTurn();
    void startMonsterTurn();
    void endTurn();
    void checkBattleEnd();
    void drawCard(); // 抽一张牌
    void discardCard(int index); // 弃一张牌
    void shuffleDrawPile(); // 洗牌
    void initializeDrawPile(); // 初始化牌堆
    void createHealthLabels();
    void createBlockLabels();
    void updateHandDisplay();
    void addCardEffectLabel(cocos2d::Sprite* cardSprite, const std::string& effect);
    void playCard(int index);
    void highlightSelectedCard();

    int _selectedCardIndex = -1; // -1 表示没有选中的卡牌
    std::vector<Card> _drawPile; // 抽牌堆
    std::vector<Card> _discardPile; // 弃牌堆
    bool _isPlayerTurn; // 是否是玩家回合
    int _turnCount; // 回合数
    std::vector<Card> _cards; // 手牌
    cocos2d::Sprite* _selectedCard; // 选中的卡牌
    Hero* _hero; // 英雄对象
    Monster* _monster; // 怪物对象
    cocos2d::Label* _heroHealthLabel; // 显示英雄血量的标签
    cocos2d::Label* _monsterHealthLabel; // 显示怪物血量的标签
    cocos2d::Label* _heroBlockLabel; // 显示英雄格挡的标签
    cocos2d::Label* _monsterBlockLabel; // 显示怪物格挡的标签
    std::vector<cocos2d::Sprite*> _cardSprites; // 卡牌精灵
    std::vector<std::chrono::steady_clock::time_point> _lastClickTimes; // 记录每张卡牌的上次点击时间
};

#endif // __FIGHTING_SCENE_H__