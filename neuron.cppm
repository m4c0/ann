module;
#include <math.h>
#include <stdio.h>

export module neuron;
import rfa;
import testdata;

export template <unsigned Ins> class neuron {
  rfa<Ins> m_w{};
  rfa<Ins> m_b{};
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
    for (auto i = 0; i < Ins; i++) {
      m_w[i] = ps[rand() % 2].m_w[i] + mutation();
      m_b[i] = ps[rand() % 2].m_b[i] + mutation();
    }
  }

  constexpr float cost() const { return m_cost; }

  float fwd(const rfa<Ins> &in) const {
    float a = 0;
    for (auto i = 0; i < Ins; i++) {
      a += m_w[i] * in[i] + m_b[i];
    }
    return sigm(a);
  }

  void dump() {
    for (auto w : m_w)
      printf("%f ", w);
    for (auto b : m_b)
      printf("%f ", b);
    printf("\n");
  }
};
