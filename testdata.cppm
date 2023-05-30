export module testdata;
import hai;
import rfa;

export struct test {
  rfa in{2};
  rfa out{1};
};
export using test_suit = hai::varray<test>;
