#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float randf() {
  return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

struct test {
  float in[2];
  float out[1];
};
struct test_suit {
  test data[4];
};

constexpr const test_suit or_data = {{
    {{0, 0}, {0}},
    {{0, 1}, {1}},
    {{1, 0}, {1}},
    {{1, 1}, {1}},
}};
constexpr const test_suit and_data = {{
    {{0, 0}, {0}},
    {{0, 1}, {0}},
    {{1, 0}, {0}},
    {{1, 1}, {1}},
}};
constexpr const test_suit xor_data = {{
    {{0, 0}, {0}},
    {{0, 1}, {1}},
    {{1, 0}, {1}},
    {{1, 1}, {0}},
}};

class rfa {
  float m_data[2];

public:
  rfa() {
    for (auto &f : m_data)
      f = randf() * 10.0f - 5.0f;
  }
  rfa(const float (&n)[2]) {
    for (auto i = 0; i < 2; i++) {
      m_data[i] = n[i];
    }
  }

  auto operator[](unsigned idx) const noexcept { return m_data[idx]; }
  auto &operator[](unsigned idx) noexcept { return m_data[idx]; }

  auto begin() const noexcept { return &m_data[0]; }
  auto end() const noexcept { return &m_data[2]; }
};
class neuron {
  rfa m_w{};
  rfa m_b{};
  float m_cost{};

  static float sigm(float a) {
    float ea = expf(a);
    return ea / (ea + 1.0f);
  }
  static float mutation() {
    constexpr const float eps = 1e-1;
    return (randf() - 0.5f) * eps;
  }

public:
  neuron() = default;
  neuron(const neuron (&ps)[2]) {
    m_w[0] = ps[rand() % 2].m_w[0] + mutation();
    m_w[1] = ps[rand() % 2].m_w[1] + mutation();
    m_b[0] = ps[rand() % 2].m_b[0] + mutation();
    m_b[1] = ps[rand() % 2].m_b[1] + mutation();
  }

  constexpr float cost() const { return m_cost; }

  float fwd(const rfa &in) const {
    float a = 0;
    for (auto i = 0; i < 2; i++) {
      a += m_w[i] * in[i] + m_b[i];
    }
    return sigm(a);
  }

  void update_cost(const test_suit &suit) {
    float cost = 0;
    for (const auto &set : suit.data) {
      float f = fwd(set.in);
      float err = f - set.out[0];
      cost += err * err;
    }
    m_cost = cost / 4.0f;
  }

  void dump() {
    for (auto w : m_w)
      printf("%f ", w);
    for (auto b : m_b)
      printf("%f ", b);
    printf("\n");
  }
};

template <unsigned N> class layer {
  neuron m_ns[N]{};
  float m_cost{};

public:
  layer() = default;
  layer(const layer<N> (&p)[2]) {
    for (auto i = 0; i < N; i++) {
      m_ns[i] = neuron{{p[0].m_ns[i], p[1].m_ns[i]}};
    }
  }

  void update_cost(const test_suit &suit) {
    float f = 0;
    for (auto &n : m_ns) {
      n.update_cost(suit);
      f += n.cost();
    }
    m_cost = f / static_cast<float>(N);
  }
  constexpr const auto cost() const { return m_cost; }

  float fwd(const rfa &in) {
    // This only makes sense for layer<1>
    return m_ns[0].fwd(in);
  }
};

class population {
  static constexpr const auto pop_size = 10;
  using ind_t = layer<1>;
  ind_t m_ns[pop_size]{};

public:
  void generation() {
    qsort(m_ns, pop_size, sizeof(ind_t),
          [](const void *a, const void *b) -> int {
            auto na = static_cast<const ind_t *>(a);
            auto nb = static_cast<const ind_t *>(b);
            return na->cost() - nb->cost() > 0 ? 1 : -1;
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

  float eval(const rfa &in) { return m_ns[0].fwd(in); }

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
    // m_ns[0].dump();
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
