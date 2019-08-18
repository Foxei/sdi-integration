#ifndef INTEGRATION_LIBRARY_THIRD_FUNCTION_H
#define INTEGRATION_LIBRARY_THIRD_FUNCTION_H

#include "function.h"

/**
 * Third derived class from abstract class Function, declares function 'sin(x)+sin(p*x)/p'.
 */
class ThirdFunction : public Function {
 public:
  /**
   * Set function name.
   */
  ThirdFunction();

  /**
   * Main evaluation function
   * @see Function::evaluate(double p, double x) For function details.
   */
  IntegrationLibrary::A evaluate(std::string p, double x) override;

  /**
   * Parameter p is mathematically out of scope if p = 0.
   * For computing, we used the float data type accuracy, FLT_EPSILON,
   * to allow values greater than 1.19209e-07, but not less, since these
   * would result in a computational error due to rounding.
   * @see Function::checkParameter(double p) For function details.
   */
  bool checkParameter(std::string p) override;
};

#endif //INTEGRATION_LIBRARY_THIRD_FUNCTION_H

