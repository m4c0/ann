#include "../ecow/ecow.hpp"
#include "../rng/build.hpp"

int main(int argc, char **argv) {
  using namespace ecow;

  auto nn = unit::create<tool>("nn");
  nn->add_wsdep("rng", rng());
  nn->add_unit<mod>("dbg")->add_impl("dbgimpl");
  nn->add_unit<mod>("testdata");
  nn->add_unit<mod>("rfa");
  nn->add_unit<mod>("neuron");
  nn->add_unit<mod>("layer");
  nn->add_unit<mod>("network");
  nn->add_unit<>("nn");
  return run_main(nn, argc, argv);
}
