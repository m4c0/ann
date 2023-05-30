#include <stdio.h>

import algo;
import rng;
import testdata;

int main() {
  static constexpr const auto max_gens = 1000;

  test_suit data{4};

  printf("Reading samples\n");
  for (unsigned i = 0; i < 4; i++) {
    test t{};
    for (unsigned j = 0; j < 2; j++) {
      scanf("%f,", &t.in[j]);
    }
    scanf("%f\n", &t.out[0]);
    data.add(t);
  }

  printf("Running AI\n");
  rng::seed();
  (new algo::weightned())->survive(data, max_gens);
}
