export module network;
import dbg;
import layer;
import rfa;
import testdata;

export class network {
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
    dbg::print("int:");
    m_int.dump();
    dbg::print("out:");
    m_out.dump();
  }
};
