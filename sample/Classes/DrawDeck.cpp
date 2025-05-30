#include "DrawDeck.h"
#include "ui/CocosGUI.h"
#include "FightingScene.h"
#include "Card.h"

USING_NS_CC;

Scene* DrawDeck::createScene(const std::vector<Card>& drawPile)
{
    return DrawDeck::create(drawPile);
}
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in FightingScene.cpp\n");
}
DrawDeck* DrawDeck::create(const std::vector<Card>& drawPile)
{
    DrawDeck* ret = new (std::nothrow) DrawDeck();
    if(ret && ret->init(drawPile))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool DrawDeck::init(const std::vector<Card>& drawPile)
{
    if (!Scene::init())
    {
        return false;
    }
    
    // 保存传入的 drawPile 数据
    _drawPile = drawPile;
    
    createBackground();
    showDeck(); 
    createBackButton(); // 创建返回按钮
    return true;
}

void DrawDeck::createBackground()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto background = Sprite::create("background.png");
    if (background == nullptr)
    {
        problemLoading("'background.png'");
        return;
    }

    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(background, 0);
}

void DrawDeck::addCardEffectLabel(cocos2d::Sprite* cardSprite, const std::string& effect)
{
    auto effectLabel = Label::createWithTTF(effect, "fonts/Marker Felt.ttf", 48);
    effectLabel->setColor(cocos2d::Color3B::BLACK);
    effectLabel->setPosition(Vec2(cardSprite->getContentSize().width / 2, cardSprite->getContentSize().height / 2));
    cardSprite->addChild(effectLabel, 1);
}


void DrawDeck::showDeck()
{
    for (auto cardSprite : _cardSprites)
    {
        this->removeChild(cardSprite);
    }
    _cardSprites.clear();

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto tempCardSprite = Sprite::create("cardBackground.jpg");
    float originalCardWidth = tempCardSprite->getContentSize().width;
    float originalCardHeight = tempCardSprite->getContentSize().height;

    // 设置固定的缩放系数，使卡牌更小
    float cardScale = 0.4f; // 降低为原来的40%大小

    float scaledCardWidth = originalCardWidth * cardScale;
    float totalWidth = _drawPile.size() * scaledCardWidth;

    // 计算卡牌之间的重叠量，使卡牌紧贴
    float cardOverlap = -10.0f; // 负值表示卡牌重叠

    // 如果总宽度超过屏幕宽度，增加重叠量
    if (totalWidth > visibleSize.width)
    {
        // 调整重叠量，使所有卡牌都能显示在屏幕上
        float availableWidth = visibleSize.width - scaledCardWidth;
        float neededWidth = (_drawPile.size() - 1) * (scaledCardWidth + cardOverlap);

        if (neededWidth > availableWidth) {
            cardOverlap = (availableWidth - (_drawPile.size() - 1) * scaledCardWidth) / (_drawPile.size() - 1);
        }
    }

    // 计算起始位置，使卡牌居中显示
    float totalDisplayWidth = scaledCardWidth + (_drawPile.size() - 1) * (scaledCardWidth + cardOverlap);
    float startX = (visibleSize.width - totalDisplayWidth) / 2 + scaledCardWidth / 2;

    // 创建并放置卡牌
    for (size_t i = 0; i < _drawPile.size(); ++i)
    {
        auto cardSprite = Sprite::create("cardBackground.jpg");
        float posX = startX + i * (scaledCardWidth + cardOverlap);
        cardSprite->setPosition(Vec2(posX, origin.y + visibleSize.height / 2));
        cardSprite->setScale(cardScale);
        this->addChild(cardSprite, 1);
        _cardSprites.push_back(cardSprite);

        // 添加卡牌效果标签
        addCardEffectLabel(cardSprite, _drawPile[i].getEffect());

        // 添加费用标签
        auto costLabel = Label::createWithTTF("Cost: " + std::to_string(_drawPile[i].getCost()), "fonts/Marker Felt.ttf", 24);
        costLabel->setTextColor(Color4B::ORANGE);
        costLabel->setPosition(Vec2(cardSprite->getContentSize().width / 2,
            cardSprite->getContentSize().height / 4));
        cardSprite->addChild(costLabel, 1);
    }
}


void DrawDeck::createBackButton()
{
    auto backButton = MenuItemImage::create(
        "button.png",
        "button_Selected.png",
        CC_CALLBACK_1(DrawDeck::backToFightingScene, this)
    );
    backButton->setScale(0.5f);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 设置按钮位置在画面左下角
    backButton->setPosition(Vec2(origin.x + backButton->getContentSize().width / 2,
        origin.y + backButton->getContentSize().height / 2));
    auto buttonMenu = Menu::create(backButton, nullptr);
    buttonMenu->setPosition(Vec2::ZERO);
    this->addChild(buttonMenu, 4);
}

void DrawDeck::backToFightingScene(cocos2d::Ref* sender)
{
    Director::getInstance()->popScene();
}
