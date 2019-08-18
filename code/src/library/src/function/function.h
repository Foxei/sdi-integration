#ifndef INTEGRATION_LIBRARY_FUNCTION_H
#define INTEGRATION_LIBRARY_FUNCTION_H

#include "math_definitions.h"
#include "IntergrationLibrary/numeric_definition.h"
#include <string>

/**
 * Abstract base class to classes FirstFunction, SecondFunction, ThirdFunction.
 */
class Function {
 protected:
  /**
   * Name of the function itself.
   */
  std::string name;

 public:

  /**
   * Virtual destructor to get inherited once to fire.
   */
  virtual ~Function() = default;

  /**
   * Main evaluation function.
   * @param x function evaluation point.
   * @param p is used within the function.
   * @return Returns the value of the function at point x.
   */
  virtual IntegrationLibrary::A evaluate(std::string p, double x) = 0;

  /**
   * Function to ensure a correct parameter p.
   * @param p is used within the function.
   * @return Returns true if p is in scope for the function, else false.
   */
  virtual bool checkParameter(std::string p) = 0;

  /**
   * Used for function selection in GUI.
   * @return Returns the mathematical formula of the function.
   */
  std::string getName();
};

#endif //INTEGRATION_LIBRARY_FUNCTION_H
