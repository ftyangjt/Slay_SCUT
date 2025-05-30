#include "Hero.h"

USING_NS_CC;

// Ìí¼ÓÒ»¸ö¾²Ì¬±äÁ¿À´´æ´¢Ó¢ĞÛµÄ½¡¿µÖµ
static int heroHealth = 100; // Ä¬ÈÏÉúÃüÖµÎª100

// ´´½¨Ö÷½ÇÊµÀı
Hero* Hero::create(const std::string& filename)
{
    Hero* hero = new (std::nothrow) Hero();
    if (hero && hero->init(filename))
    {
        hero->autorelease();
        return hero;
    }
    CC_SAFE_DELETE(hero);
    return nullptr;
}

// »ñÈ¡µ±Ç°½¡¿µÖµ
int Hero::getCurrentHealth()
{
    return heroHealth;
}

// »Ö¸´Ö¸¶¨ÊıÁ¿µÄÉúÃüÖµ£¬²»³¬¹ıÉÏÏŞ
void Hero::healHealth(int amount)
{
    heroHealth = std::min(heroHealth + amount, MAX_HEALTH);
}

// ³õÊ¼»¯Ö÷½Ç
bool Hero::init(const std::string& filename)
{
    if (!Sprite::initWithFile(filename))
    {
        return false;
    }

    // Ê¹ÓÃ±£´æµÄÉúÃüÖµ³õÊ¼»¯Ö÷½Ç
    _health = heroHealth;

    // ´´½¨Ä¬ÈÏµÄ¿¨×é
    createDefaultDeck();

    return true;
}

// ÉèÖÃÖ÷½ÇµÄÉúÃüÖµ
void Hero::setHealth(int health)
{
    _health = health;
    // Í¬Ê±¸üĞÂ¾²Ì¬±äÁ¿
    heroHealth = health;
}

// »ñÈ¡Ö÷½ÇµÄÉúÃüÖµ
int Hero::getHealth() const
{
    return _health;
}

// ÖØÖÃ½¡¿µÖµ£¨¿ÉÒÔÔÚÓÎÏ·ÖØĞÂ¿ªÊ¼Ê±µ÷ÓÃ£©
void Hero::resetHealth()
{
    heroHealth = 100;  // Ö»ĞèÖØÖÃ¾²Ì¬±äÁ¿
    // É¾³ı¶Ô _health µÄÒıÓÃ
}

// ÉèÖÃÖ÷½ÇµÄ·ÀÓùÖµ
void Hero::setBlock(int block) {
    _block = block;
}

// »ñÈ¡Ö÷½ÇµÄ·ÀÓùÖµ
int Hero::getBlock() const {
    return _block;
}

// Ìí¼Ó¿¨ÅÆµ½¿¨×é
void Hero::addCardToDeck(const Card& card)
{
    _deck.push_back(card);
}

<<<<<<< Updated upstream
// »ñÈ¡¿¨×é
=======
// è·å–å¡ç»„
>>>>>>> Stashed changes
const std::vector<Card>& Hero::getDeck() const
{
    return _deck;
}

// Çå¿Õ¿¨×é
void Hero::clearDeck()
{
    _deck.clear();
}

