import neuron;
import rfa;
import testdata;

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

template <unsigned N> class layer {
  neuron<2> m_ns[N]{};

public:
  layer() = default;
  layer(const layer<N> (&p)[2]) {
    for (auto i = 0; i < N; i++) {
      m_ns[i] = neuron{{p[0].m_ns[i], p[1].m_ns[i]}};
    }
  }

  rfa<N> fwd(const rfa<2> &in) {
    rfa<N> res{};
    for (auto i = 0; i < N; i++) {
      res[i] = m_ns[i].fwd(in);
    }
    return res;
  }

  void dump() const {
    for (const auto &n : m_ns) {
      n.dump();
    }
  }
};
class network {
  layer<2> m_int{};
  layer<1> m_out{};
  float m_cost{};

public:
  network() = default;
  network(const network (&p)[2]) {
    m_int = layer<2>{{p[0].m_int, p[1].m_int}};
    m_out = layer<1>{{p[0].m_out, p[1].m_out}};
  }

  float fwd(const rfa<2> &in) { return m_out.fwd(m_int.fwd(in))[0]; }

  void update_cost(const test_suit &suit) {
    float f = 0;
    for (const auto &d : suit.data) {
      auto err = fwd(d.in) - d.out[0];
      f += err * err;
    }
    m_cost = f / static_cast<float>(4);
  }
  constexpr const auto cost() const { return m_cost; }

  void dump() const {
    printf("int:\n");
    m_int.dump();
    printf("out:\n");
    m_out.dump();
  }
};

class population {
  static constexpr const auto pop_size = 10;
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

    ind_t parents[2];
    for (auto i = 0; i < 2; i++) {
      parents[i] = m_ns[i];
    }
    for (auto &n : m_ns) {
      n = ind_t{parents};
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
    for (auto gen = 0; gen < max_gens; gen++) {
      fitness(suit);
      generation();
    }

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
  static constexpr const auto max_gens = 1000;

  srand(time(0));

  population p{};
  p.survive(or_data, max_gens);
  p.survive(and_data, max_gens);
  p.survive(xor_data, max_gens);
}
