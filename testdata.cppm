export module testdata;
import hai;

export struct test {
  float in[2];
  float out[1];
};
export using test_suit = hai::varray<test>;
