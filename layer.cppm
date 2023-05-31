export module layer;
import neuron;
import rfa;
import traits;

export template <unsigned N> class layer {
  neuron<2> m_ns[N]{};

public:
  void inherit(const layer<N> &a, const layer<N> &b) {
    for (auto i = 0; i < N; i++) {
      m_ns[i].inherit(a.m_ns[i], b.m_ns[i]);
    }
  }

  constexpr void fwd(const rfa &in, rfa &out) const {
    for (auto i = 0; i < N; i++) {
      out[i] = m_ns[i].fwd(in);
    }
  }

  void dump() const {
    for (const auto &n : m_ns) {
      n.dump();
    }
  }
};
