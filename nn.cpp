import algo;
import rng;
import testdata;

int main() {
  static constexpr const auto max_gens = 1000;
  using alg = algo::best_pair;

  rng::seed();

  alg{}.survive(or_data, max_gens);
  alg{}.survive(and_data, max_gens);

  alg p{};
  p.survive(xor_data, max_gens);
  p.survive(xor_data, max_gens);
}
