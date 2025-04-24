#include "Monster.h"
#include <ctime>

USING_NS_CC;

// 初始化静态成员
std::unordered_map<MonsterType, MonsterData> Monster::_monsterLibrary;
std::mt19937 Monster::_rng(static_cast<unsigned int>(time(nullptr)));

void Monster::initMonsterLibrary() {
    if (!_monsterLibrary.empty()) {
        return; // 已经初始化过
    }

    // 定义所有怪物数据
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
        true // 精英怪
    );

    _monsterLibrary[MonsterType::GOLEM] = MonsterData(
        "golem",
        "golem.png",
        80,
        15,
        15,
        { Effect::Type::Strength, Effect::Type::Vulnerable },
        false,
        true // 精英怪
    );

    _monsterLibrary[MonsterType::DRAGON] = MonsterData(
        "dragon",
        "dragon.png",
        100,
        10,
        12,
        { Effect::Type::Strength, Effect::Type::Vulnerable },
        false,
        true // 精英怪
    );

    // Boss怪物
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
    // 如果未指定文件名，则随机创建一个怪物
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
    // 初始化怪物库
    initMonsterLibrary();

    // 检查指定类型是否存在于库中
    if (_monsterLibrary.find(type) == _monsterLibrary.end()) {
        CCLOG("Monster type not found in library, creating default monster.");
        return create("monster.png"); // 使用默认怪物图像
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

// 添加到Monster.cpp
Monster* Monster::createRandom(bool isBoss, bool isElite)
{
    // 初始化怪物库
    initMonsterLibrary();

    // 找出符合条件的怪物类型
    std::vector<MonsterType> candidates;
    for (const auto& pair : _monsterLibrary) {
        if ((isBoss && pair.second.isBoss) ||
            (isElite && pair.second.isElite) ||
            (!isBoss && !isElite && !pair.second.isBoss && !pair.second.isElite)) {
            candidates.push_back(pair.first);
        }
    }

    // 如果没有符合条件的怪物，使用默认方式创建
    Monster* monster = nullptr;

    if (candidates.empty()) {
        CCLOG("No suitable monsters found in library, creating default monster.");
        monster = Monster::create("monster.png");
    }
    else {
        // 随机选择一个怪物类型
        std::uniform_int_distribution<> dist(0, candidates.size() - 1);
        MonsterType selectedType = candidates[dist(_rng)];

        monster = createWithType(selectedType);
    }

    // 如果创建失败，回退到默认怪物
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

    // 初始化怪物的基本属性
    _health = 20; // 默认生命值
    _block = 0;    // 默认格挡值
    _attackDamage = 10; // 默认攻击力
    _type = MonsterType::SLIME; // 默认类型

    return true;
}

bool Monster::initWithMonsterData(const MonsterData& data)
{
    // 加载怪物图像
    if (!Sprite::initWithFile(data.sprite))
    {
        CCLOG("Failed to load monster sprite: %s", data.sprite.c_str());
        return false;
    }

    // 设置怪物属性
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
    // 安全检查：确保this指针有效
    if (this == nullptr) {
        CCLOG("Error: Called getHealth() on a nullptr Monster object!");
        return 0; // 返回默认值而不是崩溃
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
        return; // 没有可用的效果
    }

    // 随机选择一个效果
    std::uniform_int_distribution<> dist(0, _data.possibleEffects.size() - 1);
    Effect::Type effectType = _data.possibleEffects[dist(_rng)];

    // 随机决定效果强度和持续时间
    std::uniform_int_distribution<> levelDist(1, 3);  // 效果强度1-3
    std::uniform_int_distribution<> durationDist(1, 3);  // 持续1-3回合
    int level = levelDist(_rng);
    int duration = durationDist(_rng);

    // 创建效果对象
    std::shared_ptr<Effect> effect;

    // 根据效果类型创建不同的效果对象
    if (effectType == Effect::Type::Strength) {
        effect = std::make_shared<Buff>(effectType, level, duration);
    }
    else if (effectType == Effect::Type::Vulnerable) {
        effect = std::make_shared<Debuff>(effectType, level, duration);
    }

    // 应用效果
    if (effect) {
        addEffect(effect);
    }
}

void Monster::addEffect(std::shared_ptr<Effect> effect) {
    if (!effect) {
        CCLOG("Error: Attempted to add a null effect.");
        return; // 如果传入的 effect 是空指针，直接返回
    }

    for (auto& existingEffect : _effects) {
        // 检查是否存在相同类型的效果
        if (existingEffect->getType() == effect->getType()) {
            if (effect->getType() == Effect::Type::Strength) {
                // 力量效果叠加等级
                existingEffect->setLevel(existingEffect->getLevel() + effect->getLevel());
            }
            else if (effect->getType() == Effect::Type::Vulnerable) {
                // 易伤效果延长回合数
                existingEffect->addRemainingTurns(effect->getRemainingTurns());
            }
            return; // 处理完成后直接返回
        }
    }

    // 如果没有找到相同类型的效果，添加新的效果
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
            it = _effects.erase(it); // 移除持续时间为 0 的效果
        }
        else {
            ++it;
        }
    }
}
