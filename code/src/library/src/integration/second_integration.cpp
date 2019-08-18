#include "second_integration.h"
#include "IntergrationLibrary/function_manager.h"

#include <cassert>

SecondIntegration::SecondIntegration() {
  this->name_ = "Upper sum";
}

IntegrationLibrary::A SecondIntegration::evaluate(double a,
                                                  double b,
                                                  unsigned int m,
                                                  std::string p,
                                                  size_t function_index) {
  //Assert that the upper and lower bounds are said and that the upper bound is greater than the lower bound.
  //The bounds being equal would in a redundant result = 0.
  assert(a < b);
  assert(m > 1);

  FunctionManager *manager = &FunctionManager::getInstance();

  //Upper Darboux sums overestimates integral by using the largest area under the curve in each sub interval.
  //The mathematical formula is: supremum {U(f,P) = Sum(i=1:m) (Xi -Xi-1) * (supremum(x is element [xi-1,xi]) f(x))},
  //with P being a sub interval of [a,b].
  //factor splits out interval [a,b] into equal sub intervals, which are later evaluated.
  double factor = (b - a) / m;
  IntegrationLibrary::A result = 0;
  std::vector<IntegrationLibrary::A> result_vec;

  double x = a;
  //evaluate first node, a, of the interval.
  IntegrationLibrary::A f = manager->evaluateFunction(function_index, p, x);

  //integrate through the remaining interval using largest area under the curve in each sub interval.
  for (unsigned int i = 0; i < m; i++) {
    x = a + factor * (i + 1);
    IntegrationLibrary::A f1 = manager->evaluateFunction(function_index, p, x);

    //choose maximum area
    IntegrationLibrary::A g = std::max(f, f1);

    //add current area to result.
    result += factor * g;

    //push current area to vector.
    result_vec.push_back(g);

    f = f1;
  }
  this->bar_heights_ = result_vec;
  return result;
}