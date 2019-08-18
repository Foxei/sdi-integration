#include "third_integration.h"
#include "IntergrationLibrary/function_manager.h"

#include <cassert>

ThirdIntegration::ThirdIntegration() {
  this->name_ = "Trapezoid rule";
}

/**
 * @see Integration::evaluate(double a, double b, unsigned int m, double p, size_t function_index) For function
 * details.
 */
IntegrationLibrary::A ThirdIntegration::evaluate(double a, double b, unsigned int m, std::string p, size_t function_index) {
  //Assert that the upper and lower bounds are said and that the upper bound is greater than the lower bound.
  //The bounds being equal would in a redundant result = 0.
  assert(a < b);
  assert(m > 1);

  FunctionManager *manager = &FunctionManager::getInstance();
  //The trapezoidal rule estimates the integral by approximating the area und the graph in each subinterval
  //using a trapezoid and calculating its area.
  //The mathematical formula for the trapezoidal rule is: ((b-a)/2*m)*Sum(i=1:m) (f(xi=1) +f(xi)).
  //factor splits out interval [a,b] into equal subintervals, which are later evaluated.
  double factor = (b - a) / m;
  IntegrationLibrary::A result = 0;
  std::vector<IntegrationLibrary::A> result_vec;

  double x = a;
  //evaluate first node, a, of the interval.
  IntegrationLibrary::A f = manager->evaluateFunction(function_index, p, x);

  //interate through the remaining interval using the area of the trapezoid in each subinterval.
  for (unsigned int i = 0; i < m; i++) {
    x = a + factor * (i + 1);
    IntegrationLibrary::A f1 = manager->evaluateFunction(function_index, p, x);

    //trapezoid formula
    IntegrationLibrary::A g = 0.5 * (f + f1);

    //add current area to result.
    result += factor * g;

    //push current area to vector.
    result_vec.push_back(g);

    f = f1;
  }
  this->bar_heights_ = result_vec;
  return result;
}