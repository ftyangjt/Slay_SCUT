#include "Monster.h"
#include <ctime>

USING_NS_CC;

// ��ʼ����̬��Ա
std::unordered_map<MonsterType, MonsterData> Monster::_monsterLibrary;
std::mt19937 Monster::_rng(static_cast<unsigned int>(time(nullptr)));

void Monster::initMonsterLibrary() {
    if (!_monsterLibrary.empty()) {
        return; // �Ѿ���ʼ����
    }

    // �������й�������
    _monsterLibrary[MonsterType::SLIME] = MonsterData(
        "slime",
        "slime.png",
        20,
        5,
        3,
        {},
        false,
        false
    );

    _monsterLibrary[MonsterType::GOBLIN] = MonsterData(
        "goblin",
        "goblin.png",
        30,
        8,
        5,
        { Effect::Type::Strength },
        false,
        false
    );

    _monsterLibrary[MonsterType::SKELETON] = MonsterData(
        "skeleton",
        "skeleton.png",
        30,
        10,
        8,
        { Effect::Type::Strength },
        false,
        false
    );

    _monsterLibrary[MonsterType::BAT] = MonsterData(
        "bat",
        "bat.png",
        30,
        7,
        2,
        {},
        false,
        false
    );

    _monsterLibrary[MonsterType::SNAKE] = MonsterData(
        "snake",
        "snake.png",
        45,
        3,
        4,
        { Effect::Type::Vulnerable },
        false,
        false
    );

    _monsterLibrary[MonsterType::GHOST] = MonsterData(
        "ghost",
        "ghost.png",
        65,
        9,
        7,
        { Effect::Type::Vulnerable },
        false,
        false
    );

    _monsterLibrary[MonsterType::ORC] = MonsterData(
        "orc",
        "orc.png",
        85,
        12,
        10,
        { Effect::Type::Strength },
        false,
        true // ��Ӣ��
    );

    _monsterLibrary[MonsterType::GOLEM] = MonsterData(
        "golem",
        "golem.png",
        80,
        15,
        15,
        { Effect::Type::Strength, Effect::Type::Vulnerable },
        false,
        true // ��Ӣ��
    );

    _monsterLibrary[MonsterType::DRAGON] = MonsterData(
        "dragon",
        "dragon.png",
        100,
        10,
        12,
        { Effect::Type::Strength, Effect::Type::Vulnerable },
        false,
        true // ��Ӣ��
    );

    // Boss����
    _monsterLibrary[MonsterType::BOSS_KNIGHT] = MonsterData(
        "dark_knight",
        "dark_knight.png",
        200,
        25,
        20,
        { Effect::Type::Strength, Effect::Type::Vulnerable },
        true,
        false
    );

    _monsterLibrary[MonsterType::BOSS_WIZARD] = MonsterData(
        "evil_wizard",
        "evil_wizard.png",
        100,
        50,
        15,
        { Effect::Type::Strength, Effect::Type::Vulnerable },
        true,
        false
    );

    _monsterLibrary[MonsterType::BOSS_DRAGON] = MonsterData(
        "ancient_dragon",
        "ancient_dragon.png",
        300,
        15,
        25,
        { Effect::Type::Strength, Effect::Type::Vulnerable },
        true,
        false
    );
}

Monster* Monster::create(const std::string& filename)
{
    // ���δָ���ļ��������������һ������
    if (filename.empty()) {
        return createRandom();
    }

    Monster* monster = new (std::nothrow) Monster();
    if (monster && monster->init(filename))
    {
        monster->autorelease();
        return monster;
    }
    CC_SAFE_DELETE(monster);
    return nullptr;
}

Monster* Monster::createWithType(MonsterType type)
{
    // ��ʼ�������
    initMonsterLibrary();

    // ���ָ�������Ƿ�����ڿ���
    if (_monsterLibrary.find(type) == _monsterLibrary.end()) {
        CCLOG("Monster type not found in library, creating default monster.");
        return create("monster.png"); // ʹ��Ĭ�Ϲ���ͼ��
    }

    const MonsterData& data = _monsterLibrary[type];

    Monster* monster = new (std::nothrow) Monster();
    if (monster && monster->initWithMonsterData(data))
    {
        monster->autorelease();
        return monster;
    }
    CC_SAFE_DELETE(monster);
    return nullptr;
}

