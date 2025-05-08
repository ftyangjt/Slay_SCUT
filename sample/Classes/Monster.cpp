#include "Monster.h"
#include <ctime>

USING_NS_CC;

// ��ʼ����̬��Ա
std::mt19937 Monster::_rng(static_cast<unsigned int>(time(nullptr)));


// �����������������������������ʵ��
Monster* Monster::createFactory(const std::string& monsterClassName) {
    Monster* monster = nullptr;

    // ��ͨ����
    if (monsterClassName == "SlimeMonster") {
        monster = SlimeMonster::create();
    }
    else if (monsterClassName == "GoblinMonster") {
        monster = GoblinMonster::create();
    }
    else if (monsterClassName == "SnakeMonster") {
        monster = SnakeMonster::create();
    }
    // ��Ӣ����
    else if (monsterClassName == "OrcMonster") {
        monster = OrcMonster::create();
    }
    else if (monsterClassName == "GolemMonster") {
        monster = GolemMonster::create();
    }
    // Boss����
    else if (monsterClassName == "KnightBossMonster") {
        monster = KnightBossMonster::create();
    }
    else if (monsterClassName == "DragonBossMonster") {
        monster = DragonBossMonster::create();
    }
    // Ĭ������´���ʷ��ķ
    else {
        monster = SlimeMonster::create();
    }

    return monster;
}

// �����������
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

// ������ʼ��
bool Monster::init() {
    _health = 20;      // Ĭ������ֵ
    _block = 0;        // Ĭ�ϸ�ֵ
    _attackDamage = 5; // Ĭ�Ϲ�����
    _currentTurn = 0;  // ��ʼ�غ���

    return true;
}

bool Monster::initWithSprite(const std::string& spritePath) {
    if (!Sprite::initWithFile(spritePath)) {
        CCLOG("Failed to load sprite: %s", spritePath.c_str());
        return false;
    }

    return true; // ���ٵ��� init()
}

// ����getter��setter����
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

void Monster::setBlock(int block) {
    _block = block;
}

int Monster::getBlock() const {
    return _block;
}

void Monster::setAttackDamage(int damage) {
    _attackDamage = damage;
}

int Monster::getAttackDamage() const {
    return _attackDamage;
}

// ���Ч��
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

// ��ȡ��ǰ��Ϊ
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

// ִ�е�ǰ��Ϊ
void Monster::executeCurrentAction(Hero* target) {
    if (!target) {
        CCLOG("Error: Target is nullptr in executeCurrentAction!");
        return;
    }

    MonsterAction action = getCurrentAction();

    switch (action.type) {
    case MonsterActionType::ATTACK:
    {
        // ���������˺�
        int finalDamage = action.value;

        // Ӧ������Ч��
        for (const auto& effect : _effects) {
            if (auto buff = dynamic_cast<Buff*>(effect.get())) {
                if (buff->getType() == Effect::Type::Strength) {
                    finalDamage += buff->getLevel();
                }
            }
        }

        // ����Ŀ���
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

        // ����˺�
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

// ׼����һ�غ�
void Monster::prepareNextTurn() {
    _currentTurn++;
}

// ��ȡ��һ�غ���Ϊ����
std::string Monster::getNextActionDescription() const {
    if (_actionPattern.empty()) {
        return "����";
    }

    int nextActionIndex = (_currentTurn + 1) % _actionPattern.size();
    return _actionPattern[nextActionIndex].description;
}

// ���ֻ���������ĳ�ʼ��
bool NormalMonster::init() {
    if (!Monster::init()) {
        return false;
    }

    // ��ͨ�������еĳ�ʼ���߼�
    _health = 25;
    _attackDamage = 7;
    return true;
}

bool EliteMonster::init() {
    if (!Monster::init()) {
        return false;
    }

    // ��Ӣ�������еĳ�ʼ���߼�
    _health = 70;
    _attackDamage = 12;
    return true;
}

bool BossMonster::init() {
    if (!Monster::init()) {
        return false;
    }

    // Boss�������еĳ�ʼ���߼�
    _health = 150;
    _attackDamage = 20;
    return true;
}

// ����������ʵ��

// ʷ��ķ
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

    initActionPattern();
    return true;
}

void SlimeMonster::initActionPattern() {
    _actionPattern.clear();

    // ʷ��ķ�����غ�ѭ��ģʽ
    _actionPattern.push_back(MonsterAction(
        MonsterActionType::DEFEND,
        5,
        Effect::Type::Strength,
        0, 0,
        "����+5"
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::BUFF,
        0,
        Effect::Type::Strength,
        1, 3,
        "����+1"
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::ATTACK,
        _attackDamage,
        Effect::Type::Strength,
        0, 0,
        "����" + std::to_string(_attackDamage)
    ));
}

// �粼��
bool GoblinMonster::init() {
    if (!NormalMonster::init()) {
        return false;
    }

    if (!initWithSprite("goblin.png")) {
        return false;
    }

    _health = 30;
    _attackDamage = 8;
    _block = 5;

    initActionPattern();
    return true;
}

void GoblinMonster::initActionPattern() {
    _actionPattern.clear();

    // �粼�ֵ���Ϊģʽ
    _actionPattern.push_back(MonsterAction(
        MonsterActionType::ATTACK,
        _attackDamage,
        Effect::Type::Strength,
        0, 0,
        "����" + std::to_string(_attackDamage)
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::DEFEND,
        7,
        Effect::Type::Strength,
        0, 0,
        "����+7"
    ));
}

// ��
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

    initActionPattern();
    return true;
}

void SnakeMonster::initActionPattern() {
    _actionPattern.clear();

    // �ߵ���Ϊģʽ�����غϹ�����һ�غϷ���
    _actionPattern.push_back(MonsterAction(
        MonsterActionType::ATTACK,
        _attackDamage,
        Effect::Type::Strength,
        0, 0,
        "����" + std::to_string(_attackDamage)
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::ATTACK,
        _attackDamage,
        Effect::Type::Strength,
        0, 0,
        "����" + std::to_string(_attackDamage)
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::DEBUFF,
        0,
        Effect::Type::Vulnerable,
        2, 2,
        "ʩ������"
    ));
}

// ���ˣ���Ӣ��
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

    initActionPattern();
    return true;
}

