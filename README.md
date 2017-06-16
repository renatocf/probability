# Probability

[![Travis](https://img.shields.io/travis/renatocf/probability.svg)](https://travis-ci.org/renatocf/probability)
[![Codecov](https://img.shields.io/codecov/c/github/renatocf/probability.svg)](https://codecov.io/gh/renatocf/probability)

A fast implementation of probabilities / positive floating point arithmetic using logarithms.

## Usage

This library is header only, which means you can copy it and use it with a simple include:
```c++
#include "probability/probability.hpp"
```

We provide aliases for the most common uses:

| Type                        | Definition                                             |
| --------------------------- | ------------------------------------------------------ |
| `log_float_t`               | Logarithm floating point with value type `float`       |
| `log_double_t`              | Logarithm floating point with value type `double`      |
| `log_long_double_t`         | Logarithm floating point with value type `long double` |
| `probability_float_t`       | Probability with base type `float`                     |
| `probability_double_t`      | Probability with base type `double`                    |
| `probability_long_double_t` | Probability with base type `long double`               |
| `probability_t`             | Alias to `probability_double_t`                        |

The library defines a class `LogFloatingPoint` with 3 template parameters:
- `T`, the value type, used for internal storage
- `ulp`, the [units in the last place](https://en.wikipedia.org/wiki/Unit_in_the_last_place), used to define the precision of comparisons.
- `C`, the checker type, used to inject methods that verify consistency; the library provides two standard checkers: `EmptyChecker` (for the `log_*_t` types above) and `ProbabilityChecker` (for the `probability_*_t` types above).

By default, all aliases above have `ulp = 0` (meaning that the precision equals the [machine epsilon](http://en.cppreference.com/w/cpp/types/numeric_limits/epsilon) of the value type).
