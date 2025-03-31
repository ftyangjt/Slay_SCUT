#include "DiscardDeck.h"
#include "ui/CocosGUI.h"
#include "FightingScene.h"
#include "Card.h"

USING_NS_CC;

Scene* DiscardDeck::createScene(const std::vector<Card>& discardPile)
{
    return DiscardDeck::create(discardPile);
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in FightingScene.cpp\n");
}

DiscardDeck* DiscardDeck::create(const std::vector<Card>& discardPile)
{
    DiscardDeck* ret = new (std::nothrow) DiscardDeck();
    if(ret && ret->init(discardPile))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool DiscardDeck::init(const std::vector<Card>& discardPile)
{
    if (!Scene::init())
    {
        return false;
    }
    
    // 保存传入的弃牌堆数据
    _discardPile = discardPile;
    
    createBackground();
    showDeck();
    createBackButton(); // 创建返回按钮
    return true;
}

void DiscardDeck::createBackground()
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

void DiscardDeck::addCardEffectLabel(cocos2d::Sprite* cardSprite, const std::string& effect)
{
    auto effectLabel = Label::createWithTTF(effect, "fonts/Marker Felt.ttf", 48);
    effectLabel->setColor(cocos2d::Color3B::BLACK);
    effectLabel->setPosition(Vec2(cardSprite->getContentSize().width / 2, cardSprite->getContentSize().height / 2));
    cardSprite->addChild(effectLabel, 1);
}

void DiscardDeck::showDeck()
{
    // 清空已存在显示的卡牌精灵
    for (auto cardSprite : _cardSprites)
    {
        this->removeChild(cardSprite);
    }
    _cardSprites.clear();

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto tempCardSprite = Sprite::create("cardBackground.jpg");
    float cardWidth = tempCardSprite->getContentSize().width;
    float totalWidth = _discardPile.size() * cardWidth;
    float spacing = 0.0f;
    if (totalWidth > visibleSize.width)
    {
        cardWidth = (visibleSize.width - 20.0f) / _discardPile.size();
        spacing = 0.0f;
    }
    else
    {
        spacing = (visibleSize.width - totalWidth) / (_discardPile.size() + 1);
    }
    float startX = origin.x + spacing + cardWidth / 2;
    for (size_t i = 0; i < _discardPile.size(); ++i)
    {
        auto cardSprite = Sprite::create("cardBackground.jpg");
        cardSprite->setPosition(Vec2(startX + i * (cardWidth + spacing), origin.y + cardSprite->getContentSize().height / 3));
        cardSprite->setScale(cardWidth / cardSprite->getContentSize().width);
        this->addChild(cardSprite, 1);
        _cardSprites.push_back(cardSprite);
        addCardEffectLabel(cardSprite, _discardPile[i].getEffect());
    }
}

void DiscardDeck::createBackButton()
{
    auto backButton = MenuItemImage::create(
        "button.png",
        "button_Selected.png",
        CC_CALLBACK_1(DiscardDeck::backToFightingScene, this)
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

void DiscardDeck::backToFightingScene(cocos2d::Ref* sender)
{
    Director::getInstance()->popScene();
}