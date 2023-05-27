module;
#include <stdlib.h>

export module rfa;

export float randf() {
  return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

export template <unsigned N> class rfa {
  float m_data[N];

public:
  rfa() {
    for (auto &f : m_data)
      f = randf() * 10.0f - 5.0f;
  }
  rfa(const float (&n)[N]) {
    for (auto i = 0; i < 2; i++) {
      m_data[i] = n[i];
    }
  }

  auto operator[](unsigned idx) const noexcept { return m_data[idx]; }
  auto &operator[](unsigned idx) noexcept { return m_data[idx]; }

  auto begin() const noexcept { return &m_data[0]; }
  auto end() const noexcept { return &m_data[N]; }
};
