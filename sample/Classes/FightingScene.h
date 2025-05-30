#ifndef __FIGHTING_SCENE_H__
#define __FIGHTING_SCENE_H__

#include "cocos2d.h"
#include "Card.h"
#include "Hero.h"
#include "Monster.h"
#include "DiscardDeck.h"
#include "DrawDeck.h"
#include "ui/CocosGUI.h"
#include "CardLibrary.h"

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
	void exhaustCard(int index); // 消耗一张牌
    void shuffleDrawPile(); // 洗牌
    void initializeDrawPile(); // 初始化牌堆
	void createCostLabel();
    void createHealthLabels();
    void createBlockLabels();
    void createBuffLabels();
    void updateBuffLabels();
    void updateHandDisplay();
    cocos2d::Label* addCardEffectLabel(cocos2d::Sprite* cardSprite, const std::string& effect);
    void playCard(int index);
    void highlightSelectedCard();
    void handleCardTap(size_t cardIndex, cocos2d::Touch* touch); // 确保使用正确的类型
    void applyCardEffects(const Card& card);
    void FightingScene::applyEffects(int& damage, int& block, const std::vector<std::shared_ptr<Effect>>& effects, const Card::Type cardType, bool isTargetMonster);
    void createTurnCountLabel();
	void goToDrawDeck(Ref* sender);// 前往抽牌堆
	void goToDiscardDeck(Ref* sender);// 前往弃牌堆
    void playDiscardToDrawMeteorEffect(const cocos2d::Vec2& discardPilePosition, const cocos2d::Vec2& drawPilePosition);
    void drawSequentialCards(int count);
    void updateCostLabel();
    void playHeroAttackAnimation();
    void playMonsterAttackAnimation();
    void playHeroHitAnimation();
    void playMonsterHitAnimation();
    std::vector<Card> generateRandomCards(int count);
    void showCardSelectionWithCallback(const std::vector<Card>& cards, const std::function<void()>& onSelectionComplete);
    void createMonsterIntentLabel();      // 创建怪物意图标签
    void updateMonsterIntentDisplay();    // 更新怪物意图显示
    void applyHoverEffect(int cardIndex);
    void resetHoverEffect(int cardIndex);

    cocos2d::Size _visibleSize;
    cocos2d::Vec2 _origin;
<<<<<<< Updated upstream
=======
    bool _isSelectingCard = false; // 是否处于选牌界面

private:
    // 原有变量保持不变
    int _hoveringCardIndex = -1; // 当前悬停卡牌的索引
    std::map<int, bool> _cardIsHovering; // 标记卡牌是否正在悬浮中
    std::map<int, cocos2d::Vec2> _cardOriginalPositions; // 保存卡牌原始位置
    std::map<int, float> _cardOriginalRotations; // 保存卡牌原始旋转
    std::map<int, int> _cardOriginalZOrders; // 保存卡牌原始Z顺序

>>>>>>> Stashed changes
    bool _isSelectingCard = false; // 是否处于选牌界面

private:
    int _hoveringCardIndex = -1; // 当前悬停卡牌的索引
    std::map<int, bool> _cardIsHovering; // 标记卡牌是否正在悬浮中
    std::set<int> _disabledCardIndices; // 存储当前不可交互的卡牌索引
    std::map<int, cocos2d::Vec2> _cardOriginalPositions; // 保存卡牌原始位置
    std::map<int, float> _cardOriginalRotations; // 保存卡牌原始旋转
    std::map<int, int> _cardOriginalZOrders; // 保存卡牌原始Z顺序
    bool _isCooldown = false; // 表示出牌是否处于冷却状态
	bool _isEndTurnButtonEnabled = true; //表示结束回合按钮是否可用
	bool _isDrawingCards = false; // 是否正在抽牌
    int _selectedCardIndex = -1; // -1 表示没有选中的卡牌
    int _currentCost;         // 当前可用费用
    std::vector<Card> _drawPile; // 抽牌堆
    std::vector<Card> _discardPile; // 弃牌堆
    std::vector<Card> _exhaustPile; // 消耗牌堆
    bool _isPlayerTurn; // 是否是玩家回合
    int _turnCount; // 回合数
    std::vector<Card> _cards; // 手牌
    cocos2d::Sprite* _selectedCard; // 选中的卡牌

    Hero* _hero; // 英雄对象
    Monster* _monster; // 怪物对象
   
    cocos2d::Label* _costLabel; // 显示当前费用的UI
    cocos2d::Label* _heroHealthLabel; // 显示英雄血量的标签
    cocos2d::Label* _monsterHealthLabel; // 显示怪物血量的标签
    cocos2d::Label* _heroBlockLabel; // 显示英雄格挡的标签
    cocos2d::Label* _monsterBlockLabel; // 显示怪物格挡的标签
    std::vector<cocos2d::Sprite*> _cardSprites; // 卡牌精灵
    std::vector<std::chrono::steady_clock::time_point> _lastClickTimes; // 记录每张卡牌的上次点击时间
    cocos2d::Label* _turnCountLabel; // 声明回合数标签
    cocos2d::MenuItemImage* _discardDeckButton;
	cocos2d::MenuItemImage* _drawDeckButton;
    cocos2d::Label* _heroBuffLabel;    // 英雄 BUFF 标签
    cocos2d::Label* _monsterBuffLabel; // 怪物 BUFF 标签
    cocos2d::ui::Button* _endTurnButton = nullptr; // 结束回合按钮
    cocos2d::Label* _monsterIntentLabel;  // 怪物意图标签
};

#endif // __FIGHTING_SCENE_H__
