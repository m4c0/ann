export module rfa;
import hai;
export import rng;

export template <unsigned N> class rfa : public hai::array<float> {

public:
  rfa() : array{N} {
    for (auto &f : *this)
      f = rng::randf() * 10.0f - 5.0f;
  }

  void operator=(const rfa<N> &o) {
    for (auto i = 0; i < N; i++) {
      (*this)[i] = o[i];
    }
  }
};
