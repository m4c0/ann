import algo;
import rng;
import testdata;

int main() {
  static constexpr const auto max_gens = 1000;

  rng::seed();

  algo::best_pair{}.survive(or_data, max_gens);
  algo::best_pair{}.survive(and_data, max_gens);

  algo::best_pair p{};
  p.survive(xor_data, max_gens);
  p.survive(xor_data, max_gens);
}
