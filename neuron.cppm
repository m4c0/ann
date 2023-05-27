module;
#include <math.h>
#include <stdio.h>

export module neuron;
import rfa;
import testdata;

export class neuron {
  rfa<2> m_w{};
  rfa<2> m_b{};
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

  float fwd(const rfa<2> &in) const {
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
