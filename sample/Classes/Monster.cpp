#include "Monster.h"
#include <ctime>

USING_NS_CC;

// 初始化静态成员
std::mt19937 Monster::_rng(static_cast<unsigned int>(time(nullptr)));


// 工厂方法，根据类名创建具体怪物实例
Monster* Monster::createFactory(const std::string& monsterClassName) {
    Monster* monster = nullptr;

    // 普通怪物
    if (monsterClassName == "SlimeMonster") {
        monster = SlimeMonster::create();
    }
    else if (monsterClassName == "GoblinMonster") {
        monster = GoblinMonster::create();
    }
    else if (monsterClassName == "SnakeMonster") {
        monster = SnakeMonster::create();
    }
    // 精英怪物
    else if (monsterClassName == "OrcMonster") {
        monster = OrcMonster::create();
    }
    else if (monsterClassName == "GolemMonster") {
        monster = GolemMonster::create();
    }
    // Boss怪物
    else if (monsterClassName == "KnightBossMonster") {
        monster = KnightBossMonster::create();
    }
    else if (monsterClassName == "DragonBossMonster") {
        monster = DragonBossMonster::create();
    }
    // 默认情况下创建史莱姆
    else {
        monster = SlimeMonster::create();
    }

    return monster;
}

// 随机创建怪物
Monster* Monster::createRandom(bool isBoss, bool isElite) {
    std::vector<std::string> candidates;

    if (isBoss) {
        candidates = { "KnightBossMonster", "DragonBossMonster" };
    }
    else if (isElite) {
        candidates = { "OrcMonster", "GolemMonster" };
    }
    else {
        candidates = { "SlimeMonster", "GoblinMonster", "SnakeMonster" };
    }

    std::uniform_int_distribution<> dist(0, candidates.size() - 1);
    std::string selectedType = candidates[dist(_rng)];

    return createFactory(selectedType);
}

// 基础初始化
bool Monster::init() {
    _health = 20;      // 默认生命值
    _block = 0;        // 默认格挡值
    _attackDamage = 5; // 默认攻击力
    _currentTurn = 0;  // 初始回合数

    return true;
}

bool Monster::initWithSprite(const std::string& spritePath) {
    if (!Sprite::initWithFile(spritePath)) {
        CCLOG("Failed to load sprite: %s", spritePath.c_str());
        return false;
    }

    return true; // 不再调用 init()
}

// 各类getter和setter方法
void Monster::setHealth(int health) {
    _health = health;
}

int Monster::getHealth() const {
    if (this == nullptr) {
        CCLOG("Error: Called getHealth() on a nullptr Monster object!");
        return 0;
    }
    return _health;
}

void Monster::setBlock(int block)
{
    _block = block;
}

int Monster::getBlock() const
{
    return _block;
}

void Monster::setAttackDamage(int damage) {
    _attackDamage = damage;
}

int Monster::getAttackDamage() const {
    return _attackDamage;
}

// 添加效果
void Monster::addEffect(std::shared_ptr<Effect> effect) {
    if (!effect) {
        CCLOG("Error: Attempted to add a null effect.");
        return;
    }

    for (auto& existingEffect : _effects) {
        if (existingEffect->getType() == effect->getType()) {
            if (effect->getType() == Effect::Type::Strength) {
                existingEffect->setLevel(existingEffect->getLevel() + effect->getLevel());
            }
            else if (effect->getType() == Effect::Type::Vulnerable) {
                existingEffect->addRemainingTurns(effect->getRemainingTurns());
            }
            return;
        }
    }

    _effects.push_back(effect);
}

const std::vector<std::shared_ptr<Effect>>& Monster::getEffects() const {
    return _effects;
}

void Monster::updateEffects() {
    for (auto it = _effects.begin(); it != _effects.end(); ) {
        (*it)->reduceTurn();
        if ((*it)->getRemainingTurns() == 0) {
            it = _effects.erase(it);
        }
        else {
            ++it;
        }
    }
}

// 获取当前行为
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

// 执行当前行为
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

        // 应用怪物自身的力量
        for (const auto& effect : _effects) {
            if (auto buff = dynamic_cast<Buff*>(effect.get())) {
                if (buff->getType() == Effect::Type::Strength) {
                    finalDamage += buff->getLevel();
                }
            }
        }

        // 应用玩家的易伤debuff
        for (const auto& effect : target->getEffects()) {
            if (auto debuff = dynamic_cast<Debuff*>(effect.get())) {
                if (debuff->getType() == Effect::Type::Vulnerable) {
                    finalDamage = static_cast<int>(finalDamage * 1.5);
                }
            }
        }

        // 处理格挡
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

        // 造成伤害
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

// 准备下一回合
void Monster::prepareNextTurn() {
    _currentTurn++;
}

// 获取下一回合行为描述
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


// 三种基本怪物类的初始化
bool NormalMonster::init() {
    if (!Monster::init()) {
        return false;
    }

    // 普通怪物特有的初始化逻辑
    _health = 25;
    _attackDamage = 7;
    return true;
}

bool EliteMonster::init() {
    if (!Monster::init()) {
        return false;
    }

    // 精英怪物特有的初始化逻辑
    _health = 70;
    _attackDamage = 12;
    return true;
}

bool BossMonster::init() {
    if (!Monster::init()) {
        return false;
    }

    // Boss怪物特有的初始化逻辑
    _health = 150;
    _attackDamage = 20;
    return true;
}

// 具体怪物类的实现

// 史莱姆
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

    // 史莱姆的三回合循环模式
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

// 哥布林
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

    // 哥布林的行为模式
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

// 蛇
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

    // 蛇的行为模式：两回合攻击，一回合防御
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

// 兽人（精英）
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

    // 兽人的行为模式
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

// 魔像（精英）
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

    // 魔像的行为模式
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

// 黑暗骑士（Boss）
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

    // 黑暗骑士的行为模式
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

// 远古巨龙（Boss）
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

    // 远古巨龙的行为模式
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
