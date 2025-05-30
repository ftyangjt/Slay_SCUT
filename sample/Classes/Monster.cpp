#include "Monster.h"

USING_NS_CC;

Monster* Monster::create(const std::string& filename)
{
    Monster* monster = new (std::nothrow) Monster();
    if (monster && monster->init(filename))
    {
        monster->autorelease();
        return monster;
    }
    CC_SAFE_DELETE(monster);
    return nullptr;
}

bool Monster::init(const std::string& filename)
{
    if (!Sprite::initWithFile(filename))
    {
        return false;
    }

    // ≥ı ºªØπ÷ŒÔµƒ Ù–‘
    _health = 100; // ƒ¨»œ…˙√¸÷µ

    return true;
}

void Monster::setHealth(int health)
{
    _health = health;
}

int Monster::getHealth() const
{
    return _health;
}

void Monster::setBlock(int block) {
    _block = block;
}

int Monster::getBlock() const {
    return _block;
}

void Monster::addEffect(std::shared_ptr<Effect> effect) {
    if (!effect) {
        CCLOG("Error: Attempted to add a null effect.");
        return; // »Áπ˚¥´»Îµƒ effect  «ø’÷∏’Î£¨÷±Ω”∑µªÿ
    }

    for (auto& existingEffect : _effects) {
        // ºÏ≤È «∑Ò¥Ê‘⁄œ‡Õ¨¿‡–Õµƒ–ßπ˚
        if (existingEffect->getType() == effect->getType()) {
            if (effect->getType() == Effect::Type::Strength) {
                // ¡¶¡ø–ßπ˚£∫µ˛º”µ»º∂
                existingEffect->setLevel(existingEffect->getLevel() + effect->getLevel());
            }
            else if (effect->getType() == Effect::Type::Vulnerable) {
                // “◊…À–ßπ˚£∫ ±≥§µ˛º”
                existingEffect->addRemainingTurns(effect->getRemainingTurns());
            }
            return; // ¥¶¿ÌÕÍ≥…∫Û÷±Ω”∑µªÿ
        }
    }

    // »Áπ˚√ª”–’“µΩœ‡Õ¨¿‡–Õµƒ–ßπ˚£¨‘ÚÃÌº”–¬µƒ–ßπ˚
    _effects.push_back(effect);
}

const std::vector<std::shared_ptr<Effect>>& Monster::getEffects() const
{
    return _effects;
}

void Monster::updateEffects() {
    for (auto it = _effects.begin(); it != _effects.end(); ) {
        (*it)->reduceTurn();
        if ((*it)->getRemainingTurns() == 0) {
            it = _effects.erase(it); // “∆≥˝≥÷–¯ ±º‰Œ™ 0 µƒ–ßπ˚
        }
        else {
            ++it;
        }
    }
<<<<<<< Updated upstream
}
=======
}

// Ëé∑ÂèñÂΩìÂâçË°å‰∏∫
MonsterAction Monster::getCurrentAction() {
    if (_actionPattern.empty()) {
        return MonsterAction(
            MonsterActionType::ATTACK,
            _attackDamage,
            Effect::Type::Strength,
            0, 0,
            "attack" + std::to_string(_attackDamage)
        );
    }

    int actionIndex = _currentTurn % _actionPattern.size();
    return _actionPattern[actionIndex];
}

