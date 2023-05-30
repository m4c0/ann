export module rfa;
import hai;
export import rng;

export struct rfa : hai::array<float> {
  rfa(unsigned n) : array{n} {
    for (auto &f : *this)
      f = rng::randf() * 10.0f - 5.0f;
  }

  void operator=(const rfa &o) {
    for (auto i = 0; i < size(); i++) {
      (*this)[i] = o[i];
    }
  }
};
