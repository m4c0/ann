export module testdata;

export struct test {
  float in[2];
  float out[1];
};
export class test_suit {
  static constexpr const auto samples = 4;

  test m_data[samples]{};

public:
  const test *begin() const { return &m_data[0]; }
  const test *end() const { return &m_data[samples]; }

  test &operator[](unsigned i) { return m_data[i]; }
};