// ÊâßË°åÂΩìÂâçË°å‰∏∫
void Monster::executeCurrentAction(Hero* target) {
    if (!target) {
        CCLOG("Error: Target is nullptr in executeCurrentAction!");
        return;
    }

    MonsterAction action = getCurrentAction();

    switch (action.type) {
    case MonsterActionType::ATTACK:
    {
        int finalDamage = action.value;

        // Â∫îÁî®ÊÄ™Áâ©Ëá™Ë∫´ÁöÑÂäõÈáè
        for (const auto& effect : _effects) {
            if (auto buff = dynamic_cast<Buff*>(effect.get())) {
                if (buff->getType() == Effect::Type::Strength) {
                    finalDamage += buff->getLevel();
                }
            }
        }

        // Â∫îÁî®Áé©ÂÆ∂ÁöÑÊòì‰º§debuff
        for (const auto& effect : target->getEffects()) {
            if (auto debuff = dynamic_cast<Debuff*>(effect.get())) {
                if (debuff->getType() == Effect::Type::Vulnerable) {
                    finalDamage = static_cast<int>(finalDamage * 1.5);
                }
            }
        }

        // Â§ÑÁêÜÊ†ºÊå°
        int targetBlock = target->getBlock();
        if (targetBlock > 0) {
            if (targetBlock >= finalDamage) {
                target->setBlock(targetBlock - finalDamage);
                finalDamage = 0;
            }
            else {
                finalDamage -= targetBlock;
                target->setBlock(0);
            }
        }

        // ÈÄ†Êàê‰º§ÂÆ≥
        if (finalDamage > 0) {
            int newHealth = target->getHealth() - finalDamage;
            target->setHealth(newHealth);
            CCLOG("Monster dealt %d damage to Hero", finalDamage);
        }
    }
    break;

    case MonsterActionType::DEFEND:
    {
        int newBlock = _block + action.value;
        setBlock(newBlock);
        CCLOG("Monster gained %d block", action.value);
    }
    break;

    case MonsterActionType::BUFF:
    {
        auto effect = std::make_shared<Buff>(
            action.effectType,
            action.effectLevel,
            action.effectDuration
        );
        addEffect(effect);
        CCLOG("Monster gained %s buff", effect->getDescription().c_str());
    }
    break;

    case MonsterActionType::DEBUFF:
    {
        auto effect = std::make_shared<Debuff>(
            action.effectType,
            action.effectLevel,
            action.effectDuration
        );
        target->addEffect(effect);
        CCLOG("Monster applied %s debuff to Hero", effect->getDescription().c_str());
    }
    break;

    case MonsterActionType::SPECIAL:
        CCLOG("Monster used special action");
        break;
    }

    prepareNextTurn();
}

// ÂáÜÂ§á‰∏ã‰∏ÄÂõûÂêà
void Monster::prepareNextTurn() {
    _currentTurn++;
}

// Ëé∑Âèñ‰∏ã‰∏ÄÂõûÂêàË°å‰∏∫ÊèèËø∞
std::string Monster::getNextActionDescription() const {
    if (_actionPattern.empty()) {
        CCLOG("Monster action pattern is empty!");
        return "Attack";
    }

    int nextActionIndex = (_currentTurn + 1) % _actionPattern.size();
    const std::string& desc = _actionPattern[nextActionIndex].description;
    CCLOG("Next action description: %s", desc.c_str());
    return desc;
}


// ‰∏âÁßçÂü∫Êú¨ÊÄ™Áâ©Á±ªÁöÑÂàùÂßãÂåñ
bool NormalMonster::init() {
    if (!Monster::init()) {
        return false;
    }

    // ÊôÆÈÄöÊÄ™Áâ©ÁâπÊúâÁöÑÂàùÂßãÂåñÈÄªËæë
    _health = 25;
    _attackDamage = 7;
    return true;
}

bool EliteMonster::init() {
    if (!Monster::init()) {
        return false;
    }

    // Á≤æËã±ÊÄ™Áâ©ÁâπÊúâÁöÑÂàùÂßãÂåñÈÄªËæë
    _health = 70;
    _attackDamage = 12;
    return true;
}

bool BossMonster::init() {
    if (!Monster::init()) {
        return false;
    }

    // BossÊÄ™Áâ©ÁâπÊúâÁöÑÂàùÂßãÂåñÈÄªËæë
    _health = 150;
    _attackDamage = 20;
    return true;
}

// ÂÖ∑‰ΩìÊÄ™Áâ©Á±ªÁöÑÂÆûÁé∞

