export module testdata;

export struct test {
  float in[2];
  float out[1];
};
export class test_suit {
  unsigned m_samples{};
  test *m_data{};

public:
  test_suit(unsigned max_samples) : m_data{new test[max_samples]} {}

  const test *begin() const { return &m_data[0]; }
  const test *end() const { return &m_data[m_samples]; }

  void add(const test &t) { m_data[m_samples++] = t; }
};
