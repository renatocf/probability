/******************************************************************************/
/*  Probability - A fast implementation of probabilities using logarithms     */
/*  Copyright (C) 2016 Renato Cordeiro Ferreira                               */
/*                                                                            */
/*  This program is free software: you can redistribute it and/or modify      */
/*  it under the terms of the GNU General Public License as published by      */
/*  the Free Software Foundation, either version 3 of the License, or         */
/*  (at your option) any later version.                                       */
/*                                                                            */
/*  This program is distributed in the hope that it will be useful,           */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/*  GNU General Public License for more details.                              */
/*                                                                            */
/*  You should have received a copy of the GNU General Public License         */
/*  along with this program.  If not, see <www.gnu.org/licenses>.             */
/******************************************************************************/

// Standard headers
#include <limits>

// External headers
#include "gmock/gmock.h"

// Tested header
#include "probability.hpp"

/*----------------------------------------------------------------------------*/
/*                             USING DECLARATIONS                             */
/*----------------------------------------------------------------------------*/

using ::testing::Eq;
using ::testing::Ne;
using ::testing::Lt;
using ::testing::Le;
using ::testing::Gt;
using ::testing::Ge;

using ::testing::DoubleEq;

using probability::probability_t;

#define DOUBLE(X) static_cast<double>(X)

/*----------------------------------------------------------------------------*/
/*                                  FIXTURES                                  */
/*----------------------------------------------------------------------------*/

static const auto infinity
  = std::numeric_limits<probability_t::value_type>::infinity();

struct AProbabilityZero : public testing::Test {
  probability_t zero = 0.0;
};

struct AProbabilityOne : public testing::Test {
  probability_t one = 1.0;
};

struct AProbabilityHalf : public testing::Test {
  probability_t half = 0.5;
};

/*----------------------------------------------------------------------------*/
/*                                SIMPLE TESTS                                */
/*----------------------------------------------------------------------------*/

TEST(Probability, CanBeDefaultInitialized) {
  probability_t probability;
  ASSERT_THAT(DOUBLE(probability), DoubleEq(0.0));
}

TEST(Probability, CanBeInitializedInConstructorWithValue0) {
  probability_t probability(0.0);
  ASSERT_THAT(DOUBLE(probability), DoubleEq(0.0));
}

TEST(Probability, CanBeInitializedInInitializerListWithValue0) {
  probability_t probability{0.0};
  ASSERT_THAT(DOUBLE(probability), DoubleEq(0.0));
}

TEST(Probability, CanBeCopyInitializedInConstructorWithValue0) {
  probability_t probability = 0.0;
  ASSERT_THAT(DOUBLE(probability), DoubleEq(0.0));
}

TEST(Probability, CanBeCopyInitializedInInitializerListWithValue0) {
  probability_t probability = { 0.0 };
  ASSERT_THAT(DOUBLE(probability), DoubleEq(0.0));
}

TEST(Probability, CanBeInitializedWithValue1) {
  probability_t probability = 1.0;
  ASSERT_THAT(DOUBLE(probability), DoubleEq(1.0));
}

TEST(Probability, DiesIfInitializedWithValueSmallerThan0) {
  ASSERT_DEATH(probability_t probability(-1.0), "");
}

TEST(Probability, DiesIfInitializedWithValueBiggerThan1) {
  ASSERT_DEATH(probability_t probability(2.0), "");
}

/*----------------------------------------------------------------------------*/
/*                             TESTS WITH FIXTURE                             */
/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero, CanHaveInnerValueTypeAccessed) {
  ASSERT_THAT(zero.data(), Eq(-infinity));
}

TEST_F(AProbabilityZero, CanHaveInnerValueTypeAccessedInConstContext) {
  const probability_t& const_zero = zero;
  ASSERT_THAT(const_zero.data(), Eq(-infinity));
}

TEST_F(AProbabilityZero, HasInnerDataValueMinusInfinity) {
  ASSERT_THAT(zero.data(), Eq(-infinity));
}

