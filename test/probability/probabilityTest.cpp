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
#include <type_traits>

// Tested header
#include "probability/probability.hpp"


/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
/*----------------------------------------------------------------------------*/
/*                             USING DECLARATIONS                             */
/*----------------------------------------------------------------------------*/
/*////////////////////////////////////////////////////////////////////////////*/

using ::testing::Eq;
using ::testing::Ne;
using ::testing::Lt;
using ::testing::Le;
using ::testing::Gt;
using ::testing::Ge;

using ::testing::DoubleEq;

using probability::log_double_t;
using probability::probability_t;

#define DOUBLE(X) static_cast<double>(X)

/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
/*----------------------------------------------------------------------------*/
/*                              AUXILIARY CLASSES                             */
/*----------------------------------------------------------------------------*/
/*////////////////////////////////////////////////////////////////////////////*/

template<typename T>
struct Number {
  using value_type = T;
  value_type value;
  operator value_type() const noexcept { return value; }
};

/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
/*----------------------------------------------------------------------------*/
/*                                  FIXTURES                                  */
/*----------------------------------------------------------------------------*/
/*////////////////////////////////////////////////////////////////////////////*/

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

struct AConvertibleToProbabilityHalf : public testing::Test {
  Number<probability_t> convertible_to_half { 0.5 };
};

/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
/*----------------------------------------------------------------------------*/
/*                                SIMPLE TESTS                                */
/*----------------------------------------------------------------------------*/
/*////////////////////////////////////////////////////////////////////////////*/

TEST(Probability, CanBeDefaultInitialized) {
  probability_t probability;
  ASSERT_THAT(DOUBLE(probability), DoubleEq(0.0));
}

/*----------------------------------------------------------------------------*/

TEST(Probability, CanBeInitializedInConstructorWithValue0) {
  probability_t probability(0.0);
  ASSERT_THAT(DOUBLE(probability), DoubleEq(0.0));
}

/*----------------------------------------------------------------------------*/

TEST(Probability, CanBeInitializedInInitializerListWithValue0) {
  probability_t probability{0.0};
  ASSERT_THAT(DOUBLE(probability), DoubleEq(0.0));
}

/*----------------------------------------------------------------------------*/

TEST(Probability, CanBeCopyInitializedInConstructorWithValue0) {
  probability_t probability = 0.0;
  ASSERT_THAT(DOUBLE(probability), DoubleEq(0.0));
}

/*----------------------------------------------------------------------------*/

TEST(Probability, CanBeCopyInitializedInInitializerListWithValue0) {
  probability_t probability = { 0.0 };
  ASSERT_THAT(DOUBLE(probability), DoubleEq(0.0));
}

/*----------------------------------------------------------------------------*/

TEST(Probability, CanBeInitializedWithValue1) {
  probability_t probability = 1.0;
  ASSERT_THAT(DOUBLE(probability), DoubleEq(1.0));
}

/*----------------------------------------------------------------------------*/

TEST(Probability, DiesIfInitializedWithValueSmallerThan0) {
  ASSERT_DEATH(probability_t probability(-1.0), "");
}

/*----------------------------------------------------------------------------*/

TEST(Probability, DiesIfInitializedWithValueBiggerThan1) {
  ASSERT_DEATH(probability_t probability(2.0), "");
}

/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
/*----------------------------------------------------------------------------*/
/*                             TESTS WITH FIXTURE                             */
/*----------------------------------------------------------------------------*/
/*////////////////////////////////////////////////////////////////////////////*/

