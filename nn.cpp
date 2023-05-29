import algo;
import rng;
import testdata;

int main() {
  static constexpr const auto max_gens = 1000;
  using alg = algo::weightned;

  rng::seed();

  (new alg())->survive(or_data, max_gens);
  (new alg())->survive(and_data, max_gens);

  alg *p = new alg{};
  p->survive(xor_data, max_gens);
  p->survive(xor_data, max_gens);
}