TEST_F(AProbabilityZero, KeepsItsValueWhenMultipliedByAProbability) {
  probability_t half = 0.5;
  zero *= half;
  ASSERT_THAT(DOUBLE(zero), DoubleEq(0.0));
}

TEST_F(AProbabilityZero, KeepsItsValueWhenMultipliedByADouble) {
  zero *= 0.5;
  ASSERT_THAT(DOUBLE(zero), DoubleEq(0.0));
}

TEST_F(AProbabilityZero, KeepsItsValueWhenMultipliedByItself) {
  zero *= zero;
  ASSERT_THAT(DOUBLE(zero), DoubleEq(0.0));
}

TEST_F(AProbabilityZero, KeepsItsValueWhenDividedByAProbability) {
  probability_t half = 0.5;
  zero /= half;
  ASSERT_THAT(DOUBLE(zero), DoubleEq(0.0));
}

TEST_F(AProbabilityZero, KeepsItsValueWhenDividedByADouble) {
  zero /= 0.5;
  ASSERT_THAT(DOUBLE(zero), DoubleEq(0.0));
}

TEST_F(AProbabilityZero, DiesWhenDividedByItself) {
  ASSERT_DEATH(zero /= zero, "");
}

TEST_F(AProbabilityZero, CanBeIncreasedByZero) {
  zero += 0.0;
  ASSERT_THAT(DOUBLE(zero), DoubleEq(0.0));
}

TEST_F(AProbabilityZero, CanBeIncreasedByNonZero) {
  zero += 0.5;
  ASSERT_THAT(DOUBLE(zero), DoubleEq(0.5));
}

TEST_F(AProbabilityZero, CanBeIncreasedByOne) {
  zero += 1.0;
  ASSERT_THAT(DOUBLE(zero), DoubleEq(1.0));
}

TEST_F(AProbabilityZero, CanBeDecreasedByZero) {
  zero -= 0.0;
  ASSERT_THAT(DOUBLE(zero), DoubleEq(0.0));
}

TEST_F(AProbabilityZero, DiesIfDecreasedByNonZero) {
  ASSERT_DEATH(zero -= 0.5, "");
}

TEST_F(AProbabilityZero, IsEqualToItself) {
  ASSERT_THAT(zero, Eq(zero));
}

TEST_F(AProbabilityZero, IsSmallerThanAnyNonZeroProbability) {
  probability_t half = 0.5;
  ASSERT_THAT(zero, Lt(half));
}

/******/

TEST_F(AProbabilityOne, HasInnerDataValueZero) {
  ASSERT_THAT(DOUBLE(one.data()), DoubleEq(0.0));
}

TEST_F(AProbabilityOne, GetsAProbabilityValueWhenMultipliedByAProbability) {
  probability_t half = 0.5;
  one *= half;
  ASSERT_THAT(one, Eq(half));
}

TEST_F(AProbabilityOne, GetsADoubleValueWhenMultipliedByADouble) {
  one *= 0.5;
  ASSERT_THAT(DOUBLE(one), DoubleEq(0.5));
}

TEST_F(AProbabilityOne, KeepsItsValueWhenMultipliedByItself) {
  one *= one;
  ASSERT_THAT(one, Eq(one));
}

TEST_F(AProbabilityOne, DiesWhenDividedByAProbability) {
  probability_t half = 0.5;
  ASSERT_DEATH(one /= half, "");
}

TEST_F(AProbabilityOne, DiesWhenDividedByADouble) {
  ASSERT_DEATH(one /= 0.5, "");
}

TEST_F(AProbabilityOne, KeepsItsValueWhenDividedByItself) {
  one *= one;
  ASSERT_THAT(one, Eq(one));
}

TEST_F(AProbabilityOne, CanBeIncreasedByZero) {
  one += 0.0;
  ASSERT_THAT(DOUBLE(one), DoubleEq(1.0));
}

