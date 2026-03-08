/*
    test_framework.h - Shared Test Macros and Variables
    
    This header provides the test macros and declares extern variables
    that are defined in test_main.cpp.
*/

#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <iostream>
#include <cmath>

// Test counters - defined in test_main.cpp
extern int tests_passed;
extern int tests_failed;
extern int tests_total;

// Test macros
#define TEST(name) bool test_##name()
#define RUN_TEST(name) do { \
    tests_total++; \
    std::cout << "  " << #name << "... "; \
    if (test_##name()) { \
        tests_passed++; \
        std::cout << "\033[32mOK\033[0m" << std::endl; \
    } else { \
        tests_failed++; \
        std::cout << "\033[31mFAIL\033[0m" << std::endl; \
    } \
} while(0)

#define TEST_ASSERT(cond) do { \
    if (!(cond)) { \
        std::cerr << "\n    Assertion failed: " << #cond; \
        return false; \
    } \
} while(0)

#define TEST_ASSERT_EQ(a, b) do { \
    if ((a) != (b)) { \
        std::cerr << "\n    Expected " << (b) << " but got " << (a); \
        return false; \
    } \
} while(0)

#define TEST_ASSERT_NEAR(a, b, eps) do { \
    if (std::abs((a) - (b)) > (eps)) { \
        std::cerr << "\n    Expected " << (b) << " ± " << (eps) << " but got " << (a); \
        return false; \
    } \
} while(0)

#endif // TEST_FRAMEWORK_H
