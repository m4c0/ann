export module neuron;
import dbg;
import rfa;
import rng;
import testdata;

extern "C" float expf(float);

export float sigm(float a) {
  float ea = expf(a);
  return ea / (ea + 1.0f);
}
export template <unsigned Ins> class neuron {
  rfa<Ins> m_w{};
  rfa<Ins> m_b{};
  float m_cost{};

  static float mutation() {
    constexpr const float eps = 1e1;
    constexpr const float chance = 1e-1;
    if (rng::randf() > chance)
      return 0.0f;
    return (rng::randf() - 0.5f) * eps;
  }

public:
  neuron() = default;
  neuron(const neuron (&ps)[2]) {
    for (auto i = 0; i < Ins; i++) {
      m_w[i] = ps[rng::rand(2)].m_w[i] + mutation();
      m_b[i] = ps[rng::rand(2)].m_b[i] + mutation();
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

  void dump() const {
    for (auto i = 0; i < Ins; i++) {
      dbg::print("  w[%d]=%f b[%d]=%f", i, m_w[i], i, m_b[i]);
    }
    dbg::print("\n");
  }
};