void OrcMonster::initActionPattern() {
    _actionPattern.clear();

    // ���˵���Ϊģʽ
    _actionPattern.push_back(MonsterAction(
        MonsterActionType::ATTACK,
        _attackDamage,
        Effect::Type::Strength,
        0, 0,
        "����" + std::to_string(_attackDamage)
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::BUFF,
        0,
        Effect::Type::Strength,
        2, 3,
        "����+2"
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::ATTACK,
        _attackDamage * 1.5,
        Effect::Type::Strength,
        0, 0,
        "�ػ�" + std::to_string(static_cast<int>(_attackDamage * 1.5))
    ));
}

// ħ�񣨾�Ӣ��
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

    initActionPattern();
    return true;
}

void GolemMonster::initActionPattern() {
    _actionPattern.clear();

    // ħ�����Ϊģʽ
    _actionPattern.push_back(MonsterAction(
        MonsterActionType::DEFEND,
        20,
        Effect::Type::Strength,
        0, 0,
        "����+20"
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::ATTACK,
        _attackDamage,
        Effect::Type::Strength,
        0, 0,
        "����" + std::to_string(_attackDamage)
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::DEFEND,
        10,
        Effect::Type::Strength,
        0, 0,
        "����+10"
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::ATTACK,
        _attackDamage * 2,
        Effect::Type::Strength,
        0, 0,
        "�����һ�" + std::to_string(static_cast<int>(_attackDamage * 2))
    ));
}

// �ڰ���ʿ��Boss��
bool KnightBossMonster::init() {
    if (!BossMonster::init()) {
        return false;
    }

    if (!initWithSprite("dark_knight.png")) {
        return false;
    }

    _health = 200;
    _attackDamage = 25;
    _block = 20;

    initActionPattern();
    return true;
}

void KnightBossMonster::initActionPattern() {
    _actionPattern.clear();

    // �ڰ���ʿ����Ϊģʽ
    _actionPattern.push_back(MonsterAction(
        MonsterActionType::ATTACK,
        _attackDamage,
        Effect::Type::Strength,
        0, 0,
        "ն��" + std::to_string(_attackDamage)
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::DEFEND,
        25,
        Effect::Type::Strength,
        0, 0,
        "���Ƹ�+25"
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::BUFF,
        0,
        Effect::Type::Strength,
        3, 2,
        "��������+3"
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::ATTACK,
        _attackDamage * 2,
        Effect::Type::Strength,
        0, 0,
        "�����ػ�" + std::to_string(static_cast<int>(_attackDamage * 2))
    ));
}

// Զ�ž�����Boss��
bool DragonBossMonster::init() {
    if (!BossMonster::init()) {
        return false;
    }

    if (!initWithSprite("ancient_dragon.png")) {
        return false;
    }

    _health = 300;
    _attackDamage = 15;
    _block = 25;

    initActionPattern();
    return true;
}

void DragonBossMonster::initActionPattern() {
    _actionPattern.clear();

    // Զ�ž�������Ϊģʽ
    _actionPattern.push_back(MonsterAction(
        MonsterActionType::DEBUFF,
        0,
        Effect::Type::Vulnerable,
        3, 3,
        "��Ϣ��ʩ�����ˣ�"
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::DEFEND,
        30,
        Effect::Type::Strength,
        0, 0,
        "���۷���+30"
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::ATTACK,
        _attackDamage * 1.5,
        Effect::Type::Strength,
        0, 0,
        "��צ˺��" + std::to_string(static_cast<int>(_attackDamage * 1.5))
    ));

    _actionPattern.push_back(MonsterAction(
        MonsterActionType::SPECIAL,
        _attackDamage * 3,
        Effect::Type::Strength,
        0, 0,
        "������Ϣ"
    ));
}
