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

int main() {}
