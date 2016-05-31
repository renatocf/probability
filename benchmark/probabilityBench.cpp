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
#include <vector>
#include <cmath>

// External headers
#include "benchmark/benchmark.h"

// Probability header
#include "probability.hpp"

double log_sum(double log_a, double log_b) {
  if (log_a > log_b) {
    return log_a + log1p(exp(log_b - log_a));
  } else {
    return log_b + log1p(exp(log_a - log_b));
  }
}

static void BM_ForwardAlgorithmWithoutProbability(benchmark::State& state) {
  while (state.KeepRunning()) {
    auto state_alphabet_size = 10;
    auto sequence_size = state.range_x();

    auto alpha = std::vector<std::vector<double>>(
        state_alphabet_size,
        std::vector<double>(sequence_size));

    auto prob = std::log(0.000000000005);

    for (int k = 0; k < state_alphabet_size; k++)
      alpha[k][0] = prob + prob;

    for (int t = 0; t < sequence_size - 1; t++) {
      for (int i = 0; i < state_alphabet_size; i++) {
        alpha[i][t+1] = alpha[0][t] + prob;
        for (int j = 1; j < state_alphabet_size; j++) {
          alpha[i][t+1] = log_sum(
              alpha[i][t+1],
              alpha[j][t] + prob);
        }
        alpha[i][t+1] += prob;
      }
    }

    double sum =  alpha[0][sequence_size-1];
    for (int k = 1; k < state_alphabet_size; k++) {
      sum = log_sum(sum, alpha[k][sequence_size-1]);
    }
  }
}
BENCHMARK(BM_ForwardAlgorithmWithoutProbability)->Range(1024, 4*1024*1024);

static void BM_ForwardAlgorithmWithProbability(benchmark::State& state) {
  while (state.KeepRunning()) {
    auto state_alphabet_size = 10;
    auto sequence_size = state.range_x();

    auto alpha = std::vector<std::vector<probability::probability_t>>(
        state_alphabet_size,
        std::vector<probability::probability_t>(sequence_size));

    probability::probability_t prob(0.000000000005);

    for (int k = 0; k < state_alphabet_size; k++)
      alpha[k][0] = prob * prob;

    for (int t = 0; t < sequence_size - 1; t++) {
      for (int i = 0; i < state_alphabet_size; i++) {
        alpha[i][t+1] = alpha[0][t] * prob;
        for (int j = 1; j < state_alphabet_size; j++) {
          alpha[i][t+1] += alpha[j][t] * prob;
        }
        alpha[i][t+1] *= prob;
      }
    }

    double sum =  alpha[0][sequence_size-1];
    for (int k = 1; k < state_alphabet_size; k++) {
      sum += alpha[k][sequence_size-1];
    }
  }
}
BENCHMARK(BM_ForwardAlgorithmWithProbability)->Range(1024, 4*1024*1024);