// ��ӵ�Monster.cpp
Monster* Monster::createRandom(bool isBoss, bool isElite)
{
    // ��ʼ�������
    initMonsterLibrary();

    // �ҳ����������Ĺ�������
    std::vector<MonsterType> candidates;
    for (const auto& pair : _monsterLibrary) {
        if ((isBoss && pair.second.isBoss) ||
            (isElite && pair.second.isElite) ||
            (!isBoss && !isElite && !pair.second.isBoss && !pair.second.isElite)) {
            candidates.push_back(pair.first);
        }
    }

    // ���û�з��������Ĺ��ʹ��Ĭ�Ϸ�ʽ����
    Monster* monster = nullptr;

    if (candidates.empty()) {
        CCLOG("No suitable monsters found in library, creating default monster.");
        monster = Monster::create("monster.png");
    }
    else {
        // ���ѡ��һ����������
        std::uniform_int_distribution<> dist(0, candidates.size() - 1);
        MonsterType selectedType = candidates[dist(_rng)];

        monster = createWithType(selectedType);
    }

    // �������ʧ�ܣ����˵�Ĭ�Ϲ���
    if (monster == nullptr) {
        CCLOG("Failed to create random monster, falling back to default monster.");
        monster = Monster::create("monster.png");
    }

    return monster;
}


bool Monster::init(const std::string& filename)
{
    if (!Sprite::initWithFile(filename))
    {
        return false;
    }

    // ��ʼ������Ļ�������
    _health = 20; // Ĭ������ֵ
    _block = 0;    // Ĭ�ϸ�ֵ
    _attackDamage = 10; // Ĭ�Ϲ�����
    _type = MonsterType::SLIME; // Ĭ������

    return true;
}

bool Monster::initWithMonsterData(const MonsterData& data)
{
    // ���ع���ͼ��
    if (!Sprite::initWithFile(data.sprite))
    {
        CCLOG("Failed to load monster sprite: %s", data.sprite.c_str());
        return false;
    }

    // ���ù�������
    _data = data;
    _health = data.baseHealth;
    _block = data.baseBlockAmount;
    _attackDamage = data.baseAttackDamage;

    return true;
}

void Monster::setHealth(int health)
{
    _health = health;
}

int Monster::getHealth() const
{
    // ��ȫ��飺ȷ��thisָ����Ч
    if (this == nullptr) {
        CCLOG("Error: Called getHealth() on a nullptr Monster object!");
        return 0; // ����Ĭ��ֵ�����Ǳ���
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

void Monster::setAttackDamage(int damage)
{
    _attackDamage = damage;
}

int Monster::getAttackDamage() const
{
    return _attackDamage;
}

void Monster::setMonsterType(MonsterType type)
{
    _type = type;
}

MonsterType Monster::getMonsterType() const
{
    return _type;
}

const MonsterData& Monster::getMonsterData() const
{
    return _data;
}

void Monster::applyRandomEffect()
{
    if (_data.possibleEffects.empty()) {
        return; // û�п��õ�Ч��
    }

    // ���ѡ��һ��Ч��
    std::uniform_int_distribution<> dist(0, _data.possibleEffects.size() - 1);
    Effect::Type effectType = _data.possibleEffects[dist(_rng)];

    // �������Ч��ǿ�Ⱥͳ���ʱ��
    std::uniform_int_distribution<> levelDist(1, 3);  // Ч��ǿ��1-3
    std::uniform_int_distribution<> durationDist(1, 3);  // ����1-3�غ�
    int level = levelDist(_rng);
    int duration = durationDist(_rng);

    // ����Ч������
    std::shared_ptr<Effect> effect;

    // ����Ч�����ʹ�����ͬ��Ч������
    if (effectType == Effect::Type::Strength) {
        effect = std::make_shared<Buff>(effectType, level, duration);
    }
    else if (effectType == Effect::Type::Vulnerable) {
        effect = std::make_shared<Debuff>(effectType, level, duration);
    }

    // Ӧ��Ч��
    if (effect) {
        addEffect(effect);
    }
}

void Monster::addEffect(std::shared_ptr<Effect> effect) {
    if (!effect) {
        CCLOG("Error: Attempted to add a null effect.");
        return; // �������� effect �ǿ�ָ�룬ֱ�ӷ���
    }

    for (auto& existingEffect : _effects) {
        // ����Ƿ������ͬ���͵�Ч��
        if (existingEffect->getType() == effect->getType()) {
            if (effect->getType() == Effect::Type::Strength) {
                // ����Ч�����ӵȼ�
                existingEffect->setLevel(existingEffect->getLevel() + effect->getLevel());
            }
            else if (effect->getType() == Effect::Type::Vulnerable) {
                // ����Ч���ӳ��غ���
                existingEffect->addRemainingTurns(effect->getRemainingTurns());
            }
            return; // ������ɺ�ֱ�ӷ���
        }
    }

    // ���û���ҵ���ͬ���͵�Ч��������µ�Ч��
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
            it = _effects.erase(it); // �Ƴ�����ʱ��Ϊ 0 ��Ч��
        }
        else {
            ++it;
        }
    }
}
