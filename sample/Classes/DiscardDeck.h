#ifndef __DISCARD_DECK_H__
#define __DISCARD_DECK_H__

#include "cocos2d.h"
#include "Card.h"
#include <vector>

class DiscardDeck : public cocos2d::Scene
{
public:
    // 接收弃牌堆数据创建场景
    static cocos2d::Scene* createScene(const std::vector<Card>& discardPile);

    // 初始化函数，传入弃牌堆容器
    bool init(const std::vector<Card>& discardPile);

    // 自定义创建函数
    static DiscardDeck* create(const std::vector<Card>& discardPile);

private:
    void createBackground();
    void showDeck();
    void createBackButton();
    void addCardEffectLabel(cocos2d::Sprite* cardSprite, const std::string& effect);
    void backToFightingScene(cocos2d::Ref* sender);

    std::vector<Card> _discardPile;
    std::vector<cocos2d::Sprite*> _cardSprites;
};

#endif // __DISCARD_DECK_H__