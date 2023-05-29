import bestpair;
import testdata;

#include <stdlib.h>
#include <time.h>

int main() {
  static constexpr const auto max_gens = 1000;

  srand(time(0));

  best_pair_algo{}.survive(or_data, max_gens);
  best_pair_algo{}.survive(and_data, max_gens);

  best_pair_algo p{};
  p.survive(xor_data, max_gens);
  p.survive(xor_data, max_gens);
}
