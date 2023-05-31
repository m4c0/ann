module;
#include <stdlib.h>

export module algo;
import dbg;
import hai;
import network;
import rfa;
import testdata;
import traits;

namespace algo {
template <unsigned PopSize> class base {
protected:
  static constexpr const auto pop_size = PopSize;
  hai::array<network> m_ns{pop_size};

  float eval(const rfa &in) const { return best().fwd(in); }
  void dump() const { best().dump(); }

  float cost(const test_suit &suit) {
    float f = 0;
    for (auto &n : m_ns) {
      n.update_cost(suit);
      f += n.cost();
    }
    return f;
  }

  virtual void generation() = 0;
  virtual float fitness(const test_suit &suit) = 0;
  virtual const network &best() const = 0;

public:
  void survive(const test_suit &suit, unsigned max_gens) {
    unsigned gen;
    for (gen = 0; gen < max_gens; gen++) {
      fitness(suit);
      if (best().cost() < 1e-6)
        break;
      generation();
    }

    dbg::print("after %d gens:\n", gen);
    for (const auto &t : suit) {
      for (auto i : t.in)
        dbg::print("%.0f ", i);
      float e = eval(t.in);
      dbg::print("= %.0fv%.0f r6:%f\n", t.out[0], e, e);
    }
    dbg::print("best = %f\n", best().cost());
    dump();
    dbg::print("-=-=-=-=-=-=-=-=-=-=-=-=-\n");
  }
};

export class weightned : public base<10000> {
  network m_best{};
  decltype(m_ns) next{pop_size};
  float m_costs[pop_size]{};

  float fitness(const test_suit &suit) override {
    float sum_f = cost(suit);
    float new_sum = 0; // TODO: confirm this is "sum_f * 99"
    for (auto i = 0; i < pop_size; i++) {
      auto &n = m_ns[i];
      if (n.cost() < m_best.cost())
        m_best = n;
      m_costs[i] = sum_f - n.cost();
      new_sum += m_costs[i];
    }
    float acc = 0;
    for (auto &c : m_costs) {
      acc += c;
      c = acc / new_sum;
    }
    return sum_f / pop_size;
  }

  void generation() override {
    for (auto i = 0; i < pop_size; i++) {
      next[i].inherit(pick(), pick());
    }

    auto tmp = traits::move(next);
    next = traits::move(m_ns);
    m_ns = traits::move(tmp);
  }

  const network &pick() const {
    float r = rng::randf();
    for (auto i = 0; i < pop_size; i++) {
      if (m_costs[i] > r)
        return m_ns[i];
    }
    dbg::print("unreachable state: %f %f\n", r, m_costs[pop_size - 1]);
    throw 0;
  }

  const network &best() const override { return m_best; }
};

export class best_pair : public base<100> {
  static constexpr const auto parent_pool_size = pop_size / 10;

  void generation() override {
    qsort(m_ns.begin(), pop_size, sizeof(network),
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
      n.inherit(pp[p1], pp[p2]);
    }
  }

  float fitness(const test_suit &suit) override {
    return cost(suit) / static_cast<float>(pop_size);
  }

  const network &best() const override { return m_ns[0]; }
};
} // namespace algo
