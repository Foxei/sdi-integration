#include "first_function.h"

#include <cmath>
#include <cassert>
#include <iostream>

FirstFunction::FirstFunction() {
  this->name = "p*sin(x)+cos(p*x)+p";
}

IntegrationLibrary::A FirstFunction::evaluate(std::string p, double x) {
  assert(checkParameter(p));

  auto p_v = to_number<IntegrationLibrary::A>(p);
  IntegrationLibrary::A f = p_v * sin(x) + cos(p_v * x) + p_v;
  return f;
}

bool FirstFunction::checkParameter(std::string p) {
  return is_number<IntegrationLibrary::A>(p);
}