#include <stdio.h>

import algo;
import rng;
import testdata;

int main() {
  static constexpr const auto max_gens = 1000;

  test_suit data{};

  for (unsigned i = 0; i < 4; i++) {
    for (unsigned j = 0; j < 2; j++) {
      scanf("%f,", &data[i].in[j]);
    }
    scanf("%f\n", &data[i].out[0]);
  }

  rng::seed();
  (new algo::weightned())->survive(data, max_gens);
}
