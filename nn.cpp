#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float random() {
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
      f = random() * 10.0f - 5.0f;
  }
  rfa(const float (&n)[2]) {
    for (auto i = 0; i < 2; i++) {
      m_data[i] = n[i];
    }
  }

  auto operator[](unsigned idx) const noexcept { return m_data[idx]; }
};
class neuron {
  rfa m_w{};
  rfa m_b{};
  float m_cost{};

  static float sigm(float a) {
    float ea = expf(a);
    return ea / (ea + 1.0f);
  }

public:
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
};

int main() {
  constexpr const float eps = 1e-1;
  constexpr const float rate = 1e-1;

  srand(time(0));

  neuron ns[10];
  for (auto &n : ns) {
    n.update_cost(or_data);
  }
  for (const auto &n : ns) {
    printf("%f\n", n.cost());
  }
  printf("\n");
  qsort(ns, 10, sizeof(neuron), [](const void *a, const void *b) -> int {
    auto na = static_cast<const neuron *>(a);
    auto nb = static_cast<const neuron *>(b);
    return na->cost() - nb->cost() > 0 ? 1 : -1;
  });
  for (const auto &n : ns) {
    printf("%f\n", n.cost());
  }
}
