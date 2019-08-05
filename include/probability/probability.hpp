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

#ifndef PROBABILITY_PROBABILITY_
#define PROBABILITY_PROBABILITY_

// Standard headers
#include <cmath>
#include <limits>
#include <cassert>
#include <algorithm>
#include <type_traits>

// Macros
// Based on http://cnicholson.net/2009/02/stupid-c-tricks-adventures-in-assert/
#define UNUSED(x) do { (void) sizeof(x); } while (false)

namespace probability {

// Forward declaration
template<typename T> class EmptyChecker;
template<typename T, std::size_t ulp> class ProbabilityChecker;

template<typename T, std::size_t ulp = 0, typename C = EmptyChecker<T>>
class LogFloatingPoint;

// Aliases
using log_float_t = LogFloatingPoint<float>;
using log_double_t = LogFloatingPoint<double>;
using log_long_double_t = LogFloatingPoint<long double>;

template<typename T, std::size_t ulp = 0>
using Probability = LogFloatingPoint<T, ulp, ProbabilityChecker<T, ulp>>;

using probability_float_t = Probability<float>;
using probability_double_t = Probability<double>;
using probability_long_double_t = Probability<long double>;

using probability_t = probability_double_t;

/**
 * @class LogFloatingPoint
 * @tparam T Value type, used for internal store
 * @tparam ulp Units in the last place, defining the accuracy
 * @tparam C Checker type, used to inject methods that verify consistency
 * @brief Fast implementation of floats using logarithms
 */
template<typename T, std::size_t ulp, typename C>
class LogFloatingPoint {
 public:
  using value_type = T;
  using checker_type = C;

  // Constructors
  LogFloatingPoint() = default;

  LogFloatingPoint(value_type v) : value(std::log(v)) {
    assert(v >= 0.0);
    check_initial_value(v);
  }

  template<typename U,
    typename std::enable_if_t<
      !std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>, void>* = nullptr>
  LogFloatingPoint(const U& v)
      : LogFloatingPoint(static_cast<const value_type&>(v)) {
  }

  template<typename RhsT, std::size_t RhsUlp, typename RhsC>
  LogFloatingPoint(const LogFloatingPoint<RhsT, RhsUlp, RhsC>& v)
      : value(v.data()) {
    check_range();
  }

  // Operator overloads
  explicit operator value_type() const noexcept {
    return std::exp(value);
  }

  LogFloatingPoint& operator+=(const LogFloatingPoint& rhs) noexcept {
    if (rhs.data() == -infinity) {
      // Do nothing: summing with 0
    } else if (value == -infinity) {
      value = rhs.data();  // probability is 0: just attributes
    } else if (value >= rhs.data()) {
      value += std::log1p(std::exp(rhs.data() - value));
      check_range();
    } else {
      value = rhs.data() + std::log1p(std::exp(value - rhs.data()));
      check_range();
    }
    return *this;
  }

  LogFloatingPoint& operator-=(const LogFloatingPoint& rhs) noexcept {
    if (rhs.data() == -infinity) {
      // Do nothing: subtracting by 0
    } else if (value == -infinity) {
      // probability is 0: cannot be negative
      assert(false);  // LCOV_EXCL_LINE (not counted due to fork() in GTest)
    } else {
      assert(value >= rhs.data());
      value += std::log1p(-std::exp(rhs.data() - value));
      check_range();
    }
    return *this;
  }

  LogFloatingPoint& operator*=(const LogFloatingPoint& rhs) noexcept {
    value += rhs.data();
    check_range();
    return *this;
  }

  LogFloatingPoint& operator/=(const LogFloatingPoint& rhs) noexcept {
    value -= rhs.data();
    check_range();
    return *this;
  }

  bool operator==(const LogFloatingPoint& rhs) const noexcept {
    return value == rhs.data();
  }

  bool operator!=(const LogFloatingPoint& rhs) const noexcept {
    return !operator==(rhs);
  }

  bool operator<(const LogFloatingPoint& rhs) const noexcept {
    return value < rhs.data();
  }

  bool operator<=(const LogFloatingPoint& rhs) const noexcept {
    return operator<(rhs) || operator==(rhs);
  }

  bool operator>(const LogFloatingPoint& rhs) const noexcept {
    return !operator<=(rhs);
  }

  bool operator>=(const LogFloatingPoint& rhs) const noexcept {
    return !operator<(rhs);
  }

  // Concrete methods
  value_type& data() noexcept {
    return value;
  }

  const value_type& data() const noexcept {
    return value;
  }

