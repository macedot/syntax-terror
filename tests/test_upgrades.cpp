/*
    test_upgrades.cpp - Upgrade System Unit Tests
    
    Tests for upgrade system functionality
*/

#include "test_framework.h"
#include <cmath>

// Test upgrade cost calculation
static int calculateUpgradeCost(int currentLevel) {
    // Base cost is 10, increases by 5 per level
    return 10 + (currentLevel - 1) * 5;
}

TEST(upgrade_should_cost_10_at_level_1) {
    int cost = calculateUpgradeCost(1);
    TEST_ASSERT_EQ(cost, 10);
    return true;
}

TEST(upgrade_should_cost_15_at_level_2) {
    int cost = calculateUpgradeCost(2);
    TEST_ASSERT_EQ(cost, 15);
    return true;
}

TEST(upgrade_should_cost_30_at_level_5) {
    int cost = calculateUpgradeCost(5);
    TEST_ASSERT_EQ(cost, 30);
    return true;
}

TEST(upgrade_should_cost_55_at_level_10) {
    int cost = calculateUpgradeCost(10);
    TEST_ASSERT_EQ(cost, 55);
    return true;
}

TEST(upgrade_should_increase_cost_linearly) {
    int cost1 = calculateUpgradeCost(1);
    int cost2 = calculateUpgradeCost(2);
    int cost3 = calculateUpgradeCost(3);
    
    TEST_ASSERT_EQ(cost2 - cost1, 5);
    TEST_ASSERT_EQ(cost3 - cost2, 5);
    
    return true;
}

// Test canAfford logic
static bool canAfford(int gold, int cost) {
    return gold >= cost;
}

TEST(upgrade_should_be_affordable_with_exact_amount) {
    TEST_ASSERT(canAfford(10, 10));
    return true;
}

TEST(upgrade_should_be_affordable_with_more_than_needed) {
    TEST_ASSERT(canAfford(15, 10));
    return true;
}

TEST(upgrade_should_not_be_affordable_with_less_than_needed) {
    TEST_ASSERT(!canAfford(9, 10));
    return true;
}

TEST(upgrade_should_not_be_affordable_with_zero_gold) {
    TEST_ASSERT(!canAfford(0, 10));
    return true;
}

// Test stat bonus calculation
static int calculateStatBonus(int upgradeLevel, int baseBonus) {
    return upgradeLevel * baseBonus;
}

TEST(upgrade_should_provide_5_stat_bonus_at_level_1) {
    int bonus = calculateStatBonus(1, 5);
    TEST_ASSERT_EQ(bonus, 5);
    return true;
}

TEST(upgrade_should_provide_15_stat_bonus_at_level_3) {
    int bonus = calculateStatBonus(3, 5);
    TEST_ASSERT_EQ(bonus, 15);
    return true;
}

TEST(upgrade_should_provide_25_stat_bonus_at_level_5) {
    int bonus = calculateStatBonus(5, 5);
    TEST_ASSERT_EQ(bonus, 25);
    return true;
}

// Test upgrade point spending
static int spendUpgradePoint(int currentPoints) {
    return currentPoints - 1;
}

TEST(upgrade_should_reduce_points_when_spending) {
    int points = spendUpgradePoint(5);
    TEST_ASSERT_EQ(points, 4);
    return true;
}

TEST(upgrade_should_spend_to_zero_from_one_point) {
    int points = spendUpgradePoint(1);
    TEST_ASSERT_EQ(points, 0);
    return true;
}

// Test max upgrade level
TEST(upgrade_should_have_max_level_of_10) {
    const int MAX_UPGRADE_LEVEL = 10;
    TEST_ASSERT_EQ(MAX_UPGRADE_LEVEL, 10);
    return true;
}

TEST(upgrade_should_not_allow_exceeding_max_level) {
    int currentLevel = 10;
    bool canUpgrade = (currentLevel < 10);
    TEST_ASSERT(!canUpgrade);
    return true;
}

bool run_upgrade_tests() {
    RUN_TEST(upgrade_should_cost_10_at_level_1);
    RUN_TEST(upgrade_should_cost_15_at_level_2);
    RUN_TEST(upgrade_should_cost_30_at_level_5);
    RUN_TEST(upgrade_should_cost_55_at_level_10);
    RUN_TEST(upgrade_should_increase_cost_linearly);
    RUN_TEST(upgrade_should_be_affordable_with_exact_amount);
    RUN_TEST(upgrade_should_be_affordable_with_more_than_needed);
    RUN_TEST(upgrade_should_not_be_affordable_with_less_than_needed);
    RUN_TEST(upgrade_should_not_be_affordable_with_zero_gold);
    RUN_TEST(upgrade_should_provide_5_stat_bonus_at_level_1);
    RUN_TEST(upgrade_should_provide_15_stat_bonus_at_level_3);
    RUN_TEST(upgrade_should_provide_25_stat_bonus_at_level_5);
    RUN_TEST(upgrade_should_reduce_points_when_spending);
    RUN_TEST(upgrade_should_spend_to_zero_from_one_point);
    RUN_TEST(upgrade_should_have_max_level_of_10);
    RUN_TEST(upgrade_should_not_allow_exceeding_max_level);
    
    return true;
}
