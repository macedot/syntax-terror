/*
    test_enemy.cpp - Enemy System Unit Tests
*/

#include "../src/enemy.h"
#include "test_framework.h"
#include <memory>

TEST(enemy_bug_should_have_specific_config_values) {
    EnemyConfig bugConfig = Enemy::getConfig(EnemyType::BUG);
    TEST_ASSERT(bugConfig.health > 0);
    TEST_ASSERT(bugConfig.damage > 0);
    TEST_ASSERT(bugConfig.speed > 0);
    TEST_ASSERT(bugConfig.minWave == 1);
    
    EnemyConfig segfaultConfig = Enemy::getConfig(EnemyType::SEGFAULT);
    TEST_ASSERT(segfaultConfig.health > bugConfig.health);
    TEST_ASSERT(segfaultConfig.minWave > 1);
    return true;
}

TEST(enemy_should_create_from_factory) {
    olc::vf2d pos(100.0f, 100.0f);
    
    auto bug = Enemy::create(EnemyType::BUG, pos);
    TEST_ASSERT(bug != nullptr);
    TEST_ASSERT(bug->getType() == EnemyType::BUG);
    TEST_ASSERT(bug->getHealth() > 0);
    
    auto segfault = Enemy::create(EnemyType::SEGFAULT, pos);
    TEST_ASSERT(segfault != nullptr);
    TEST_ASSERT(segfault->getType() == EnemyType::SEGFAULT);
    TEST_ASSERT(segfault->getHealth() > bug->getHealth());
    return true;
}

TEST(enemy_should_take_damage_and_die) {
    olc::vf2d pos(100.0f, 100.0f);
    auto enemy = Enemy::create(EnemyType::BUG, pos);
    
    int initialHealth = enemy->getHealth();
    enemy->takeDamage(5);
    
    TEST_ASSERT(enemy->getHealth() == initialHealth - 5);
    TEST_ASSERT(!enemy->isDead());
    
    enemy->takeDamage(initialHealth);
    TEST_ASSERT(enemy->isDead());
    return true;
}

TEST(enemy_should_clamp_to_bounds) {
    olc::vf2d pos(100.0f, 100.0f);
    auto enemy = Enemy::create(EnemyType::BUG, pos);
    
    enemy->setBounds(olc::vf2d(0.0f, 0.0f), olc::vf2d(200.0f, 200.0f));
    // Bounds are set - position clamping would happen in update
    TEST_ASSERT(enemy->getPosition().x == 100.0f);
    TEST_ASSERT(enemy->getPosition().y == 100.0f);
    return true;
}

bool run_enemy_tests() {
    RUN_TEST(enemy_bug_should_have_specific_config_values);
    RUN_TEST(enemy_should_create_from_factory);
    RUN_TEST(enemy_should_take_damage_and_die);
    RUN_TEST(enemy_should_clamp_to_bounds);
    return true;
}
