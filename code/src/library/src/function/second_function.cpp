#include "second_function.h"

#include <cmath>
#include <cassert>

SecondFunction::SecondFunction() {
  this->name = "p*cos(p*x)+p";
}

IntegrationLibrary::A SecondFunction::evaluate(std::string p, double x) {
  assert(checkParameter(p));

  auto p_v = to_number<IntegrationLibrary::A>(p);
  IntegrationLibrary::A f = p_v * cos(p_v * x) + p_v;
  return f;
}

bool SecondFunction::checkParameter(std::string p) {
  return is_number<IntegrationLibrary::A>(p);
}