/*----------------------------------------------------------------------------*/
/*                               TESTS FOR P=0                                */
/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero, CanHaveInnerValueTypeAccessed) {
  ASSERT_THAT(zero.data(), Eq(-infinity));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero, CanHaveInnerValueTypeAccessedInConstContext) {
  const probability_t& const_zero = zero;
  ASSERT_THAT(const_zero.data(), Eq(-infinity));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero, HasInnerDataValueMinusInfinity) {
  ASSERT_THAT(zero.data(), Eq(-infinity));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero, KeepsItsValueWhenMultipliedByAProbability) {
  probability_t half = 0.5;
  zero *= half;
  ASSERT_THAT(DOUBLE(zero), DoubleEq(0.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero,
    KeepsItsValueWhenMultipliedByAConvertibleToProbability) {
  Number<probability_t> convertible_to_half { 0.5 };
  zero *= convertible_to_half;
  ASSERT_THAT(DOUBLE(zero), DoubleEq(0.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero, KeepsItsValueWhenMultipliedByALogDouble) {
  log_double_t half = 0.5;
  zero *= half;
  ASSERT_THAT(DOUBLE(zero), DoubleEq(0.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero, KeepsItsValueWhenMultipliedByADouble) {
  zero *= 0.5;
  ASSERT_THAT(DOUBLE(zero), DoubleEq(0.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero, KeepsItsValueWhenMultipliedByAConvertibleToDouble) {
  Number<double> convertible_to_half { 0.5 };
  zero *= convertible_to_half;
  ASSERT_THAT(DOUBLE(zero), DoubleEq(0.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero, KeepsItsValueWhenMultipliedByItself) {
  zero *= zero;
  ASSERT_THAT(DOUBLE(zero), DoubleEq(0.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero, KeepsItsValueWhenMultipliedByAConvertibleToItself) {
  Number<probability_t> convertible_to_itself { zero };
  zero *= convertible_to_itself;
  ASSERT_THAT(DOUBLE(zero), DoubleEq(0.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero, KeepsItsValueWhenDividedByAProbability) {
  zero /= 0.5;
  ASSERT_THAT(DOUBLE(zero), DoubleEq(0.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero, KeepsItsValueWhenDividedByAConvertibleToProbability) {
  Number<probability_t> convertible_to_half { 0.5 };
  zero /= convertible_to_half;
  ASSERT_THAT(DOUBLE(zero), DoubleEq(0.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero, KeepsItsValueWhenDividedByADouble) {
  zero /= 0.5;
  ASSERT_THAT(DOUBLE(zero), DoubleEq(0.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero, KeepsItsValueWhenDividedByAConvertibleToDouble) {
  Number<double> convertible_to_half { 0.5 };
  zero /= convertible_to_half;
  ASSERT_THAT(DOUBLE(zero), DoubleEq(0.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero, DiesWhenDividedByItself) {
  ASSERT_DEATH(zero /= zero, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero, DiesWhenDividedByAConvertibleToItself) {
  Number<probability_t> convertible_to_itself { zero };
  ASSERT_DEATH(zero /= convertible_to_itself, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero, CanBeIncreasedByZero) {
  zero += 0.0;
  ASSERT_THAT(DOUBLE(zero), DoubleEq(0.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero, CanBeIncreasedByConvertibleToZero) {
  Number<double> convertible_to_zero { 0.0 };
  zero += convertible_to_zero;
  ASSERT_THAT(DOUBLE(zero), DoubleEq(0.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero, CanBeIncreasedByNonZero) {
  zero += 0.5;
  ASSERT_THAT(DOUBLE(zero), DoubleEq(0.5));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero, CanBeIncreasedByConvertibleToNonZero) {
  Number<double> convertible_to_non_zero { 0.5 };
  zero += convertible_to_non_zero;
  ASSERT_THAT(DOUBLE(zero), DoubleEq(0.5));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero, CanBeIncreasedByOne) {
  zero += 1.0;
  ASSERT_THAT(DOUBLE(zero), DoubleEq(1.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero, CanBeIncreasedByConvertibleToOne) {
  Number<double> convertible_to_one { 1.0 };
  zero += convertible_to_one;
  ASSERT_THAT(DOUBLE(zero), DoubleEq(1.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero, CanBeDecreasedByZero) {
  zero -= 0.0;
  ASSERT_THAT(DOUBLE(zero), DoubleEq(0.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero, CanBeDecreasedByConvertibleToZero) {
  Number<double> convertible_to_zero { 0.0 };
  zero -= convertible_to_zero;
  ASSERT_THAT(DOUBLE(zero), DoubleEq(0.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero, DiesIfDecreasedByNonZero) {
  ASSERT_DEATH(zero -= 0.5, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero, DiesIfDecreasedByConvertibleToNonZero) {
  Number<double> convertible_to_non_zero { 0.5 };
  ASSERT_DEATH(zero -= convertible_to_non_zero, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero, IsEqualToItself) {
  ASSERT_THAT(zero, Eq(zero));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero, IsEqualToAConvertibleToItself) {
  Number<probability_t> convertible_to_itself { zero };
  ASSERT_THAT(zero, Eq(convertible_to_itself));
  ASSERT_THAT(convertible_to_itself, Eq(zero));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero, IsSmallerThanAnyNonZeroProbability) {
  probability_t half = 0.5;
  ASSERT_THAT(zero, Lt(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityZero, IsSmallerThanAnyNonZeroConvertibleToProbability) {
  Number<probability_t> convertible_to_half { 0.5 };
  ASSERT_THAT(zero, Lt(convertible_to_half));
}

/*----------------------------------------------------------------------------*/
/*                               TESTS FOR P=1                                */
/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityOne, HasInnerDataValueZero) {
  ASSERT_THAT(DOUBLE(one.data()), DoubleEq(0.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityOne, GetsAProbabilityValueWhenMultipliedByAProbability) {
  probability_t half = 0.5;
  one *= half;
  ASSERT_THAT(one, Eq(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityOne,
    GetsAProbabilityValueWhenMultipliedByAConvertibleToProbability) {
  Number<probability_t> convertible_to_half { 0.5 };
  one *= convertible_to_half;
  ASSERT_THAT(one, Eq(convertible_to_half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityOne, GetsADoubleValueWhenMultipliedByADouble) {
  one *= 0.5;
  ASSERT_THAT(DOUBLE(one), DoubleEq(0.5));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityOne, GetsADoubleValueWhenMultipliedByAConvertibleToDouble) {
  Number<double> convertible_to_half { 0.5 };
  one *= convertible_to_half;
  ASSERT_THAT(DOUBLE(one), DoubleEq(0.5));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityOne, KeepsItsValueWhenMultipliedByItself) {
  one *= one;
  ASSERT_THAT(one, Eq(one));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityOne, GetsADoubleValueWhenMultipliedByAConvertibleToItself) {
  Number<probability_t> convertible_to_itself { one };
  one *= convertible_to_itself;
  ASSERT_THAT(one, Eq(one));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityOne, DiesWhenDividedByAProbability) {
  probability_t half = 0.5;
  ASSERT_DEATH(one /= half, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityOne, DiesWhenDividedByAConvertibleToProbability) {
  Number<probability_t> convertible_to_half { 0.5 };
  ASSERT_DEATH(one /= convertible_to_half, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityOne, DiesWhenDividedByADouble) {
  ASSERT_DEATH(one /= 0.5, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityOne, DiesWhenDividedByAConvertibleToDouble) {
  Number<double> convertible_to_half { 0.5 };
  ASSERT_DEATH(one /= convertible_to_half, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityOne, KeepsItsValueWhenDividedByItself) {
  one /= one;
  ASSERT_THAT(one, Eq(one));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityOne, KeepsItsValueWhenDividedByAConvertibleToItself) {
  Number<probability_t> convertible_to_itself { one };
  one /= convertible_to_itself;
  ASSERT_THAT(one, Eq(one));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityOne, CanBeIncreasedByZero) {
  one += 0.0;
  ASSERT_THAT(DOUBLE(one), DoubleEq(1.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityOne, CanBeIncreasedByConvertibleToZero) {
  Number<double> convertible_to_zero { 0.0 };
  one += convertible_to_zero;
  ASSERT_THAT(DOUBLE(one), DoubleEq(1.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityOne, DiesIfIncreasedByNonZero) {
  ASSERT_DEATH(one += 0.5, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityOne, DiesIfIncreasedByConvertibleToNonZero) {
  Number<double> convertible_to_non_zero { 0.5 };
  ASSERT_DEATH(one += convertible_to_non_zero, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityOne, CanBeDecreasedByZero) {
  one -= 0.0;
  ASSERT_THAT(DOUBLE(one), DoubleEq(1.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityOne, CanBeDecreasedByConvertibleToZero) {
  Number<double> convertible_to_zero { 0.0 };
  one -= convertible_to_zero;
  ASSERT_THAT(DOUBLE(one), DoubleEq(1.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityOne, CanBeDecreasedByNonZero) {
  one -= 0.5;
  ASSERT_THAT(DOUBLE(one), DoubleEq(0.5));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityOne, CanBeDecreasedByConvertibleToNonZero) {
  Number<double> convertible_to_non_zero { 0.5 };
  one -= convertible_to_non_zero;
  ASSERT_THAT(DOUBLE(one), DoubleEq(0.5));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityOne, CanBeDecreasedByOne) {
  one -= 1.0;
  ASSERT_THAT(DOUBLE(one), DoubleEq(0.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityOne, CanBeDecreasedByConvertibleToOne) {
  Number<double> convertible_to_one { 1.0 };
  one -= convertible_to_one;
  ASSERT_THAT(DOUBLE(one), DoubleEq(0.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityOne, IsEqualToItself) {
  ASSERT_THAT(one, Eq(one));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityOne, IsEqualToAConvertibleToItself) {
  Number<probability_t> convertible_to_itself { one };
  ASSERT_THAT(one, Eq(convertible_to_itself));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityOne, IsBiggerThanAnyNonZeroProbability) {
  probability_t half = 0.5;
  ASSERT_THAT(one, Gt(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityOne, IsBiggerThanAnyNonZeroConvertibleToProbability) {
  Number<probability_t> convertible_to_half { 0.5 };
  ASSERT_THAT(one, Gt(convertible_to_half));
}

/*----------------------------------------------------------------------------*/
/*                              TESTS FOR P=0.5                               */
/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsEqualToAnotherProbabilityHalf) {
  probability_t half2 = 0.5;
  ASSERT_THAT(half, Eq(half2));
  ASSERT_THAT(half2, Eq(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsEqualToAConvertibleToProbabilityHalf) {
  Number<probability_t> convertible_to_half { 0.5 };
  ASSERT_THAT(half, Eq(convertible_to_half));
  ASSERT_THAT(convertible_to_half, Eq(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsGreaterOrEqualToAnotherProbabilityHalf) {
  probability_t half2 = 0.5;
  ASSERT_THAT(half, Ge(half2));
  ASSERT_THAT(half2, Le(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsGreaterOrEqualToAConvertibleToProbabilityHalf) {
  Number<probability_t> convertible_to_half { 0.5 };
  ASSERT_THAT(half, Ge(convertible_to_half));
  ASSERT_THAT(convertible_to_half, Le(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsLessOrEqualToAnotherProbabilityHalf) {
  probability_t half2 = 0.5;
  ASSERT_THAT(half, Le(half2));
  ASSERT_THAT(half2, Ge(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsLessOrEqualToAConvertibleToProbabilityHalf) {
  Number<probability_t> convertible_to_half { 0.5 };
  ASSERT_THAT(half, Le(convertible_to_half));
  ASSERT_THAT(convertible_to_half, Ge(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsDifferentFromAProbabilityAQuarter) {
  probability_t quarter = 0.25;
  ASSERT_THAT(half, Ne(quarter));
  ASSERT_THAT(quarter, Ne(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsDifferentFromAConvertibleToProbabilityAQuarter) {
  Number<probability_t> convertible_to_quarter { 0.25 };
  ASSERT_THAT(half, Ne(convertible_to_quarter));
  ASSERT_THAT(convertible_to_quarter, Ne(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsGreaterThanAProbabilityAQuarter) {
  probability_t quarter = 0.25;
  ASSERT_THAT(half, Gt(quarter));
  ASSERT_THAT(quarter, Le(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsGreaterThanAConvertibleToProbabilityAQuarter) {
  Number<probability_t> convertible_to_quarter { 0.25 };
  ASSERT_THAT(half, Gt(convertible_to_quarter));
  ASSERT_THAT(convertible_to_quarter, Le(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsGreaterOrEqualThanAProbabilityAQuarter) {
  probability_t quarter = 0.25;
  ASSERT_THAT(half, Ge(quarter));
  ASSERT_THAT(quarter, Lt(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf,
    IsGreaterOrEqualThanAConvertibleToProbabilityAQuarter) {
  Number<probability_t> convertible_to_quarter { 0.25 };
  ASSERT_THAT(half, Ge(convertible_to_quarter));
  ASSERT_THAT(convertible_to_quarter, Lt(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsDifferentFromAProbabilityThreeQuarters) {
  probability_t three_quarters = 0.75;
  ASSERT_THAT(half, Ne(three_quarters));
  ASSERT_THAT(three_quarters, Ne(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf,
    IsDifferentFromAConvertibleToProbabilityThreeQuarters) {
  Number<probability_t> convertible_to_three_quarters { 0.75 };
  ASSERT_THAT(half, Ne(convertible_to_three_quarters));
  ASSERT_THAT(convertible_to_three_quarters, Ne(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsLessThanAProbabilityThreeQuarters) {
  probability_t three_quarters = 0.75;
  ASSERT_THAT(half, Lt(three_quarters));
  ASSERT_THAT(three_quarters, Ge(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsLessThanAConvertibleToProbabilityThreeQuarters) {
  Number<probability_t> convertible_to_three_quarters { 0.75 };
  ASSERT_THAT(half, Lt(convertible_to_three_quarters));
  ASSERT_THAT(convertible_to_three_quarters, Ge(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsLessOrEqualThanAProbabilityThreeQuarters) {
  probability_t three_quarters = 0.75;
  ASSERT_THAT(half, Le(three_quarters));
  ASSERT_THAT(three_quarters, Gt(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf,
    IsLessOrEqualThanAConvertibleToProbabilityThreeQuarters) {
  Number<probability_t> convertible_to_three_quarters { 0.75 };
  ASSERT_THAT(half, Le(convertible_to_three_quarters));
  ASSERT_THAT(convertible_to_three_quarters, Gt(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsEqualToADoubleHalf) {
  ASSERT_THAT(DOUBLE(half), DoubleEq(0.5));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsEqualToAConvertibleToDoubleHalf) {
  Number<double> convertible_to_half { 0.5 };
  ASSERT_THAT(DOUBLE(half), DoubleEq(convertible_to_half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsGreaterOrEqualToADoubleHalf) {
  ASSERT_THAT(half, Ge(0.5));
  ASSERT_THAT(0.5, Le(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsGreaterOrEqualToAConvertibleToDoubleHalf) {
  Number<double> convertible_to_half { 0.5 };
  ASSERT_THAT(half, Ge(convertible_to_half));
  ASSERT_THAT(convertible_to_half, Le(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsLessOrEqualToADoubleHalf) {
  ASSERT_THAT(half, Le(0.5));
  ASSERT_THAT(0.5, Ge(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsLessOrEqualToAConvertibleToDoubleHalf) {
  Number<double> convertible_to_half { 0.5 };
  ASSERT_THAT(half, Le(convertible_to_half));
  ASSERT_THAT(convertible_to_half, Ge(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsDifferentFromADoubleAQuarter) {
  ASSERT_THAT(half, Ne(0.25));
  ASSERT_THAT(0.25, Ne(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsDifferentFromAConvertibleToDoubleAQuarter) {
  Number<double> convertible_to_quarter { 0.25 };
  ASSERT_THAT(half, Ne(convertible_to_quarter));
  ASSERT_THAT(convertible_to_quarter, Ne(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsGreaterThanADoubleAQuarter) {
  ASSERT_THAT(half, Gt(0.25));
  ASSERT_THAT(0.25, Le(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsGreaterThanAConvertibleToDoubleAQuarter) {
  Number<double> convertible_to_quarter { 0.25 };
  ASSERT_THAT(half, Gt(convertible_to_quarter));
  ASSERT_THAT(convertible_to_quarter, Le(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsGreaterOrEqualThanADoubleAQuarter) {
  ASSERT_THAT(half, Ge(0.25));
  ASSERT_THAT(0.25, Lt(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsGreaterOrEqualThanAConvertibleToDoubleAQuarter) {
  Number<double> convertible_to_quarter { 0.25 };
  ASSERT_THAT(half, Ge(convertible_to_quarter));
  ASSERT_THAT(convertible_to_quarter, Lt(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsDifferentFromADoubleThreeQuarters) {
  ASSERT_THAT(half, Ne(0.75));
  ASSERT_THAT(0.75, Ne(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsDifferentFromAConvertibleToDoubleThreeQuarters) {
  Number<double> convertible_to_three_quarters { 0.75 };
  ASSERT_THAT(half, Ne(convertible_to_three_quarters));
  ASSERT_THAT(convertible_to_three_quarters, Ne(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsLessThanADoubleThreeQuarters) {
  ASSERT_THAT(half, Lt(0.75));
  ASSERT_THAT(0.75, Ge(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsLessThanAConvertibleToDoubleThreeQuarters) {
  Number<double> convertible_to_three_quarters { 0.75 };
  ASSERT_THAT(half, Lt(convertible_to_three_quarters));
  ASSERT_THAT(convertible_to_three_quarters, Ge(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsLessOrEqualThanADoubleThreeQuarters) {
  ASSERT_THAT(half, Le(0.75));
  ASSERT_THAT(0.75, Gt(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, IsLessOrEqualThanAConvertibleToDoubleThreeQuarters) {
  Number<double> convertible_to_three_quarters { 0.75 };
  ASSERT_THAT(half, Le(convertible_to_three_quarters));
  ASSERT_THAT(convertible_to_three_quarters, Gt(half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeRightMultipliedByADouble) {
  ASSERT_THAT(DOUBLE(half * 0.4), DoubleEq(0.2));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeRightMultipliedByAConvertibleToDouble) {
  Number<double> convertible_to_quarter { 0.25 };
  ASSERT_THAT(DOUBLE(half * convertible_to_quarter), DoubleEq(0.125));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeLeftMultipliedByADouble) {
  ASSERT_THAT(DOUBLE(0.4 * half), DoubleEq(0.2));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeLeftMultipliedByAConvertibleToDouble) {
  Number<double> convertible_to_quarter { 0.25 };
  ASSERT_THAT(DOUBLE(convertible_to_quarter * half), DoubleEq(0.125));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeRightMultipliedByAProbability) {
  probability_t quarter = 0.25;
  ASSERT_THAT(DOUBLE(half * quarter), DoubleEq(0.125));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeRightMultipliedByAConvertibleToProbability) {
  Number<probability_t> convertible_to_quarter { 0.25 };
  ASSERT_THAT(DOUBLE(half * convertible_to_quarter), DoubleEq(0.125));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeLeftMultipliedByAProbability) {
  probability_t quarter = 0.25;
  ASSERT_THAT(DOUBLE(quarter * half), DoubleEq(0.125));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeLeftMultipliedByAConvertibleToProbability) {
  Number<probability_t> convertible_to_quarter { 0.25 };
  ASSERT_THAT(DOUBLE(convertible_to_quarter * half), DoubleEq(0.125));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeMultipliedByItself) {
  ASSERT_THAT(DOUBLE(half * half), DoubleEq(0.25));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeRightMultipliedByAConvertibleToItself) {
  Number<probability_t> convertible_to_itself { half };
  ASSERT_THAT(DOUBLE(half * convertible_to_itself), DoubleEq(0.25));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeLeftMultipliedByAConvertibleToItself) {
  Number<probability_t> convertible_to_itself { half };
  ASSERT_THAT(DOUBLE(convertible_to_itself * half), DoubleEq(0.25));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeDividedByASmallerDouble) {
  ASSERT_DEATH(half / 0.4, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeDividedByASmallerConvertibleToDouble) {
  Number<double> convertible_to_smaller { 0.4 };
  ASSERT_DEATH(half / convertible_to_smaller, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeDividedByABiggerDouble) {
  ASSERT_THAT(DOUBLE(half / 0.6), DoubleEq(0.5/0.6));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeDividedByABiggerConvertibleToDouble) {
  Number<double> convertible_to_bigger { 0.6 };
  ASSERT_THAT(DOUBLE(half / convertible_to_bigger), DoubleEq(0.5/0.6));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanDivideASmallerDouble) {
  ASSERT_THAT(DOUBLE(0.4 / half), DoubleEq(0.4/0.5));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanDivideASmallerConvertibleToDouble) {
  Number<double> convertible_to_smaller { 0.4 };
  ASSERT_THAT(DOUBLE(convertible_to_smaller / half), DoubleEq(0.4/0.5));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanDivideABiggerDouble) {
  ASSERT_DEATH(0.6 / half, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanDivideABiggerConvertibleToDouble) {
  Number<double> convertible_to_bigger { 0.6 };
  ASSERT_DEATH(convertible_to_bigger / half, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeDividedByASmallerProbability) {
  probability_t probability = 0.4;
  ASSERT_DEATH(half / probability, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeDividedByASmallerConvertibleToProbability) {
  Number<probability_t> convertible_to_smaller { 0.4 };
  ASSERT_DEATH(half / convertible_to_smaller, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeDividedByABiggerProbability) {
  probability_t probability = 0.6;
  ASSERT_THAT(DOUBLE(half / probability), DoubleEq(0.5/0.6));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeDividedByABiggerConvertibleToProbability) {
  Number<probability_t> convertible_to_bigger { 0.6 };
  ASSERT_THAT(DOUBLE(half / convertible_to_bigger), DoubleEq(0.5/0.6));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanDivideASmallerProbability) {
  probability_t probability = 0.4;
  ASSERT_THAT(DOUBLE(probability / half), DoubleEq(0.4/0.5));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanDivideASmallerConvertibleToProbability) {
  Number<probability_t> convertible_to_smaller { 0.4 };
  ASSERT_THAT(DOUBLE(convertible_to_smaller / half), DoubleEq(0.4/0.5));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanDivideABiggerProbability) {
  probability_t probability = 0.6;
  ASSERT_DEATH(probability / half, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanDivideABiggerConvertibleToProbability) {
  Number<probability_t> convertible_to_bigger { 0.6 };
  ASSERT_DEATH(convertible_to_bigger / half, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeDividedByItself) {
  ASSERT_THAT(DOUBLE(half / half), DoubleEq(1.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeDividedByAConvertibleToItself) {
  Number<probability_t> convertible_to_itself { half };
  ASSERT_THAT(DOUBLE(half / convertible_to_itself), DoubleEq(1.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanDivideAConvertibleToItself) {
  Number<probability_t> convertible_to_itself { half };
  ASSERT_THAT(DOUBLE(convertible_to_itself / half), DoubleEq(1.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeRightAddedByADouble) {
  ASSERT_THAT(DOUBLE(half + 0.25), DoubleEq(0.75));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeRightAddedByAConvertibleToDouble) {
  Number<double> convertible_to_quarter { 0.25 };
  ASSERT_THAT(DOUBLE(half + convertible_to_quarter), DoubleEq(0.75));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeLeftAddedByADouble) {
  ASSERT_THAT(DOUBLE(0.25 + half), DoubleEq(0.75));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeLeftAddedByAConvertibleToDouble) {
  Number<double> convertible_to_quarter { 0.25 };
  ASSERT_THAT(DOUBLE(convertible_to_quarter + half), DoubleEq(0.75));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeRightAddedByAProbability) {
  probability_t quarter = 0.25;
  ASSERT_THAT(DOUBLE(half + quarter), DoubleEq(0.75));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeRightAddedByAConvertibleToProbability) {
  Number<probability_t> convertible_to_quarter { 0.25 };
  ASSERT_THAT(DOUBLE(half + convertible_to_quarter), DoubleEq(0.75));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeLeftAddedByAProbability) {
  probability_t quarter = 0.25;
  ASSERT_THAT(DOUBLE(quarter + half), DoubleEq(0.75));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeLeftAddedByAConvertibleToProbability) {
  Number<probability_t> convertible_to_quarter { 0.25 };
  ASSERT_THAT(DOUBLE(convertible_to_quarter + half), DoubleEq(0.75));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeAddedByItself) {
  ASSERT_THAT(DOUBLE(half + half), DoubleEq(1.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeRightAddedByAConvertibleToItself) {
  Number<probability_t> convertible_to_itself { half };
  ASSERT_THAT(DOUBLE(half + convertible_to_itself), DoubleEq(1.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeLeftAddedByAConvertibleToItself) {
  Number<probability_t> convertible_to_itself { half };
  ASSERT_THAT(DOUBLE(convertible_to_itself + half), DoubleEq(1.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanSubtractASmallerDouble) {
  ASSERT_THAT(DOUBLE(half - 0.25), DoubleEq(0.25));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanSubtractASmallerConvertibleToDouble) {
  Number<double> convertible_to_quarter { 0.25 };
  ASSERT_THAT(DOUBLE(half - convertible_to_quarter), DoubleEq(0.25));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanSubtractABiggerDouble) {
  ASSERT_DEATH(half - 0.75, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanSubtractABiggerConvertibleToDouble) {
  Number<double> convertible_to_three_quarters { 0.75 };
  ASSERT_DEATH(half - convertible_to_three_quarters, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeSubtractedFromASmallerDouble) {
  ASSERT_DEATH(0.25 - half, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeSubtractedFromASmallerConvertibleToDouble) {
  Number<double> convertible_to_quarter { 0.25 };
  ASSERT_DEATH(convertible_to_quarter - half, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeSubtractedFromABiggerDouble) {
  ASSERT_THAT(DOUBLE(0.75 - half), DoubleEq(0.25));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeSubtractedFromABiggerConvertibleToDouble) {
  Number<double> convertible_to_three_quarters { 0.75 };
  ASSERT_THAT(DOUBLE(convertible_to_three_quarters - half), DoubleEq(0.25));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeSubtractedByItself) {
  ASSERT_THAT(DOUBLE(half - half), DoubleEq(0.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanSubtractAConvertibleToItself) {
  Number<probability_t> convertible_to_itself { half };
  ASSERT_THAT(DOUBLE(half - convertible_to_itself), DoubleEq(0.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AProbabilityHalf, CanBeSubtractedFromAConvertibleToItself) {
  Number<probability_t> convertible_to_itself { half };
  ASSERT_THAT(DOUBLE(convertible_to_itself - half), DoubleEq(0.0));
}

/*----------------------------------------------------------------------------*/
/*                       TESTS FOR CONVERTIBLE TO P=0.5                       */
/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
    IsEqualToAnotherConvertibleToProbabilityHalf) {
  Number<probability_t> convertible_to_half2 { 0.5 };
  ASSERT_THAT(convertible_to_half, Eq(convertible_to_half2));
  ASSERT_THAT(convertible_to_half2, Eq(convertible_to_half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
    IsGreaterOrEqualToAnotherConvertibleToProbabilityHalf) {
  Number<probability_t> convertible_to_half2 { 0.5 };
  ASSERT_THAT(convertible_to_half, Ge(convertible_to_half2));
  ASSERT_THAT(convertible_to_half2, Le(convertible_to_half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
    IsLessOrEqualToAnotherConvertibleToProbabilityHalf) {
  Number<probability_t> convertible_to_half2 { 0.5 };
  ASSERT_THAT(convertible_to_half, Le(convertible_to_half2));
  ASSERT_THAT(convertible_to_half2, Ge(convertible_to_half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
    IsDifferentFromAnotherConvertibleToProbabilityAQuarter) {
  Number<probability_t> quarter { 0.25 };
  ASSERT_THAT(convertible_to_half, Ne(quarter));
  ASSERT_THAT(quarter, Ne(convertible_to_half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
    IsGreaterThanAnotherConvertibleToProbabilityAQuarter) {
  Number<probability_t> quarter { 0.25 };
  ASSERT_THAT(convertible_to_half, Gt(quarter));
  ASSERT_THAT(quarter, Le(convertible_to_half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
    IsGreaterOrEqualThanAnotherConvertibleToProbabilityAQuarter) {
  Number<probability_t> convertible_to_quarter { 0.25 };
  ASSERT_THAT(convertible_to_half, Ge(convertible_to_quarter));
  ASSERT_THAT(convertible_to_quarter, Lt(convertible_to_half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
    IsDifferentFromAnotherConvertibleToProbabilityThreeQuarters) {
  Number<probability_t> convertible_to_three_quarters { 0.75 };
  ASSERT_THAT(convertible_to_half, Ne(convertible_to_three_quarters));
  ASSERT_THAT(convertible_to_three_quarters, Ne(convertible_to_half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
    IsLessThanAnotherConvertibleToProbabilityThreeQuarters) {
  Number<probability_t> convertible_to_three_quarters { 0.75 };
  ASSERT_THAT(convertible_to_half, Lt(convertible_to_three_quarters));
  ASSERT_THAT(convertible_to_three_quarters, Ge(convertible_to_half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
    IsLessOrEqualThanAnotherConvertibleToProbabilityThreeQuarters) {
  Number<probability_t> convertible_to_three_quarters { 0.75 };
  ASSERT_THAT(convertible_to_half, Le(convertible_to_three_quarters));
  ASSERT_THAT(convertible_to_three_quarters, Gt(convertible_to_half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, IsGreaterThanADoubleAQuarter) {
  ASSERT_THAT(convertible_to_half, Gt(0.25));
  ASSERT_THAT(0.25, Le(convertible_to_half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
    IsGreaterThanAConvertibleToDoubleAQuarter) {
  Number<double> convertible_to_quarter { 0.25 };
  ASSERT_THAT(convertible_to_half, Gt(convertible_to_quarter));
  ASSERT_THAT(convertible_to_quarter, Le(convertible_to_half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, IsGreaterOrEqualThanADoubleAQuarter) {
  ASSERT_THAT(convertible_to_half, Ge(0.25));
  ASSERT_THAT(0.25, Lt(convertible_to_half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
    IsGreaterOrEqualThanAConvertibleToDoubleAQuarter) {
  Number<double> convertible_to_quarter { 0.25 };
  ASSERT_THAT(convertible_to_half, Ge(convertible_to_quarter));
  ASSERT_THAT(convertible_to_quarter, Lt(convertible_to_half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, IsDifferentFromADoubleThreeQuarters) {
  ASSERT_THAT(convertible_to_half, Ne(0.75));
  ASSERT_THAT(0.75, Ne(convertible_to_half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
    IsDifferentFromAConvertibleToDoubleThreeQuarters) {
  Number<double> convertible_to_three_quarters { 0.75 };
  ASSERT_THAT(convertible_to_half, Ne(convertible_to_three_quarters));
  ASSERT_THAT(convertible_to_three_quarters, Ne(convertible_to_half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, IsLessThanADoubleThreeQuarters) {
  ASSERT_THAT(convertible_to_half, Lt(0.75));
  ASSERT_THAT(0.75, Ge(convertible_to_half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
    IsLessThanAConvertibleToDoubleThreeQuarters) {
  Number<double> convertible_to_three_quarters { 0.75 };
  ASSERT_THAT(convertible_to_half, Lt(convertible_to_three_quarters));
  ASSERT_THAT(convertible_to_three_quarters, Ge(convertible_to_half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, IsLessOrEqualThanADoubleThreeQuarters) {
  ASSERT_THAT(convertible_to_half, Le(0.75));
  ASSERT_THAT(0.75, Gt(convertible_to_half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
    IsLessOrEqualThanAConvertibleToDoubleThreeQuarters) {
  Number<double> convertible_to_three_quarters { 0.75 };
  ASSERT_THAT(convertible_to_half, Le(convertible_to_three_quarters));
  ASSERT_THAT(convertible_to_three_quarters, Gt(convertible_to_half));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanBeRightMultipliedByADouble) {
  ASSERT_THAT(DOUBLE(convertible_to_half * 0.4), DoubleEq(0.2));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
    CanBeRightMultipliedByAConvertibleToDouble) {
  Number<double> convertible_to_quarter { 0.25 };
  ASSERT_THAT(DOUBLE(convertible_to_half * convertible_to_quarter),
              DoubleEq(0.125));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanBeLeftMultipliedByADouble) {
  ASSERT_THAT(DOUBLE(0.4 * convertible_to_half), DoubleEq(0.2));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
    CanBeLeftMultipliedByAConvertibleToDouble) {
  Number<double> convertible_to_quarter { 0.25 };
  ASSERT_THAT(DOUBLE(convertible_to_quarter * convertible_to_half),
              DoubleEq(0.125));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanBeRightMultipliedByAProbability) {
  probability_t quarter = 0.25;
  ASSERT_THAT(DOUBLE(convertible_to_half * quarter), DoubleEq(0.125));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
    CanBeRightMultipliedByAConvertibleToProbability) {
  Number<probability_t> convertible_to_quarter { 0.25 };
  ASSERT_THAT(DOUBLE(convertible_to_half * convertible_to_quarter),
              DoubleEq(0.125));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanBeLeftMultipliedByAProbability) {
  probability_t quarter = 0.25;
  ASSERT_THAT(DOUBLE(quarter * convertible_to_half), DoubleEq(0.125));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
       CanBeLeftMultipliedByAConvertibleToProbability) {
  Number<probability_t> convertible_to_quarter { 0.25 };
  ASSERT_THAT(DOUBLE(convertible_to_quarter * convertible_to_half),
              DoubleEq(0.125));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanBeMultipliedByItself) {
  ASSERT_THAT(DOUBLE(convertible_to_half * convertible_to_half),
              DoubleEq(0.25));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
       CanBeRightMultipliedByAConvertibleToItself) {
  Number<probability_t> convertible_to_itself { convertible_to_half };
  ASSERT_THAT(DOUBLE(convertible_to_half * convertible_to_itself),
              DoubleEq(0.25));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
    CanBeLeftMultipliedByAConvertibleToItself) {
  Number<probability_t> convertible_to_itself { convertible_to_half };
  ASSERT_THAT(DOUBLE(convertible_to_itself * convertible_to_half),
              DoubleEq(0.25));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanBeDividedByASmallerDouble) {
  ASSERT_DEATH(convertible_to_half / 0.4, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
    CanBeDividedByASmallerConvertibleToDouble) {
  Number<double> convertible_to_smaller { 0.4 };
  ASSERT_DEATH(convertible_to_half / convertible_to_smaller, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanBeDividedByABiggerDouble) {
  ASSERT_THAT(DOUBLE(convertible_to_half / 0.6), DoubleEq(0.5/0.6));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
    CanBeDividedByABiggerConvertibleToDouble) {
  Number<double> convertible_to_bigger { 0.6 };
  ASSERT_THAT(DOUBLE(convertible_to_half / convertible_to_bigger),
              DoubleEq(0.5/0.6));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanDivideASmallerDouble) {
  ASSERT_THAT(DOUBLE(0.4 / convertible_to_half), DoubleEq(0.4/0.5));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanDivideASmallerConvertibleToDouble) {
  Number<double> convertible_to_smaller { 0.4 };
  ASSERT_THAT(DOUBLE(convertible_to_smaller / convertible_to_half),
              DoubleEq(0.4/0.5));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanDivideABiggerDouble) {
  ASSERT_DEATH(0.6 / convertible_to_half, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanDivideABiggerConvertibleToDouble) {
  Number<double> convertible_to_bigger { 0.6 };
  ASSERT_DEATH(convertible_to_bigger / convertible_to_half, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanBeDividedByASmallerProbability) {
  probability_t probability = 0.4;
  ASSERT_DEATH(convertible_to_half / probability, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
    CanBeDividedByASmallerConvertibleToProbability) {
  Number<probability_t> convertible_to_smaller { 0.4 };
  ASSERT_DEATH(convertible_to_half / convertible_to_smaller, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanBeDividedByABiggerProbability) {
  probability_t probability = 0.6;
  ASSERT_THAT(DOUBLE(convertible_to_half / probability), DoubleEq(0.5/0.6));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
    CanBeDividedByABiggerConvertibleToProbability) {
  Number<probability_t> convertible_to_bigger { 0.6 };
  ASSERT_THAT(DOUBLE(convertible_to_half / convertible_to_bigger),
              DoubleEq(0.5/0.6));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanDivideASmallerProbability) {
  probability_t probability = 0.4;
  ASSERT_THAT(DOUBLE(probability / convertible_to_half), DoubleEq(0.4/0.5));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
    CanDivideASmallerConvertibleToProbability) {
  Number<probability_t> convertible_to_smaller { 0.4 };
  ASSERT_THAT(DOUBLE(convertible_to_smaller / convertible_to_half),
              DoubleEq(0.4/0.5));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanDivideABiggerProbability) {
  probability_t probability = 0.6;
  ASSERT_DEATH(probability / convertible_to_half, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
       CanDivideABiggerConvertibleToProbability) {
  Number<probability_t> convertible_to_bigger { 0.6 };
  ASSERT_DEATH(convertible_to_bigger / convertible_to_half, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanBeDividedByItself) {
  ASSERT_THAT(DOUBLE(convertible_to_half / convertible_to_half), DoubleEq(1.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanBeDividedByAConvertibleToItself) {
  Number<probability_t> convertible_to_itself { convertible_to_half };
  ASSERT_THAT(DOUBLE(convertible_to_half / convertible_to_itself),
              DoubleEq(1.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanDivideAConvertibleToItself) {
  Number<probability_t> convertible_to_itself { convertible_to_half };
  ASSERT_THAT(DOUBLE(convertible_to_itself / convertible_to_half),
              DoubleEq(1.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanBeRightAddedByADouble) {
  ASSERT_THAT(DOUBLE(convertible_to_half + 0.25), DoubleEq(0.75));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanBeRightAddedByAConvertibleToDouble) {
  Number<double> convertible_to_quarter { 0.25 };
  ASSERT_THAT(DOUBLE(convertible_to_half + convertible_to_quarter),
              DoubleEq(0.75));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanBeLeftAddedByADouble) {
  ASSERT_THAT(DOUBLE(0.25 + convertible_to_half), DoubleEq(0.75));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanBeLeftAddedByAConvertibleToDouble) {
  Number<double> convertible_to_quarter { 0.25 };
  ASSERT_THAT(DOUBLE(convertible_to_quarter + convertible_to_half),
              DoubleEq(0.75));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanBeRightAddedByAProbability) {
  probability_t quarter = 0.25;
  ASSERT_THAT(DOUBLE(convertible_to_half + quarter), DoubleEq(0.75));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
    CanBeRightAddedByAConvertibleToProbability) {
  Number<probability_t> convertible_to_quarter { 0.25 };
  ASSERT_THAT(DOUBLE(convertible_to_half + convertible_to_quarter),
              DoubleEq(0.75));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanBeLeftAddedByAProbability) {
  probability_t quarter = 0.25;
  ASSERT_THAT(DOUBLE(quarter + convertible_to_half), DoubleEq(0.75));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
    CanBeLeftAddedByAConvertibleToProbability) {
  Number<probability_t> convertible_to_quarter { 0.25 };
  ASSERT_THAT(DOUBLE(convertible_to_quarter + convertible_to_half),
              DoubleEq(0.75));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanBeAddedByItself) {
  ASSERT_THAT(DOUBLE(convertible_to_half + convertible_to_half), DoubleEq(1.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanBeRightAddedByAConvertibleToItself) {
  Number<probability_t> convertible_to_itself { convertible_to_half };
  ASSERT_THAT(DOUBLE(convertible_to_half + convertible_to_itself),
              DoubleEq(1.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanBeLeftAddedByAConvertibleToItself) {
  Number<probability_t> convertible_to_itself { convertible_to_half };
  ASSERT_THAT(DOUBLE(convertible_to_itself + convertible_to_half),
              DoubleEq(1.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanSubtractASmallerDouble) {
  ASSERT_THAT(DOUBLE(convertible_to_half - 0.25), DoubleEq(0.25));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanSubtractASmallerConvertibleToDouble) {
  Number<double> convertible_to_quarter { 0.25 };
  ASSERT_THAT(DOUBLE(convertible_to_half - convertible_to_quarter),
              DoubleEq(0.25));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanSubtractABiggerDouble) {
  ASSERT_DEATH(convertible_to_half - 0.75, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanSubtractABiggerConvertibleToDouble) {
  Number<double> convertible_to_three_quarters { 0.75 };
  ASSERT_DEATH(convertible_to_half - convertible_to_three_quarters, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanBeSubtractedFromASmallerDouble) {
  ASSERT_DEATH(0.25 - convertible_to_half, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
    CanBeSubtractedFromASmallerConvertibleToDouble) {
  Number<double> convertible_to_quarter { 0.25 };
  ASSERT_DEATH(convertible_to_quarter - convertible_to_half, "");
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanBeSubtractedFromABiggerDouble) {
  ASSERT_THAT(DOUBLE(0.75 - convertible_to_half), DoubleEq(0.25));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
    CanBeSubtractedFromABiggerConvertibleToDouble) {
  Number<double> convertible_to_three_quarters { 0.75 };
  ASSERT_THAT(DOUBLE(convertible_to_three_quarters - convertible_to_half),
              DoubleEq(0.25));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanBeSubtractedByItself) {
  ASSERT_THAT(DOUBLE(convertible_to_half - convertible_to_half),
              DoubleEq(0.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf, CanSubtractAConvertibleToItself) {
  Number<probability_t> convertible_to_itself { convertible_to_half };
  ASSERT_THAT(DOUBLE(convertible_to_half - convertible_to_itself),
              DoubleEq(0.0));
}

/*----------------------------------------------------------------------------*/

TEST_F(AConvertibleToProbabilityHalf,
    CanBeSubtractedFromAConvertibleToItself) {
  Number<probability_t> convertible_to_itself { convertible_to_half };
  ASSERT_THAT(DOUBLE(convertible_to_itself - convertible_to_half),
              DoubleEq(0.0));
}
/*----------------------------------------------------------------------------*/
