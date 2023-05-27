#include "../ecow/ecow.hpp"

int main(int argc, char **argv) {
  using namespace ecow;

  auto nn = unit::create<tool>("nn");
  nn->add_unit<>("nn");
  return run_main(nn, argc, argv);
}
