#ifndef INTEGRATION_LIBRARY_FIRST_FUNCTION_H
#define INTEGRATION_LIBRARY_FIRST_FUNCTION_H

#include "function.h"


/**
 * First derived class from abstract class Function, declares function 'p*sin(x)+cos(p*x)+p'.
 */
class FirstFunction : public Function {

 public:
  /**
   * Set function name.
   */
  FirstFunction();

  /**
   * @see Function::evaluate(double p, double x) For function details.
   */
  IntegrationLibrary::A evaluate(std::string p, double x) override;

  /**
   * For the first function, p is never out of scope.
   * @see Function::checkParameter(double p) For function details.
   */
  bool checkParameter(std::string p) override;
};

#endif //INTERGRATION_LIBRARY_FIRST_FUNCTION_H
