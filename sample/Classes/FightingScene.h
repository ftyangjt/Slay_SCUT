#ifndef __FIGHTING_SCENE_H__
#define __FIGHTING_SCENE_H__

#include "cocos2d.h"
#include "Card.h"
#include "Hero.h"
#include "Monster.h"
#include "DiscardDeck.h"
#include "DrawDeck.h"

// 常量定义
const int DOUBLE_CLICK_THRESHOLD = 300;  // 毫秒
const float CARD_SCALE = 0.25f;
const float COOLDOWN_TIME = 0.5f;
const float DISCARD_DELAY = 0.1f;

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
	void createDrawDeck();// 创建抽牌堆按钮
	void createDiscardDeck();// 创建弃牌堆按钮
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
    void handleCardTap(size_t cardIndex, cocos2d::Touch* touch); // 确保使用正确的类型
    void applyCardEffects(const Card& card);
    void applyEffects(int& damage, int& block, const std::vector<std::shared_ptr<Effect>>& effects, bool isTargetMonster);
    void createTurnCountLabel();
	void goToDrawDeck(Ref* sender);// 前往抽牌堆
	void goToDiscardDeck(Ref* sender);// 前往弃牌堆
    cocos2d::Size _visibleSize;
    cocos2d::Vec2 _origin;
    bool _isCooldown = false; // 表示出牌是否处于冷却状态
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
    cocos2d::Label* _turnCountLabel; // 声明回合数标签
};

#endif // __FIGHTING_SCENE_H__
