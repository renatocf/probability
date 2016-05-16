/******************************************************************************/
/*  Probability - A fast implementation of probabilities with log space       */
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
#include <limits>
#include <cassert>
#include <algorithm>

namespace probability {

class probability_t {
 public:
  using value_type = double;

  // Constructors
  probability_t(value_type v = 0.0) : value(std::log(v)) {
    assert(v >= 0.0 && v <= 1.0);
  }

  // Operator overloads
  operator value_type() const noexcept {
    return std::exp(value);
  }

  probability_t& operator+=(const probability_t& rhs) {
    if (*rhs.data() == -infinity) {
      // Do nothing: summing with 0
    } else if (value == -infinity) {
      value = *rhs.data();  // probability is 0: just attributes
    } else {
      const auto& max = std::max(value, *rhs.data());
      const auto& min = std::min(value, *rhs.data());
      value = max + std::log1p(std::exp(min - max));
      check_range();
    }
    return *this;
  }

  probability_t& operator-=(const probability_t& rhs) {
    // assert(value != -infinity);
    if (*rhs.data() == -infinity) {
      // Do nothing: subtracting by 0
    } else if (value == -infinity) {
      // probability is 0: cannot be negative
      assert(false);  // LCOV_EXCL_LINE (not counted due to fork() in GTest)
    } else {
      assert(value >= *rhs.data());
      const auto& max = std::max(value, *rhs.data());
      const auto& min = std::min(value, *rhs.data());
      value = max + std::log1p(-std::exp(min - max));
      check_range();
    }
    return *this;
  }

  probability_t& operator*=(const probability_t& rhs) {
    value += *rhs.data();
    check_range();
    return *this;
  }

  probability_t& operator/=(const probability_t& rhs) {
    value -= *rhs.data();
    check_range();
    return *this;
  }

  bool operator==(const probability_t& rhs) const noexcept {
    return value == *rhs.data();
  }

  bool operator!=(const probability_t& rhs) const noexcept {
    return !operator==(rhs);
  }

  bool operator<(const probability_t& rhs) const noexcept {
    return value < *rhs.data();
  }

  bool operator<=(const probability_t& rhs) const noexcept {
    return operator<(rhs) || operator==(rhs);
  }

  bool operator>(const probability_t& rhs) const noexcept {
    return !operator<=(rhs);
  }

  bool operator>=(const probability_t& rhs) const noexcept {
    return !operator<(rhs);
  }

  // Concrete methods
  value_type* data() noexcept {
    return &value;
  }

  const value_type* data() const noexcept {
    return &value;
  }

 private:
  // Static variables
  static constexpr auto infinity = std::numeric_limits<value_type>::infinity();

  // Instance variables
  value_type value = 0.0;

  // Concrete methods
  void check_range() {
    assert(value <= 0 || std::isnan(value));
  }
};

// Operator overloads
bool operator==(const double& lhs, const probability_t& rhs) noexcept {
  return std::log(lhs) == *rhs.data();
}

bool operator!=(const double& lhs, const probability_t& rhs) noexcept {
  return !operator==(lhs, rhs);
}

bool operator<(const double& lhs, const probability_t& rhs) noexcept {
  return std::log(lhs) < *rhs.data();
}

bool operator<=(const double& lhs, const probability_t& rhs) noexcept {
  return operator<(lhs, rhs) || operator==(lhs, rhs);
}

bool operator>(const double& lhs, const probability_t& rhs) noexcept {
  return !operator<=(lhs, rhs);
}

bool operator>=(const double& lhs, const probability_t& rhs) noexcept {
  return !operator<(lhs, rhs);
}

/**/

bool operator==(const probability_t& lhs, const double& rhs) noexcept {
  return *lhs.data() == std::log(rhs);
}

bool operator!=(const probability_t& lhs, const double& rhs) noexcept {
  return !operator==(lhs, rhs);
}

bool operator<(const probability_t& lhs, const double& rhs) noexcept {
  return *lhs.data() < std::log(rhs);
}

bool operator<=(const probability_t& lhs, const double& rhs) noexcept {
  return operator<(lhs, rhs) || operator==(lhs, rhs);
}

bool operator>(const probability_t& lhs, const double& rhs) noexcept {
  return !operator<=(lhs, rhs);
}

bool operator>=(const probability_t& lhs, const double& rhs) noexcept {
  return !operator<(lhs, rhs);
}

/**/

probability_t operator*(probability_t lhs, const probability_t& rhs) {
  lhs *= rhs;
  return lhs;
}

probability_t operator*(const probability_t& lhs, const double& rhs) {
  return { static_cast<double>(lhs) * static_cast<double>(rhs) };
}

probability_t operator*(const double& lhs, const probability_t& rhs) {
  return { static_cast<double>(lhs) * static_cast<double>(rhs) };
}

/**/

probability_t operator/(probability_t lhs, const probability_t& rhs) {
  lhs /= rhs;
  return lhs;
}

probability_t operator/(const probability_t& lhs, const double& rhs) {
  return { static_cast<double>(lhs) / static_cast<double>(rhs) };
}

probability_t operator/(const double& lhs, const probability_t& rhs) {
  return { static_cast<double>(lhs) / static_cast<double>(rhs) };
}

/**/

probability_t operator+(probability_t lhs, const probability_t& rhs) {
  lhs += rhs;
  return lhs;
}

probability_t operator+(probability_t lhs, const double& rhs) {
  return operator+(lhs, probability_t(rhs));
}

probability_t operator+(const double& lhs, const probability_t& rhs) {
  probability_t result(lhs);
  result += rhs;
  return result;
}

/**/

probability_t operator-(probability_t lhs, const probability_t& rhs) {
  lhs -= rhs;
  return lhs;
}

probability_t operator-(probability_t lhs, const double& rhs) {
  return operator-(lhs, probability_t(rhs));
}

probability_t operator-(const double& lhs, const probability_t& rhs) {
  probability_t result(lhs);
  result -= rhs;
  return result;
}

}  // namespace probability

#endif  // PROBABILITY_PROBABILITY_