// Âè≤Ëé±ÂßÜ
bool SlimeMonster::init() {
    if (!NormalMonster::init()) {
        return false;
    }

    if (!initWithSprite("slime.png")) {
        return false;
    }

    _health = 20;
    _attackDamage = 5;
    _block = 3;
    this->setScale(0.4f);
    initActionPattern();
    return true;
}

void SlimeMonster::initActionPattern() {
    _actionPattern.clear();

    // Âè≤Ëé±ÂßÜÁöÑ‰∏âÂõûÂêàÂæ™ÁéØÊ®°Âºè
    _actionPattern.push_back(MonsterAction(
        MonsterActionType::DEFEND,
        5,
        Effect::Type::Strength,
        0, 0,
        "Defence+5"
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::BUFF,
        0,
        Effect::Type::Strength,
        1, -1,
        "Strength+1"
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::ATTACK,
        _attackDamage,
        Effect::Type::Strength,
        0, 0,
        "Attack" + std::to_string(_attackDamage)
    ));
}

// Âì•Â∏ÉÊûó
bool GoblinMonster::init() {
    if (!NormalMonster::init()) {
        return false;
    }

    if (!initWithSprite("goblin-1.png")) {
        return false;
    }

    _health = 30;
    _attackDamage = 8;
    _block = 5;
	this->setScale(0.7f);
    initActionPattern();
    return true;
}

void GoblinMonster::initActionPattern() {
    _actionPattern.clear();

    // Âì•Â∏ÉÊûóÁöÑË°å‰∏∫Ê®°Âºè
    _actionPattern.push_back(MonsterAction(
        MonsterActionType::ATTACK,
        _attackDamage,
        Effect::Type::Strength,
        0, 0,
        "Attack" + std::to_string(_attackDamage)
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::DEFEND,
        7,
        Effect::Type::Strength,
        0, 0,
        "Defence+7"
    ));
}

// Ëõá
bool SnakeMonster::init() {
    if (!NormalMonster::init()) {
        return false;
    }

    if (!initWithSprite("snake.png")) {
        return false;
    }

    _health = 45;
    _attackDamage = 3;
    _block = 4;
    this->setScale(0.5f);
    this->setPositionY(this->getPositionY() - 20.0f);
    this->setPositionX(this->getPositionX() + 200.0f);
    initActionPattern();
    return true;
}

void SnakeMonster::initActionPattern() {
    _actionPattern.clear();

    // ËõáÁöÑË°å‰∏∫Ê®°ÂºèÔºö‰∏§ÂõûÂêàÊîªÂáªÔºå‰∏ÄÂõûÂêàÈò≤Âæ°
    _actionPattern.push_back(MonsterAction(
        MonsterActionType::ATTACK,
        _attackDamage,
        Effect::Type::Strength,
        0, 0,
        "Attack" + std::to_string(_attackDamage)
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::ATTACK,
        _attackDamage,
        Effect::Type::Strength,
        0, 0,
        "Attack" + std::to_string(_attackDamage)
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::DEBUFF,
        0,
        Effect::Type::Vulnerable,
        2, 2,
        "Apply Vulnerable"
    ));
}

// ÂÖΩ‰∫∫ÔºàÁ≤æËã±Ôºâ
bool OrcMonster::init() {
    if (!EliteMonster::init()) {
        return false;
    }

    if (!initWithSprite("orc.png")) {
        return false;
    }

    _health = 85;
    _attackDamage = 12;
    _block = 10;
    this->setScale(1.5f);

    initActionPattern();
    return true;
}

void OrcMonster::initActionPattern() {
    _actionPattern.clear();

    // ÂÖΩ‰∫∫ÁöÑË°å‰∏∫Ê®°Âºè
    _actionPattern.push_back(MonsterAction(
        MonsterActionType::ATTACK,
        _attackDamage,
        Effect::Type::Strength,
        0, 0,
        "Attack" + std::to_string(_attackDamage)
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::BUFF,
        0,
        Effect::Type::Strength,
        2, -1,
        "Strength+2"
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::ATTACK,
        _attackDamage * 1.5,
        Effect::Type::Strength,
        0, 0,
        "Heavy Blow" + std::to_string(static_cast<int>(_attackDamage * 1.5))
    ));
}

