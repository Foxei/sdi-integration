#include "third_function.h"

#include <cmath>
#include <cfloat>
#include <cassert>

ThirdFunction::ThirdFunction() {
  this->name = "sin(x)+sin(p*x)/p";
}

IntegrationLibrary::A ThirdFunction::evaluate(std::string p, double x) {
  assert(checkParameter(p));

  auto p_v = to_number<IntegrationLibrary::A>(p);
  IntegrationLibrary::A f = sin(x) + sin(p_v * x) / p_v;
  return f;
}

bool ThirdFunction::checkParameter(std::string p) {
  if(!is_number<IntegrationLibrary::A>(p)) return false;
  auto p_v = to_number<IntegrationLibrary::A>(p);
  bool value = (std::abs(p_v) > FLT_EPSILON);
  return value;
}