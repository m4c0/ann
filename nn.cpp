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

  static float sigm(float a) {
    float ea = expf(a);
    return ea / (ea + 1.0f);
  }

public:
  float fwd(const rfa &in) const {
    float a = 0;
    for (auto i = 0; i < 2; i++) {
      a += m_w[i] * in[i] + m_b[i];
    }
    return sigm(a);
  }
};

float cost(const neuron &n, const test_suit &suit) {
  float cost = 0;
  for (const auto &set : suit.data) {
    float f = n.fwd(set.in);
    float err = f - set.out[0];
    cost += err * err;
  }
  return cost / 4.0f;
}

int main() {
  constexpr const float eps = 1e-1;
  constexpr const float rate = 1e-1;

  srand(time(0));

  neuron n;
  float c = cost(n, or_data);

  printf("%f\n", c);
}
