export module neuron;
import dbg;
import rfa;
import rng;
import testdata;

extern "C" float expf(float);

export float sigm(float a) { return 1.0f / (1.0f + expf(-a)); }

class synapse {
  float m_w = rng::randf();
  float m_b = rng::randf();

  static float mutation() {
    constexpr const float eps = 1e1;
    constexpr const float chance = 1e-1;
    if (rng::randf() > chance)
      return 0.0f;
    return (rng::randf() - 0.5f) * eps;
  }

public:
  void inherit(const synapse &a, const synapse &b) {
    const synapse *ps[2]{&a, &b};
    m_w = ps[rng::rand(2)]->m_w + mutation();
    m_b = ps[rng::rand(2)]->m_b + mutation();
  }

  float fwd(float in) const { return m_w * in + m_b; }

  void dump() const { dbg::print("  w=%f b=%f", m_w, m_b); }
};

export template <unsigned Ins> class neuron {
  synapse m_s[Ins];

public:
  void inherit(const neuron<Ins> &a, const neuron<Ins> &b) {
    for (auto i = 0; i < Ins; i++) {
      m_s[i].inherit(a.m_s[i], b.m_s[i]);
    }
  }

  float fwd(const rfa &in) const {
    float a = 0;
    for (auto i = 0; i < Ins; i++) {
      a += m_s[i].fwd(in[i]);
    }
    return sigm(a);
  }

  void dump() const {
    for (auto i = 0; i < Ins; i++) {
      m_s[i].dump();
    }
    dbg::print("\n");
  }
};
