export module testdata;
import hai;
import rfa;

export struct test {
  rfa<2> in;
  rfa<1> out;
};
export using test_suit = hai::varray<test>;
