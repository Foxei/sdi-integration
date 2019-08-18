#ifndef INTEGRATION_LIBRARY_THIRD_INTEGRATION_H
#define INTEGRATION_LIBRARY_THIRD_INTEGRATION_H

#include "integration.h"

/**
 * Third derived class from abstract class Integration, declares the 'Trapezoidal rule' method.
 */
class ThirdIntegration : public Integration {

 public:
  /**
   * Set integration function name
   */
  ThirdIntegration();

  /**
   * @see Integration::evaluate(double a, double b, unsigned int m, double p, size_t function_index) For function
   * details.
   */
  IntegrationLibrary::A evaluate(double a, double b, unsigned int m, std::string p, size_t function_index) override;
};

#endif //INTEGRATION_LIBRARY_THIRD_INTEGRATION_H
