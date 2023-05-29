module;
#include <stdlib.h>

export module bestpair;
import dbg;
import network;
import rfa;
import testdata;

export class best_pair_algo {
  static constexpr const auto pop_size = 100;
  static constexpr const auto parent_pool_size = pop_size / 10;
  network m_ns[pop_size]{};

public:
  void generation() {
    qsort(m_ns, pop_size, sizeof(network),
          [](const void *a, const void *b) -> int {
            auto na = static_cast<const network *>(a);
            auto nb = static_cast<const network *>(b);
            return na->cost() > nb->cost() ? 1 : -1;
          });

    network pp[parent_pool_size];
    for (auto i = 0; i < parent_pool_size; i++) {
      pp[i] = m_ns[i];
    }

    for (auto &n : m_ns) {
      auto p1 = rng::rand(parent_pool_size);
      auto p2 = rng::rand(parent_pool_size);
      n = network{pp[p1], pp[p2]};
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

    dbg::print("after %d gens:\n", gen);
    for (const auto &t : suit.data) {
      for (auto i : t.in)
        dbg::print("%f ", i);
      dbg::print("%f %f\n", t.out[0], eval(t.in));
    }
    dbg::print("f = %f\n", fitness(suit));
    m_ns[0].dump();
    dbg::print("-=-=-=-=-=-=-=-=-=-=-=-=-\n");
  }
};
