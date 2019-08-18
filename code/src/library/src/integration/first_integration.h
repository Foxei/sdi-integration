#ifndef INTEGRATION_LIBRARY_FIRST_INTEGRATION_H
#define INTEGRATION_LIBRARY_FIRST_INTEGRATION_H

#include "integration.h"

#include <cassert>
#include <cmath>

/**
 * First derived class from abstract class Integration, declares the 'lower Darboux integral' method.
 */
class FirstIntegration : public Integration {

 public:
  /**
   * Set name of integration method.
   */
  FirstIntegration();

  /**
   * Main evaluation function
   * @see Integration::evaluate(double a, double b, unsigned int m, double p, size_t function_index) For function
   * details.
   */
  IntegrationLibrary::A evaluate(double a, double b, unsigned int m, std::string p, size_t function_index) override;
};

#endif //INTEGRATION_LIBRARY_FIRST_INTEGRATION_H