// ³õÊ¼»¯Ä¬ÈÏ¿¨×é
void Hero::createDefaultDeck()
{
    // ÕâÀïÊ¾Àı´´½¨Ò»¸ö³õÊ¼¿¨×é£¬¾ßÌå¿¨ÅÆÊôĞÔ¸ù¾İÊµ¼ÊĞèÇóµ÷Õû
    clearDeck();

    // Ìí¼ÓÊ¾Àı¿¨ÅÆ£¨¼ÙÉè Card ¹¹Ôìº¯Êı£ºCard(name, type, cost, effect, background, attack, defense)£©
    addCardToDeck(Card("Strike", Card::Type::Attack, 1, "Deal 6 damage", "cardBackground.jpg", 6, 0));
    addCardToDeck(Card("Strike", Card::Type::Attack, 1, "Deal 6 damage", "cardBackground.jpg", 6, 0));
    addCardToDeck(Card("Strike", Card::Type::Attack, 1, "Deal 6 damage", "cardBackground.jpg", 6, 0));
    addCardToDeck(Card("Defend", Card::Type::Skill, 1, "Gain 5 Block", "cardBackground.jpg", 0, 5));
    addCardToDeck(Card("Defend", Card::Type::Skill, 1, "Gain 5 Block", "cardBackground.jpg", 0, 5));
    addCardToDeck(Card("Defend", Card::Type::Skill, 1, "Gain 5 Block", "cardBackground.jpg", 0, 5));

    // ´´½¨ Bash ¿¨ÅÆ²¢Ìí¼ÓÒ×ÉËĞ§¹û
    Card bashCard("Bash", Card::Type::Attack, 2, "Deal 8 damage and apply Vulnerable", "cardBackground.jpg", 8, 0);
    // Ê¹ÓÃĞÂ·½·¨Ìí¼ÓĞ§¹û£ºÖ±½ÓÖ¸¶¨Ğ§¹ûÀàĞÍ¡¢µÈ¼¶ºÍ³ÖĞøÊ±¼ä
    bashCard.addEffect(Effect::Type::Vulnerable, 1, 3); // 1¼¶Ò×ÉË£¬³ÖĞø3»ØºÏ
    addCardToDeck(bashCard);

    Card strengthCard("Strength", Card::Type::Power, 1, "Gain 2 Strength", "cardBackground.jpg");
    strengthCard.addEffect(Effect::Type::Strength, 2, -1); // 2¼¶Á¦Á¿£¬³ÖĞøÊ±¼äÎªÓÀ¾Ã
    addCardToDeck(strengthCard);
}

//  Ìí¼ÓĞ§¹û
void Hero::addEffect(std::shared_ptr<Effect> effect) {
    if (!effect) {
        CCLOG("Error: Attempted to add a null effect.");
        return; // Èç¹û´«ÈëµÄ effect ÊÇ¿ÕÖ¸Õë£¬Ö±½Ó·µ»Ø
    }

    for (auto& existingEffect : _effects) {
        // ¼ì²éÊÇ·ñ´æÔÚÏàÍ¬ÀàĞÍµÄĞ§¹û
        if (existingEffect->getType() == effect->getType()) {
            if (effect->getType() == Effect::Type::Strength) {
                // Á¦Á¿Ğ§¹û£ºµş¼ÓµÈ¼¶
                existingEffect->setLevel(existingEffect->getLevel() + effect->getLevel());
            }
            else if (effect->getType() == Effect::Type::Vulnerable) {
                // Ò×ÉËĞ§¹û£ºÊ±³¤µş¼Ó
                existingEffect->addRemainingTurns(effect->getRemainingTurns());
            }
            return; // ´¦ÀíÍê³ÉºóÖ±½Ó·µ»Ø
        }
    }

    // Èç¹ûÃ»ÓĞÕÒµ½ÏàÍ¬ÀàĞÍµÄĞ§¹û£¬ÔòÌí¼ÓĞÂµÄĞ§¹û
    _effects.push_back(effect);
}

// »ñÈ¡ÓµÓĞµÄĞ§¹û
const std::vector<std::shared_ptr<Effect>>& Hero::getEffects() const
{
    return _effects;
}

// ¸üĞÂĞ§¹û£¬ÒÆ³ı³ÖĞøÊ±¼äÎª 0 µÄĞ§¹û
void Hero::updateEffects() {
    for (auto it = _effects.begin(); it != _effects.end(); ) {
        (*it)->reduceTurn();
        if ((*it)->getRemainingTurns() == 0) {
            it = _effects.erase(it); // ÒÆ³ı³ÖĞøÊ±¼äÎª 0 µÄĞ§¹û
        }
        else {
            ++it;
        }
    }
}
<<<<<<< Updated upstream
=======

// æ›´æ–°çŠ¶æ€æ˜¾ç¤º (å®ä¾‹æ–¹æ³•)
void Hero::updateStatusDisplay() {
    updateStatusDisplayStatic(); // ç®€å•åœ°è°ƒç”¨é™æ€æ–¹æ³•
}

bool Hero::isDeckInitialized() const {
    return _isDeckInitialized;
}

// è®¾ç½®å¡ç»„åˆå§‹åŒ–çŠ¶æ€
void Hero::setDeckInitialized(bool initialized) {
    _isDeckInitialized = initialized;
}
>>>>>>> Stashed changes
