module;
#include <stdlib.h>

export module algo;
import dbg;
import network;
import rfa;
import testdata;

namespace algo {
template <unsigned Size> class pop {
  network m_ns[Size]{};

public:
  auto &operator[](unsigned idx) { return m_ns[idx]; }
  const auto &operator[](unsigned idx) const { return m_ns[idx]; }
  auto *begin() { return &m_ns[0]; }
  const auto *begin() const { return &m_ns[0]; }
  auto *end() { return &m_ns[Size]; }
  const auto *end() const { return &m_ns[Size]; }
};

template <unsigned PopSize> class base {
protected:
  static constexpr const auto pop_size = PopSize;
  pop<pop_size> m_ns{};

  float eval(const rfa<2> &in) const { return best().fwd(in); }
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
    dump();
    dbg::print("-=-=-=-=-=-=-=-=-=-=-=-=-\n");
  }
};

export class weightned : public base<100> {
  float fitness(const test_suit &suit) override {
    float sum_f = cost(suit);
    float new_sum = 0; // TODO: confirm this is "sum_f * 99"
    for (auto &n : m_ns) {
      n.set_cost(sum_f - n.cost());
      new_sum += n.cost();
    }
    float acc = 0;
    for (auto &n : m_ns) {
      acc += n.cost() / new_sum;
      n.set_cost(acc);
    }
    return sum_f;
  }

  void generation() override {
    const auto parents = m_ns;
    for (auto i = 0; i < pop_size; i++) {
      const auto &p1 = best();
      const auto &p2 = best();
      m_ns[i] = {p1, p2};
    }
  }

  const network &best() const override {
    float r = rng::randf();
    for (const auto &n : m_ns) {
      if (n.cost() > r)
        return n;
    }
    // unreachable
    throw 0;
  }
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
      n = network{pp[p1], pp[p2]};
    }
  }

  float fitness(const test_suit &suit) override {
    return cost(suit) / static_cast<float>(pop_size);
  }

  const network &best() const override { return m_ns[0]; }
};
} // namespace algo
