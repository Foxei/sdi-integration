#ifndef INTEGRATION_LIBRARY_SECOND_FUNCTION_H
#define INTEGRATION_LIBRARY_SECOND_FUNCTION_H

#include "function.h"

/**
 * Second derived class from abstract class Function, implements function 'p*cos(p*x)+p'.
 */
class SecondFunction : public Function {

 public:
  /**
   * Set function name.
   */
  SecondFunction();

  /**
   * Main evaluation function.
   * @see Function::evaluate(double p, double x) For function details.
   */
  IntegrationLibrary::A evaluate(std::string p, double x) override;

  /**
   * For the second function, p is never out of scope.
   * @see Function::checkParameter(double p) For function details.
   */
  bool checkParameter(std::string p) override;
};

#endif //INTEGRATION_LIBRARY_SECOND_FUNCTION_H
