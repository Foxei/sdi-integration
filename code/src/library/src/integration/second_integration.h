#ifndef INTEGRATION_LIBRARY_SECOND_INTEGRATION_H
#define INTEGRATION_LIBRARY_SECOND_INTEGRATION_H

#include "integration.h"

/**
 * Second derived class from abstract class Integration, declares the 'upper Darboux sums' method.
 */
class SecondIntegration : public Integration {

 public:
  /**
   * Set name of integration function
   */
  SecondIntegration();

  /**
   * @see Integration::evaluate(double a, double b, unsigned int m, double p, size_t function_index) For function
   * details.
   */
  IntegrationLibrary::A evaluate(double a, double b, unsigned int m, std::string p, size_t function_index) override;
};

#endif //INTEGRATION_LIBRARY_SECOND_INTEGRATION_H