TEST_F(AProbabilityOne, DiesIfIncreasedByNonZero) {
  ASSERT_DEATH(one += 0.5, "");
}

TEST_F(AProbabilityOne, CanBeDecreasedByZero) {
  one -= 0.0;
  ASSERT_THAT(DOUBLE(one), DoubleEq(1.0));
}

TEST_F(AProbabilityOne, CanBeDecreasedByNonZero) {
  one -= 0.5;
  ASSERT_THAT(DOUBLE(one), DoubleEq(0.5));
}

TEST_F(AProbabilityOne, CanBeDecreasedByOne) {
  one -= 1.0;
  ASSERT_THAT(DOUBLE(one), DoubleEq(0.0));
}

TEST_F(AProbabilityOne, IsEqualToItself) {
  ASSERT_THAT(one, Eq(one));
}

TEST_F(AProbabilityOne, IsBiggerThanAnyNonZeroProbability) {
  probability_t half = 0.5;
  ASSERT_THAT(one, Gt(half));
}

/******/

TEST_F(AProbabilityHalf, IsEqualToAnotherProbabilityHalf) {
  probability_t half2 = 0.5;
  ASSERT_THAT(half, Eq(half2));
}

TEST_F(AProbabilityHalf, IsGreaterOrEqualToAnotherProbabilityHalf) {
  probability_t half2 = 0.5;
  ASSERT_THAT(half, Ge(half2));
}

TEST_F(AProbabilityHalf, IsLessOrEqualToAnotherProbabilityHalf) {
  probability_t half2 = 0.5;
  ASSERT_THAT(half, Le(half2));
}

TEST_F(AProbabilityHalf, IsDifferentFromAProbabilityAQuarter) {
  probability_t quarter = 0.25;
  ASSERT_THAT(half, Ne(quarter));
}

TEST_F(AProbabilityHalf, IsGreaterThanAProbabilityAQuarter) {
  probability_t quarter = 0.25;
  ASSERT_THAT(half, Gt(quarter));
}

TEST_F(AProbabilityHalf, IsGreaterOrEqualThanAProbabilityAQuarter) {
  probability_t quarter = 0.25;
  ASSERT_THAT(half, Ge(quarter));
}

TEST_F(AProbabilityHalf, IsDifferentFromAProbabilityThreeQuarters) {
  probability_t three_quarters = 0.75;
  ASSERT_THAT(half, Ne(three_quarters));
}

TEST_F(AProbabilityHalf, IsLessThanAProbabilityThreeQuarters) {
  probability_t three_quarters = 0.75;
  ASSERT_THAT(half, Lt(three_quarters));
}

TEST_F(AProbabilityHalf, IsLessOrEqualThanAProbabilityThreeQuarters) {
  probability_t three_quarters = 0.75;
  ASSERT_THAT(half, Le(three_quarters));
}

TEST_F(AProbabilityHalf, IsEqualToADoubleHalf) {
  ASSERT_THAT(DOUBLE(half), DoubleEq(0.5));
}

TEST_F(AProbabilityHalf, IsGreaterOrEqualToADoubleHalf) {
  ASSERT_THAT(half, Ge(0.5));
  ASSERT_THAT(0.5,  Le(half));
}

TEST_F(AProbabilityHalf, IsLessOrEqualToADoubleHalf) {
  ASSERT_THAT(half, Le(0.5));
  ASSERT_THAT(0.5,  Ge(half));
}

TEST_F(AProbabilityHalf, IsDifferentFromADoubleAQuarter) {
  ASSERT_THAT(half, Ne(0.25));
  ASSERT_THAT(0.25, Ne(half));
}

TEST_F(AProbabilityHalf, IsGreaterThanADoubleAQuarter) {
  ASSERT_THAT(half, Gt(0.25));
  ASSERT_THAT(0.25, Le(half));
}

TEST_F(AProbabilityHalf, IsGreaterOrEqualThanADoubleAQuarter) {
  ASSERT_THAT(half, Ge(0.25));
  ASSERT_THAT(0.25, Lt(half));
}

