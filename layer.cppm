export module layer;
import neuron;
import rfa;

export template <unsigned N> class layer {
  neuron<2> m_ns[N]{};

public:
  layer() = default;
  layer(const layer<N> (&p)[2]) {
    for (auto i = 0; i < N; i++) {
      m_ns[i] = {{p[0].m_ns[i], p[1].m_ns[i]}};
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
