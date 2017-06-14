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

namespace probability {

// Forward declaration
template<typename T, std::size_t ulp = 0> class LogFloatingPoint;

// Aliases
using float_probability_t = LogFloatingPoint<float>;
using double_probability_t = LogFloatingPoint<double>;
using probability_t = double_probability_t;

/**
 * @class LogFloatingPoint
 * @brief Fast implementation of floats using logarithms
 */
template<typename T, std::size_t ulp>
class LogFloatingPoint {
 public:
  using value_type = T;

  // Constructors
  LogFloatingPoint() = default;

  LogFloatingPoint(value_type v) : value(std::log(v)) {
    assert(v >= 0.0 && v <= 1.0);
  }

  // Operator overloads
  operator value_type() const noexcept {
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

  static constexpr auto limit
    = std::numeric_limits<value_type>::epsilon() * (1 << ulp);

  // Instance variables
  value_type value = -infinity;

  // Concrete methods
  void check_range() {
    assert(value <= limit);
  }
};

// Operator overloads
template<typename T, std::size_t ulp>
inline bool operator==(const T& lhs,
                       const LogFloatingPoint<T, ulp>& rhs) noexcept {
  return std::log(lhs) == rhs.data();
}

template<typename T, std::size_t ulp>
inline bool operator!=(const T& lhs,
                       const LogFloatingPoint<T, ulp>& rhs) noexcept {
  return !operator==(lhs, rhs);
}

template<typename T, std::size_t ulp>
inline bool operator<(const T& lhs,
                      const LogFloatingPoint<T, ulp>& rhs) noexcept {
  return std::log(lhs) < rhs.data();
}

template<typename T, std::size_t ulp>
inline bool operator<=(const T& lhs,
                       const LogFloatingPoint<T, ulp>& rhs) noexcept {
  return operator<(lhs, rhs) || operator==(lhs, rhs);
}

template<typename T, std::size_t ulp>
inline bool operator>(const T& lhs,
                      const LogFloatingPoint<T, ulp>& rhs) noexcept {
  return !operator<=(lhs, rhs);
}

template<typename T, std::size_t ulp>
inline bool operator>=(const T& lhs,
                       const LogFloatingPoint<T, ulp>& rhs) noexcept {
  return !operator<(lhs, rhs);
}

/**/

template<typename T, std::size_t ulp>
inline bool operator==(const LogFloatingPoint<T, ulp>& lhs,
                       const T& rhs) noexcept {
  return lhs.data() == std::log(rhs);
}

template<typename T, std::size_t ulp>
inline bool operator!=(const LogFloatingPoint<T, ulp>& lhs,
                       const T& rhs) noexcept {
  return !operator==(lhs, rhs);
}

template<typename T, std::size_t ulp>
inline bool operator<(const LogFloatingPoint<T, ulp>& lhs,
                      const T& rhs) noexcept {
  return lhs.data() < std::log(rhs);
}

template<typename T, std::size_t ulp>
inline bool operator<=(const LogFloatingPoint<T, ulp>& lhs,
                       const T& rhs) noexcept {
  return operator<(lhs, rhs) || operator==(lhs, rhs);
}

template<typename T, std::size_t ulp>
inline bool operator>(const LogFloatingPoint<T, ulp>& lhs,
                      const T& rhs) noexcept {
  return !operator<=(lhs, rhs);
}

template<typename T, std::size_t ulp>
inline bool operator>=(const LogFloatingPoint<T, ulp>& lhs,
                       const T& rhs) noexcept {
  return !operator<(lhs, rhs);
}

/**/

template<typename T, std::size_t ulp>
inline LogFloatingPoint<T, ulp>
operator*(LogFloatingPoint<T, ulp> lhs,
          const LogFloatingPoint<T, ulp>& rhs) noexcept {
  lhs *= rhs;
  return lhs;
}

template<typename T, std::size_t ulp>
inline LogFloatingPoint<T, ulp>
operator*(const LogFloatingPoint<T, ulp>& lhs, const T& rhs) noexcept {
  return { static_cast<T>(lhs) * static_cast<T>(rhs) };
}

template<typename T, std::size_t ulp>
inline LogFloatingPoint<T, ulp>
operator*(const T& lhs, const LogFloatingPoint<T, ulp>& rhs) noexcept {
  return { static_cast<T>(lhs) * static_cast<T>(rhs) };
}

/**/

template<typename T, std::size_t ulp>
inline LogFloatingPoint<T, ulp>
operator/(LogFloatingPoint<T, ulp> lhs,
          const LogFloatingPoint<T, ulp>& rhs) noexcept {
  lhs /= rhs;
  return lhs;
}

template<typename T, std::size_t ulp>
inline LogFloatingPoint<T, ulp>
operator/(const LogFloatingPoint<T, ulp>& lhs, const T& rhs) noexcept {
  return { static_cast<T>(lhs) / static_cast<T>(rhs) };
}

template<typename T, std::size_t ulp>
inline LogFloatingPoint<T, ulp>
operator/(const T& lhs, const LogFloatingPoint<T, ulp>& rhs) noexcept {
  return { static_cast<T>(lhs) / static_cast<T>(rhs) };
}

/**/

template<typename T, std::size_t ulp>
inline LogFloatingPoint<T, ulp>
operator+(LogFloatingPoint<T, ulp> lhs,
          const LogFloatingPoint<T, ulp>& rhs) noexcept {
  lhs += rhs;
  return lhs;
}

template<typename T, std::size_t ulp>
inline LogFloatingPoint<T, ulp>
operator+(LogFloatingPoint<T, ulp> lhs, const T& rhs) noexcept {
  return operator+(lhs, LogFloatingPoint<T, ulp>(rhs));
}

template<typename T, std::size_t ulp>
inline LogFloatingPoint<T, ulp>
operator+(const T& lhs, const LogFloatingPoint<T, ulp>& rhs) noexcept {
  LogFloatingPoint<T, ulp> result(lhs);
  result += rhs;
  return result;
}

/**/

template<typename T, std::size_t ulp>
inline LogFloatingPoint<T, ulp>
operator-(LogFloatingPoint<T, ulp> lhs,
          const LogFloatingPoint<T, ulp>& rhs) noexcept {
  lhs -= rhs;
  return lhs;
}

template<typename T, std::size_t ulp>
inline LogFloatingPoint<T, ulp>
operator-(LogFloatingPoint<T, ulp> lhs, const T& rhs) noexcept {
  return operator-(lhs, LogFloatingPoint<T, ulp>(rhs));
}

template<typename T, std::size_t ulp>
inline LogFloatingPoint<T, ulp>
operator-(const T& lhs, const LogFloatingPoint<T, ulp>& rhs) noexcept {
  LogFloatingPoint<T, ulp> result(lhs);
  result -= rhs;
  return result;
}

}  // namespace probability

#endif  // PROBABILITY_PROBABILITY_
