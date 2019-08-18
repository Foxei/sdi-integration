#ifndef INTEGRATION_LIBRARY_INTEGRATION_H
#define INTEGRATION_LIBRARY_INTEGRATION_H

#include "math_definitions.h"
#include "IntergrationLibrary/numeric_definition.h"

#include <vector>
#include <string>

/**
 * Abstract base class to classes FirstIntegration, SecondIntegration, ThirdIntegration.
 */
class Integration {

 protected:

  std::string name_;
  std::vector<IntegrationLibrary::A> bar_heights_;

 public:

  virtual ~Integration() = default;

  /**
   * @param a Is the lower bound of the interval.
   * @param b Is the upper bound of the interval.
   * @param m is the amount of nodes used to divide the interval into subintervals, the minimum being m=2, as the lower
   * and upper bound are included and must always be set.
   * @param p is a paramater used within the function to be integrated.
   * @param function_index is the index of the function to be integrated in FunctionManager::functions_in_register,
   * containing all supported functions.
   * @return Returns the value of function at each interval node, the last entry in the vector being the total area
   * under the graph.
   */
  virtual IntegrationLibrary::A evaluate(double a, double b, unsigned int m, std::string p,
                     size_t function_index) = 0;

  /**
   * Heights will be calculated on the fly by evaluate.
   * @return bar heights of last evaluation
   */
  std::vector<IntegrationLibrary::A> getBarHeights() {
    return bar_heights_;
  }
  /**
   * Used for integration method selection in GUI.
   * @return Returns the name of the integration method.
   */
  std::string getName() {
    return name_;
  }
};

#endif //INTEGRATION_LIBRARY_INTEGRATION_H
