#ifndef __DRAW_DECK_H__
#define __DRAW_DECK_H__
#include "cocos2d.h"
#include "Card.h"
#include <vector>

class DrawDeck : public cocos2d::Scene
{
public:
    // 提供一个接收 drawPile 数据的创建函数
    static cocos2d::Scene* createScene(const std::vector<Card>& drawPile);
    
    // 改为接受参数的初始化函数
    bool init(const std::vector<Card>& drawPile);
    
    // 自定义 create 函数
    static DrawDeck* create(const std::vector<Card>& drawPile);

private:
    void createBackground();
    void showDeck();
    void createBackButton();
    void addCardEffectLabel(cocos2d::Sprite* cardSprite, const std::string& effect);
    void backToFightingScene(cocos2d::Ref* sender);

    std::vector<Card> _drawPile;

    std::vector<cocos2d::Sprite*> _cardSprites;
};

#endif // __DRAW_DECK_H__