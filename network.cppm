export module network;
import dbg;
import layer;
import rfa;
import testdata;

export class network {
  layer<2> m_int{};
  layer<1> m_out{};
  float m_cost{1e30f};

public:
  network() = default;

  void inherit(const network &a, const network &b) {
    m_int.inherit(a.m_int, b.m_int);
    m_out.inherit(a.m_out, b.m_out);
  }

  float fwd(const rfa &in) const {
    rfa int_out{2};
    m_int.fwd(in, int_out);

    rfa out_out{1};
    m_out.fwd(int_out, out_out);
    return out_out[0];
  }

  void update_cost(const test_suit &suit) {
    float f = 0;
    for (const auto &d : suit) {
      auto err = fwd(d.in) - d.out[0];
      f += err * err;
    }
    m_cost = f / static_cast<float>(4);
  }
  constexpr const auto cost() const { return m_cost; }

  void dump() const {
    dbg::print("int:\n");
    m_int.dump();
    dbg::print("out:\n");
    m_out.dump();
  }
};