// È≠îÂÉèÔºàÁ≤æËã±Ôºâ
bool GolemMonster::init() {
    if (!EliteMonster::init()) {
        return false;
    }

    if (!initWithSprite("golem.png")) {
        return false;
    }

    _health = 80;
    _attackDamage = 15;
    _block = 15;
    this->setScale(1.5f);
    initActionPattern();
    return true;
}

void GolemMonster::initActionPattern() {
    _actionPattern.clear();

    // È≠îÂÉèÁöÑË°å‰∏∫Ê®°Âºè
    _actionPattern.push_back(MonsterAction(
        MonsterActionType::DEFEND,
        20,
        Effect::Type::Strength,
        0, 0,
        "Defence+20"
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::ATTACK,
        _attackDamage,
        Effect::Type::Strength,
        0, 0,
        "Attack" + std::to_string(_attackDamage)
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::DEFEND,
        10,
        Effect::Type::Strength,
        0, 0,
        "Defence+10"
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::ATTACK,
        _attackDamage * 2,
        Effect::Type::Strength,
        0, 0,
        "Heavy Smash" + std::to_string(static_cast<int>(_attackDamage * 2))
    ));
}

// ÈªëÊöóÈ™ëÂ£´ÔºàBossÔºâ
bool KnightBossMonster::init() {
    if (!BossMonster::init()) {
        return false;
    }

    if (!initWithSprite("knightboss.jpg")) {
        return false;
    }

    _health = 200;
    _attackDamage = 25;
    _block = 20;
	this->setScale(2.5f);
    initActionPattern();
    return true;
}

void KnightBossMonster::initActionPattern() {
    _actionPattern.clear();

    // ÈªëÊöóÈ™ëÂ£´ÁöÑË°å‰∏∫Ê®°Âºè
    _actionPattern.push_back(MonsterAction(
        MonsterActionType::ATTACK,
        _attackDamage,
        Effect::Type::Strength,
        0, 0,
        "Slash" + std::to_string(_attackDamage)
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::DEFEND,
        25,
        Effect::Type::Strength,
        0, 0,
        "Shield Block+25"
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::BUFF,
        0,
        Effect::Type::Strength,
        3, -1,
        "Strength Accumulation+3"
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::ATTACK,
        _attackDamage * 2,
        Effect::Type::Strength,
        0, 0,
        "Fatal Strike" + std::to_string(static_cast<int>(_attackDamage * 2))
    ));
}

// ËøúÂè§Â∑®ÈæôÔºàBossÔºâ
bool DragonBossMonster::init() {
    if (!BossMonster::init()) {
        return false;
    }

    if (!initWithSprite("dragonboss.png")) {
        return false;
    }

    _health = 300;
    _attackDamage = 15;
    _block = 25;
	this->setScale(0.8f);
    initActionPattern();
    return true;
}

void DragonBossMonster::initActionPattern() {
    _actionPattern.clear();

    // ËøúÂè§Â∑®ÈæôÁöÑË°å‰∏∫Ê®°Âºè
    _actionPattern.push_back(MonsterAction(
        MonsterActionType::DEBUFF,
        0,
        Effect::Type::Vulnerable,
        3, 3,
        "Dragon's Breath (Inflict Vulnerable)"
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::DEFEND,
        30,
        Effect::Type::Strength,
        0, 0,
        "Dragon Scale Defense+30"
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::ATTACK,
        _attackDamage * 1.5,
        Effect::Type::Strength,
        0, 0,
        "Dragon Claw Rend" + std::to_string(static_cast<int>(_attackDamage * 1.5))
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::SPECIAL,
        _attackDamage * 3,
        Effect::Type::Strength,
        0, 0,
        "Apocalyptic Dragon Breath"
    ));
}
>>>>>>> Stashed changes
