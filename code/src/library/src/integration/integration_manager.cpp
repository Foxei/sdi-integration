#include "IntergrationLibrary/integration_manager.h"

#include "first_integration.h"
#include "second_integration.h"
#include "third_integration.h"

void IntegrationManager::init() {
  registerIntegration(new FirstIntegration);
  registerIntegration(new SecondIntegration);
  registerIntegration(new ThirdIntegration);
}

void IntegrationManager::free() {
  for (Integration *integration:integration_in_register) {
    delete integration;
  }
}

void IntegrationManager::registerIntegration(Integration *integration) {
  integration_in_register.push_back(integration);
}

size_t IntegrationManager::getNumberRegistertedIntegration() {
  return integration_in_register.size();
}

std::string IntegrationManager::getIntegrationName(size_t index) {
  assert(integration_in_register.size() > index);
  return integration_in_register.at(index)->getName();
}

IntegrationLibrary::A IntegrationManager::evaluateIntegral(size_t index, double a, double b, unsigned int m, std::string p, size_t function_index) {
  assert(integration_in_register.size() > index);
  return integration_in_register.at(index)->evaluate(a, b, m, p, function_index);
}

std::vector<IntegrationLibrary::A> IntegrationManager::getBarHeight(size_t index) {
  assert(integration_in_register.size() > index);
  return integration_in_register.at(index)->getBarHeights();
}