 private:
  // Validation
  static_assert(ulp <= std::numeric_limits<value_type>::digits,
      "Units in the last place cannot be bigger than the maximum "
      "number of digits in the mantissa");

  // Static variables
  static constexpr auto infinity
    = std::numeric_limits<value_type>::infinity();

  // Instance variables
  value_type value = -infinity;

  // Concrete methods
  void check_initial_value(value_type v) {
    checker_type::check_initial_value(v);
  }

  void check_range() {
    checker_type::check_range(value);
  }
};

// Operator overloads
template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    !std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>,
  void>* = nullptr>
inline bool operator==(const U& lhs,
                       const LogFloatingPoint<T, ulp, C>& rhs) noexcept {
  return std::log(static_cast<const T&>(lhs)) == rhs.data();
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>, void>* = nullptr>
inline bool operator==(const U& lhs,
                       const LogFloatingPoint<T, ulp, C>& rhs) noexcept {
  return static_cast<const LogFloatingPoint<T, ulp, C>&>(lhs) == rhs;
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    !std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>,
  void>* = nullptr>
inline bool operator!=(const U& lhs,
                       const LogFloatingPoint<T, ulp, C>& rhs) noexcept {
  return !operator==(lhs, rhs);
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>, void>* = nullptr>
inline bool operator!=(const U& lhs,
                       const LogFloatingPoint<T, ulp, C>& rhs) noexcept {
  return static_cast<const LogFloatingPoint<T, ulp, C>&>(lhs) != rhs;
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    !std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>,
  void>* = nullptr>
inline bool operator<(const U& lhs,
                      const LogFloatingPoint<T, ulp, C>& rhs) noexcept {
  return std::log(static_cast<const T&>(lhs)) < rhs.data();
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>, void>* = nullptr>
inline bool operator<(const U& lhs,
                      const LogFloatingPoint<T, ulp, C>& rhs) noexcept {
  return static_cast<const LogFloatingPoint<T, ulp, C>&>(lhs) < rhs;
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    !std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>,
  void>* = nullptr>
inline bool operator<=(const U& lhs,
                       const LogFloatingPoint<T, ulp, C>& rhs) noexcept {
  return operator<(lhs, rhs) || operator==(lhs, rhs);
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>, void>* = nullptr>
inline bool operator<=(const U& lhs,
                      const LogFloatingPoint<T, ulp, C>& rhs) noexcept {
  return static_cast<const LogFloatingPoint<T, ulp, C>&>(lhs) <= rhs;
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    !std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>,
  void>* = nullptr>
inline bool operator>(const U& lhs,
                      const LogFloatingPoint<T, ulp, C>& rhs) noexcept {
  return !operator<=(lhs, rhs);
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>, void>* = nullptr>
inline bool operator>(const U& lhs,
                      const LogFloatingPoint<T, ulp, C>& rhs) noexcept {
  return static_cast<const LogFloatingPoint<T, ulp, C>&>(lhs) > rhs;
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    !std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>,
  void>* = nullptr>
inline bool operator>=(const U& lhs,
                       const LogFloatingPoint<T, ulp, C>& rhs) noexcept {
  return !operator<(lhs, rhs);
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>, void>* = nullptr>
inline bool operator>=(const U& lhs,
                       const LogFloatingPoint<T, ulp, C>& rhs) noexcept {
  return static_cast<const LogFloatingPoint<T, ulp, C>&>(lhs) >= rhs;
}

/**/

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    !std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>,
  void>* = nullptr>
inline bool operator==(const LogFloatingPoint<T, ulp, C>& lhs,
                       const U& rhs) noexcept {
  return lhs.data() == std::log(rhs);
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>, void>* = nullptr>
inline bool operator==(const LogFloatingPoint<T, ulp, C>& lhs,
                       const U& rhs) noexcept {
  return lhs == static_cast<const LogFloatingPoint<T, ulp, C>&>(rhs);
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    !std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>,
  void>* = nullptr>
inline bool operator!=(const LogFloatingPoint<T, ulp, C>& lhs,
                       const U& rhs) noexcept {
  return !operator==(lhs, rhs);
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>, void>* = nullptr>
inline bool operator!=(const LogFloatingPoint<T, ulp, C>& lhs,
                       const U& rhs) noexcept {
  return lhs != static_cast<const LogFloatingPoint<T, ulp, C>&>(rhs);
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    !std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>,
  void>* = nullptr>
inline bool operator<(const LogFloatingPoint<T, ulp, C>& lhs,
                      const U& rhs) noexcept {
  return lhs.data() < std::log(static_cast<const T&>(rhs));
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>, void>* = nullptr>
inline bool operator<(const LogFloatingPoint<T, ulp, C>& lhs,
                      const U& rhs) noexcept {
  return lhs < static_cast<const LogFloatingPoint<T, ulp, C>&>(rhs);
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    !std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>,
  void>* = nullptr>
inline bool operator<=(const LogFloatingPoint<T, ulp, C>& lhs,
                       const U& rhs) noexcept {
  return operator<(lhs, rhs) || operator==(lhs, rhs);
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>, void>* = nullptr>
inline bool operator<=(const LogFloatingPoint<T, ulp, C>& lhs,
                       const U& rhs) noexcept {
  return lhs <= static_cast<const LogFloatingPoint<T, ulp, C>&>(rhs);
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    !std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>,
  void>* = nullptr>
inline bool operator>(const LogFloatingPoint<T, ulp, C>& lhs,
                      const U& rhs) noexcept {
  return !operator<=(lhs, rhs);
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>, void>* = nullptr>
inline bool operator>(const LogFloatingPoint<T, ulp, C>& lhs,
                      const U& rhs) noexcept {
  return lhs > static_cast<const LogFloatingPoint<T, ulp, C>&>(rhs);
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    !std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>,
  void>* = nullptr>
inline bool operator>=(const LogFloatingPoint<T, ulp, C>& lhs,
                       const U& rhs) noexcept {
  return !operator<(lhs, rhs);
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>, void>* = nullptr>
inline bool operator>=(const LogFloatingPoint<T, ulp, C>& lhs,
                       const U& rhs) noexcept {
  return lhs >= static_cast<const LogFloatingPoint<T, ulp, C>&>(rhs);
}

/**/

template<typename T, std::size_t ulp, typename C>
inline LogFloatingPoint<T, ulp, C>
operator*(LogFloatingPoint<T, ulp, C> lhs,
          const LogFloatingPoint<T, ulp, C>& rhs) noexcept {
  lhs *= rhs;
  return lhs;
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    !std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>, void>* = nullptr>
inline LogFloatingPoint<T, ulp, C>
operator*(const LogFloatingPoint<T, ulp, C>& lhs, const U& rhs) noexcept {
  return { static_cast<const T&>(lhs) * static_cast<const T&>(rhs) };
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>, void>* = nullptr>
inline LogFloatingPoint<T, ulp, C>
operator*(const LogFloatingPoint<T, ulp, C>& lhs, const U& rhs) noexcept {
  return lhs * static_cast<const LogFloatingPoint<T, ulp, C>&>(rhs);
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    !std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>, void>* = nullptr>
inline LogFloatingPoint<T, ulp, C>
operator*(const U& lhs, const LogFloatingPoint<T, ulp, C>& rhs) noexcept {
  return { static_cast<const T&>(lhs) * static_cast<const T&>(rhs) };
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>, void>* = nullptr>
inline LogFloatingPoint<T, ulp, C>
operator*(const U& lhs, const LogFloatingPoint<T, ulp, C>& rhs) noexcept {
  return static_cast<const LogFloatingPoint<T, ulp, C>&>(lhs) * rhs;
}

/**/

template<typename T, std::size_t ulp, typename C>
inline LogFloatingPoint<T, ulp, C>
operator/(LogFloatingPoint<T, ulp, C> lhs,
          const LogFloatingPoint<T, ulp, C>& rhs) noexcept {
  lhs /= rhs;
  return lhs;
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    !std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>,
  void>* = nullptr>
inline LogFloatingPoint<T, ulp, C>
operator/(const LogFloatingPoint<T, ulp, C>& lhs, const U& rhs) noexcept {
  return { static_cast<const T&>(lhs) / static_cast<const T&>(rhs) };
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>, void>* = nullptr>
inline LogFloatingPoint<T, ulp, C>
operator/(const LogFloatingPoint<T, ulp, C>& lhs, const U& rhs) noexcept {
  return lhs / static_cast<const LogFloatingPoint<T, ulp, C>&>(rhs);
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    !std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>,
  void>* = nullptr>
inline LogFloatingPoint<T, ulp, C>
operator/(const U& lhs, const LogFloatingPoint<T, ulp, C>& rhs) noexcept {
  return { static_cast<const T&>(lhs) / static_cast<const T&>(rhs) };
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>, void>* = nullptr>
inline LogFloatingPoint<T, ulp, C>
operator/(const U& lhs, const LogFloatingPoint<T, ulp, C>& rhs) noexcept {
  return static_cast<const LogFloatingPoint<T, ulp, C>&>(lhs) / rhs;
}

/**/

template<typename T, std::size_t ulp, typename C>
inline LogFloatingPoint<T, ulp, C>
operator+(LogFloatingPoint<T, ulp, C> lhs,
          const LogFloatingPoint<T, ulp, C>& rhs) noexcept {
  lhs += rhs;
  return lhs;
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    !std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>,
  void>* = nullptr>
inline LogFloatingPoint<T, ulp, C>
operator+(LogFloatingPoint<T, ulp, C> lhs, const U& rhs) noexcept {
  return lhs + LogFloatingPoint<T, ulp, C>(rhs);
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>, void>* = nullptr>
inline LogFloatingPoint<T, ulp, C>
operator+(LogFloatingPoint<T, ulp, C> lhs, const U& rhs) noexcept {
  return lhs + static_cast<const LogFloatingPoint<T, ulp, C>&>(rhs);
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    !std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>,
  void>* = nullptr>
inline LogFloatingPoint<T, ulp, C>
operator+(const U& lhs, const LogFloatingPoint<T, ulp, C>& rhs) noexcept {
  LogFloatingPoint<T, ulp, C> result(static_cast<const T&>(lhs));
  result += rhs;
  return result;
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>, void>* = nullptr>
inline LogFloatingPoint<T, ulp, C>
operator+(const U& lhs, const LogFloatingPoint<T, ulp, C>& rhs) noexcept {
  return static_cast<const LogFloatingPoint<T, ulp, C>&>(lhs) + rhs;
}

/**/

template<typename T, std::size_t ulp, typename C>
inline LogFloatingPoint<T, ulp, C>
operator-(LogFloatingPoint<T, ulp, C> lhs,
          const LogFloatingPoint<T, ulp, C>& rhs) noexcept {
  lhs -= rhs;
  return lhs;
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    !std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>,
  void>* = nullptr>
inline LogFloatingPoint<T, ulp, C>
operator-(LogFloatingPoint<T, ulp, C> lhs, const U& rhs) noexcept {
  return lhs - LogFloatingPoint<T, ulp, C>(static_cast<const T&>(rhs));
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>, void>* = nullptr>
inline LogFloatingPoint<T, ulp, C>
operator-(LogFloatingPoint<T, ulp, C> lhs, const U& rhs) noexcept {
  return lhs - static_cast<const LogFloatingPoint<T, ulp, C>&>(rhs);
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    !std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>,
  void>* = nullptr>
inline LogFloatingPoint<T, ulp, C>
operator-(const U& lhs, const LogFloatingPoint<T, ulp, C>& rhs) noexcept {
  LogFloatingPoint<T, ulp, C> result(static_cast<const T&>(lhs));
  result -= rhs;
  return result;
}

template<typename U, typename T, std::size_t ulp, typename C,
  typename std::enable_if_t<
    std::is_convertible_v<U, LogFloatingPoint<T, ulp, C>>, void>* = nullptr>
inline LogFloatingPoint<T, ulp, C>
operator-(const U& lhs, const LogFloatingPoint<T, ulp, C>& rhs) noexcept {
  return static_cast<const LogFloatingPoint<T, ulp, C>&>(lhs) - rhs;
}

/**
 * @class EmptyChecker
 * @brief Imposes no restrictions on values in LogFloatingPoint
 */
template<typename T>
class EmptyChecker {
 public:
  // Aliases
  using value_type = T;

  // Concrete methods
  static void check_initial_value(value_type /* v */) {
  }

  static void check_range(value_type /* value */) {
  }
};

/**
 * @class ProbabilityChecker
 * @brief Requires values in LogFloatingPoint to be probabilities
 */
template<typename T, std::size_t ulp>
class ProbabilityChecker {
 public:
  // Aliases
  using value_type = T;

  // Concrete methods
  static void check_initial_value(value_type v) {
    assert(v <= 1.0);
    UNUSED(v);  // Avoid 'unused variable' warning when 'assert' is disabled
  }

  static void check_range(value_type value) {
    assert(value <= limit);
    UNUSED(value);  // Avoid 'unused variable' warning when 'assert' is disabled
  }

 private:
  // Static variables
  static constexpr auto limit
    = std::numeric_limits<value_type>::epsilon() * (1 << ulp);
};

}  // namespace probability

// Macros
#undef UNUSED

#endif  // PROBABILITY_PROBABILITY_
