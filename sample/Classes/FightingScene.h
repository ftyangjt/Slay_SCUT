#ifndef __FIGHTING_SCENE_H__
#define __FIGHTING_SCENE_H__

#include "cocos2d.h"
#include "Card.h"
#include "Hero.h"
#include "Monster.h"
#include "DiscardDeck.h"
#include "DrawDeck.h"

// ��������
const int DOUBLE_CLICK_THRESHOLD = 300;  // ����
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
	void createDrawDeck();// �������ƶѰ�ť
	void createDiscardDeck();// �������ƶѰ�ť
    void updateHealthAndBlockLabels();
    void startPlayerTurn();
    void startMonsterTurn();
    void endTurn();
    void checkBattleEnd();
    void drawCard(); // ��һ����
    void discardCard(int index); // ��һ����
    void shuffleDrawPile(); // ϴ��
    void initializeDrawPile(); // ��ʼ���ƶ�
    void createHealthLabels();
    void createBlockLabels();
    void createBuffLabels();
    void updateBuffLabels();
    void updateHandDisplay();
    void addCardEffectLabel(cocos2d::Sprite* cardSprite, const std::string& effect);
    void playCard(int index);
    void highlightSelectedCard();
    void handleCardTap(size_t cardIndex, cocos2d::Touch* touch); // ȷ��ʹ����ȷ������
    void applyCardEffects(const Card& card);
    void FightingScene::applyEffects(int& damage, int& block, const std::vector<std::shared_ptr<Effect>>& effects, const Card::Type cardType, bool isTargetMonster);
    void createTurnCountLabel();
	void goToDrawDeck(Ref* sender);// ǰ�����ƶ�
	void goToDiscardDeck(Ref* sender);// ǰ�����ƶ�
    cocos2d::Size _visibleSize;
    cocos2d::Vec2 _origin;
    bool _isCooldown = false; // ��ʾ�����Ƿ�����ȴ״̬
    int _selectedCardIndex = -1; // -1 ��ʾû��ѡ�еĿ���
    std::vector<Card> _drawPile; // ���ƶ�
    std::vector<Card> _discardPile; // ���ƶ�
    bool _isPlayerTurn; // �Ƿ�����һغ�
    int _turnCount; // �غ���
    std::vector<Card> _cards; // ����
    cocos2d::Sprite* _selectedCard; // ѡ�еĿ���
    Hero* _hero; // Ӣ�۶���
    Monster* _monster; // �������
    cocos2d::Label* _heroHealthLabel; // ��ʾӢ��Ѫ���ı�ǩ
    cocos2d::Label* _monsterHealthLabel; // ��ʾ����Ѫ���ı�ǩ
    cocos2d::Label* _heroBlockLabel; // ��ʾӢ�۸񵲵ı�ǩ
    cocos2d::Label* _monsterBlockLabel; // ��ʾ����񵲵ı�ǩ
    std::vector<cocos2d::Sprite*> _cardSprites; // ���ƾ���
    std::vector<std::chrono::steady_clock::time_point> _lastClickTimes; // ��¼ÿ�ſ��Ƶ��ϴε��ʱ��
    cocos2d::Label* _turnCountLabel; // �����غ�����ǩ
    cocos2d::Label* _heroBuffLabel;    // Ӣ�� BUFF ��ǩ
    cocos2d::Label* _monsterBuffLabel; // ���� BUFF ��ǩ
};

#endif // __FIGHTING_SCENE_H__
