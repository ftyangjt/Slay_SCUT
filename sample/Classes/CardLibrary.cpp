// CardLibrary.cpp
#include "CardLibrary.h"
#include "Card.h"

USING_NS_CC;

namespace CardLibrary {

    // Function to get all available cards
    std::vector<Card> getAllCards() {
        std::vector<Card> allCards;

        // 攻击牌
        
		// 添加Strike卡牌
        allCards.push_back(Card("Strike", Card::Type::Attack, 1, "Deal 6 damage", "cardBackground.jpg", 6, 0));

		// 添加Bash卡牌
        Card bashCard("Bash", Card::Type::Attack, 2, "Deal 8 damage and apply 2 Vulnerable", "cardBackground.jpg", 8, 0);
        bashCard.addEffect(Effect::Type::Vulnerable, 2, 2); // 2 stacks of Vulnerable, lasts 2 turns
        allCards.push_back(bashCard);

		// 添加Pommel Strike卡牌
        Card pommelStrike("Pommel Strike", Card::Type::Attack, 1, "Deal 5 damage and draw 1 card", "cardBackground.jpg", 5, 0);
        pommelStrike.setSpecialEffect(Card::SpecialEffect::DrawCard, 1); // Draw 1 card
        allCards.push_back(pommelStrike);

		// 添加Counter卡牌
        Card counterAttack("Counter", Card::Type::Attack, 2, "Gain 5 Block, then deal 5 damage", "cardBackground.jpg", 5, 5);
        allCards.push_back(counterAttack);

		// 添加Heavy Blow卡牌
        allCards.push_back(Card("Heavy Blow", Card::Type::Attack, 2, "Deal 32 damage", "cardBackground.jpg", 32, 0));

        // 技能牌、能力牌

		// 添加Defend卡牌
        allCards.push_back(Card("Defend", Card::Type::Skill, 1, "Gain 5 Block", "cardBackground.jpg", 0, 5));

        // 添加Strength卡牌
        Card strengthPower("Strength", Card::Type::Power, 1, "Gain 2 Strength", "cardBackground.jpg");
        strengthPower.addEffect(Effect::Type::Strength, 2, -1); // 2级力量，持续时间为永久
        allCards.push_back(strengthPower);

        // 添加Adrenaline Rush卡牌
        Card adrenalineRush("Adrenaline Rush", Card::Type::Skill, 0, "Lose 3 HP, gain 2 Energy", "cardBackground.jpg");
        std::vector<std::pair<Card::SpecialEffect, int>> effects;
        effects.push_back(std::make_pair(Card::SpecialEffect::LoseHealth, 3));
        effects.push_back(std::make_pair(Card::SpecialEffect::GainEnergy, 2));
        // 添加每个效果
        for (const auto& effect : effects) {
            adrenalineRush.setSpecialEffect(effect.first, effect.second);
        }
        allCards.push_back(adrenalineRush);

        // 添加祭品卡牌
        Card sacrifice("Sacrifice", Card::Type::Skill, 0, "Lose 6HP,gain 2 energy,draw 3 cards", "cardBackground.jpg");
        std::vector<std::pair<Card::SpecialEffect, int>> sacrificeEffects;
        sacrificeEffects.push_back(std::make_pair(Card::SpecialEffect::LoseHealth, 6));
        sacrificeEffects.push_back(std::make_pair(Card::SpecialEffect::GainEnergy, 2));
        sacrificeEffects.push_back(std::make_pair(Card::SpecialEffect::DrawCard, 3));
        // 添加每个效果
        for (const auto& effect : sacrificeEffects) {
            sacrifice.setSpecialEffect(effect.first, effect.second);
        }
        allCards.push_back(sacrifice);

		// 诅咒牌
        // 添加一张名为"Curse of Weakness"的诅咒牌
        Card curseCard("Curse", Card::Type::Curse, 0, "This card cannot be played", "curseBackground.jpg");
        curseCard.setPlayable(false); // 诅咒牌不能被打出
        allCards.push_back(curseCard);

        return allCards;
    }

    // Get starter deck
    std::vector<Card> getStarterDeck() {
        std::vector<Card> starterDeck;

        // Add 5 "Strike" cards
        for (int i = 0; i < 5; i++) {
            starterDeck.push_back(Card("Strike", Card::Type::Attack, 1, "Deal 6 damage", "cardBackground.jpg", 6, 0));
        }

        // Add 5 "Defend" cards
        for (int i = 0; i < 5; i++) {
            starterDeck.push_back(Card("Defend", Card::Type::Skill, 1, "Gain 5 Block", "cardBackground.jpg", 0, 5));
        }

        // Add 1 "Bash" card
        Card bashCard("Bash", Card::Type::Attack, 2, "Deal 8 damage and apply 2 Vulnerable", "cardBackground.jpg", 8, 0);
        bashCard.addEffect(Effect::Type::Vulnerable, 2, 2);
        starterDeck.push_back(bashCard);
        
        starterDeck.push_back(getCardByName("Curse"));

        return starterDeck;
    }

    // Get card by name
    Card getCardByName(const std::string& cardName) {
        std::vector<Card> allCards = getAllCards();

        for (const auto& card : allCards) {
            if (card.getName() == cardName) {
                return card;
            }
        }

        // If not found, return a default card
        return Card("Unknown", Card::Type::Skill, 0, "Card not found", "cardBackground.jpg", 0, 0);
    }

    // Get a random card
    Card getRandomCard() {
        std::vector<Card> allCards = getAllCards();
        if (allCards.empty()) {
            return Card("Unknown", Card::Type::Skill, 0, "No cards available", "cardBackground.jpg", 0, 0);
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, allCards.size() - 1);

        return allCards[dis(gen)];
    }

    // Get cards by type
    std::vector<Card> getCardsByType(Card::Type type) {
        std::vector<Card> result;
        std::vector<Card> allCards = getAllCards();

        for (const auto& card : allCards) {
            if (card.getType() == type) {
                result.push_back(card);
            }
        }

        return result;
    }

    Card getRandomNonInitialNonCurseCard() {
        // 获取所有卡牌
        std::vector<Card> allCards = getAllCards();

        // 过滤掉初始牌和诅咒牌
        std::vector<Card> filteredCards;
        for (const auto& card : allCards) {
            if (card.getName() != "Strike" &&
                card.getName() != "Defend" &&
                card.getName() != "Bash" &&
                card.getType() != Card::Type::Curse) {
                filteredCards.push_back(card);
            }
        }

        // 如果没有符合条件的卡牌，返回一个默认卡牌
        if (filteredCards.empty()) {
            return Card("Unknown", Card::Type::Skill, 0, "No valid cards available", "cardBackground.jpg", 0, 0);
        }

        // 随机选择一张卡牌
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, filteredCards.size() - 1);

        return filteredCards[dis(gen)];
    }

} // namespace CardLibrary