TEST_F(AProbabilityHalf, IsDifferentFromADoubleThreeQuarters) {
  ASSERT_THAT(half, Ne(0.75));
  ASSERT_THAT(0.75, Ne(half));
}

TEST_F(AProbabilityHalf, IsLessThanADoubleThreeQuarters) {
  ASSERT_THAT(half, Lt(0.75));
  ASSERT_THAT(0.75, Ge(half));
}

TEST_F(AProbabilityHalf, IsLessOrEqualThanADoubleThreeQuarters) {
  ASSERT_THAT(half, Le(0.75));
  ASSERT_THAT(0.75, Gt(half));
}

TEST_F(AProbabilityHalf, CanBeRightMultipliedByADouble) {
  ASSERT_THAT(DOUBLE(half * 0.4), DoubleEq(0.2));
}

TEST_F(AProbabilityHalf, CanBeLeftMultipliedByADouble) {
  ASSERT_THAT(DOUBLE(0.4 * half), DoubleEq(0.2));
}

TEST_F(AProbabilityHalf, CanBeRightMultipliedByAProbability) {
  probability_t quarter = 0.25;
  ASSERT_THAT(DOUBLE(half * quarter), DoubleEq(0.125));
}

TEST_F(AProbabilityHalf, CanBeLeftMultipliedByAProbability) {
  probability_t quarter = 0.25;
  ASSERT_THAT(DOUBLE(quarter * half), DoubleEq(0.125));
}

TEST_F(AProbabilityHalf, CanBeMultipliedByItself) {
  ASSERT_THAT(DOUBLE(half * half), DoubleEq(0.25));
}

TEST_F(AProbabilityHalf, CanBeRightDividedByASmallerDouble) {
  ASSERT_DEATH(half / 0.4, "");
}

TEST_F(AProbabilityHalf, CanBeRightDividedByABiggerDouble) {
  ASSERT_THAT(DOUBLE(half / 0.6), DoubleEq(0.5/0.6));
}

TEST_F(AProbabilityHalf, CanBeLeftDividedBySmallerADouble) {
  ASSERT_THAT(DOUBLE(0.4 / half), DoubleEq(0.4/0.5));
}

TEST_F(AProbabilityHalf, CanBeLeftDividedByABiggerouble) {
  ASSERT_DEATH(0.6 / half, "");
}

TEST_F(AProbabilityHalf, CanBeDividedByItself) {
  ASSERT_THAT(DOUBLE(half / half), DoubleEq(1.0));
}

TEST_F(AProbabilityHalf, CanBeRightAddedByADouble) {
  ASSERT_THAT(DOUBLE(half + 0.4), DoubleEq(0.9));
}

TEST_F(AProbabilityHalf, CanBeLeftAddedByADouble) {
  ASSERT_THAT(DOUBLE(0.4 + half), DoubleEq(0.9));
}

TEST_F(AProbabilityHalf, CanBeAddedByItself) {
  ASSERT_THAT(DOUBLE(half + half), DoubleEq(1.0));
}

TEST_F(AProbabilityHalf, CanBeRightSubtractedBySmallerADouble) {
  ASSERT_THAT(DOUBLE(half - 0.4), DoubleEq(0.1));
}

TEST_F(AProbabilityHalf, CanBeRightSubtractedByABiggerDouble) {
  ASSERT_DEATH(half - 0.6, "");
}

TEST_F(AProbabilityHalf, CanBeLeftSubtractedByASmallerDouble) {
  ASSERT_DEATH(0.4 - half, "");
}

TEST_F(AProbabilityHalf, CanBeLeftSubtractedByABiggerDouble) {
  ASSERT_THAT(DOUBLE(0.6 - half), DoubleEq(0.1));
}

TEST_F(AProbabilityHalf, CanBeSubtractedByItself) {
  ASSERT_THAT(DOUBLE(half - half), DoubleEq(0.0));
}
