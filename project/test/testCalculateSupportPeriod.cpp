#include <stdexcept>

#include <gtest/gtest.h>

#include "../src/internship.h"

TEST(CalculateSupportPeriodTest, SameDate) {
    ASSERT_EQ(internship::calculateSupportPeriod("2020-04-26", "2020-04-26"), 1);
}

TEST(CalculateSupportPeriodTest, ValidDates) {
    EXPECT_EQ(internship::calculateSupportPeriod("2020-04-25", "2020-04-26"), 2);
    EXPECT_EQ(internship::calculateSupportPeriod("2020-04-25", "2020-04-27"), 3);
    EXPECT_EQ(internship::calculateSupportPeriod("2020-04-30", "2020-05-01"), 2);
    EXPECT_EQ(internship::calculateSupportPeriod("2023-01-03", "2024-08-04"), 580);
}

TEST(CalculateSupportPeriodTest, InvalidDates) {
    EXPECT_THROW(internship::calculateSupportPeriod("20-04", "2020-04-26"), std::invalid_argument)
    EXPECT_THROW(internship::calculateSupportPeriod("2020-04-25", "204-26"), std::invalid_argument)
    EXPECT_THROW(internship::calculateSupportPeriod("2024-08-04", "2023-01-03"), std::invalid_argument)
}
