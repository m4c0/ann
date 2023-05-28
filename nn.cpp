import network;
import rfa;
import testdata;

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

class population {
  static constexpr const auto pop_size = 100;
  using ind_t = network;
  ind_t m_ns[pop_size]{};

public:
  void generation() {
    qsort(m_ns, pop_size, sizeof(ind_t),
          [](const void *a, const void *b) -> int {
            auto na = static_cast<const ind_t *>(a);
            auto nb = static_cast<const ind_t *>(b);
            return na->cost() > nb->cost() ? 1 : -1;
          });

    for (auto &n : m_ns) {
      n = ind_t{{m_ns[0], m_ns[1]}};
    }
  }

  float fitness(const test_suit &suit) {
    float f = 0;
    for (auto &n : m_ns) {
      n.update_cost(suit);
      f += n.cost();
    }
    return f / static_cast<float>(pop_size);
  }

  float eval(const rfa<2> &in) { return m_ns[0].fwd(in); }

  void survive(const test_suit &suit, unsigned max_gens) {
    unsigned gen;
    for (gen = 0; gen < max_gens; gen++) {
      if (fitness(suit) < 1e-6)
        break;
      generation();
    }

    printf("after %d gens:\n", gen);
    for (const auto &t : suit.data) {
      for (auto i : t.in)
        printf("%f ", i);
      printf("%f %f\n", t.out[0], eval(t.in));
    }
    printf("f = %f\n", fitness(suit));
    m_ns[0].dump();
    printf("-=-=-=-=-=-=-=-=-=-=-=-=-\n");
  }
};

int main() {
  static constexpr const auto max_gens = 1000000;

  srand(time(0));

  population{}.survive(or_data, max_gens);
  population{}.survive(and_data, max_gens);
  population{}.survive(xor_data, max_gens);
}
