#include <gtest/gtest.h>

#include <InputParser.h>

#include <fstream>

namespace balancer::test {
struct BalanceTest : ::testing::Test {
    void SetUp() override {
        std::filesystem::path input = std::filesystem::path{DATA_FOLDER};
        std::string filename{::testing::UnitTest::GetInstance()->current_test_info()->name()};
        filename += ".txt";
        input /= filename;

        forest = balancer::importScalesForest(input);
    }   
    
    balancer::ScaleForest forest;
};

TEST_F(BalanceTest, assignment_example) {
    auto deltas = balancer::balance(forest);
    balancer::print(deltas);

    Masses deltas_expected {
        {"B1", 25, 0},
        {"B2", 0, 13},
        {"B3", 1, 0},
    };
    EXPECT_EQ(deltas, deltas_expected);
}

TEST_F(BalanceTest, assignment_example_changed_order) {
    auto deltas = balancer::balance(forest);
    balancer::print(deltas);

    Masses deltas_expected {
        {"B1", 25, 0},
        {"B2", 0, 13},
        {"B3", 1, 0},
    };
    EXPECT_EQ(deltas, deltas_expected);
}

TEST_F(BalanceTest, root_no_addition_needed_single_root) {
    auto deltas = balancer::balance(forest);
    balancer::print(deltas);

    Masses deltas_expected {
        {"B0", 0, 0},
    };
    EXPECT_EQ(deltas, deltas_expected);
}

TEST_F(BalanceTest, root_no_addition_needed_simple_tree) {
    auto deltas = balancer::balance(forest);
    balancer::print(deltas);

    Masses deltas_expected {
        {"left", 0, 0},
        {"right", 0, 0},
        {"root", 0, 0},
    };
    EXPECT_EQ(deltas, deltas_expected);
}

TEST_F(BalanceTest, root_no_addition_needed_tree) {
    auto deltas = balancer::balance(forest);
    balancer::print(deltas);

    Masses deltas_expected {
        {"childA", 0, 0},
        {"childB", 0, 0},
        {"left", 0, 0},
        {"right", 0, 0},
        {"root", 0, 0},
    };
    EXPECT_EQ(deltas, deltas_expected);
}

TEST_F(BalanceTest, tree) {
    auto deltas = balancer::balance(forest);
    balancer::print(deltas);

    Masses deltas_expected {
        {"childA", 1, 0},
        {"childB", 1, 0},
        {"left", 1, 0},
        {"right", 0, 1},
        {"root", 0, 0},
    };
    EXPECT_EQ(deltas, deltas_expected);
}

TEST_F(BalanceTest, forest) {
    auto deltas = balancer::balance(forest);
    balancer::print(deltas);

    Masses deltas_expected {
        {"B1", 25, 0},
        {"B2", 0, 13},
        {"B3", 1, 0},
        {"B4", 25, 0},
        {"B5", 0, 13},
        {"B6", 1, 0},
    };
    EXPECT_EQ(deltas, deltas_expected);
}
} 
