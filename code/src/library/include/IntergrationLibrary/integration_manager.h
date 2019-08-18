#ifndef INTEGRATION_LIBRARY_INTEGRATION_MANAGER_H
#define INTEGRATION_LIBRARY_INTEGRATION_MANAGER_H

class Integration;

#include "numeric_definition.h"

#include <cassert>
#include <vector>
#include <string>
#include <cstdlib>

/**
 * This class handles all declared integration methods and allows easier invocation for the GUI implementation.\n
 * To fill #integration_in_register with an object of class Integration,
 * init() allocates memory, which later has to be deallocated using free().\n
 */
class IntegrationManager {
 private:
  /**
   * Used to store all integration methods imported by IntegrationManager::init().
   */
  std::vector<Integration*> integration_in_register;
 public:

  /**
   * To include self-implemented integration methods into the library:\n
   * Write the mandatory .h and .cpp and include the .h header.\n
   * Finally, in init(), invoke registerIntegration() with said method.
   * @return Returns static instance of class IntegrationManager.
   */
  static IntegrationManager &getInstance() {
    static IntegrationManager instance;
    return instance;
  }

  /**
   * Adds included integration methods to the #integration_in_register.
   * @see IntegrationManager::registerIntegration()
   */
  void init();

  /**
   * Deletes all integration methods previously loaded by init().
   */
  void free();

  /**
   * Function used by init() to fill #integration_in_register with included integration methods.
   * The indices in the vector are:\n
   * Index for FirstIntegration:	0.\n
   * Index for SecondIntegration:	1.\n
   * Index for ThirdIntegration:	2.\n
   * @see IntegrationManager::init()
   */
  void registerIntegration(Integration *intigration);

  /**
   * @return Returns number of registered integration methods in #integration_in_register.
   */
  size_t getNumberRegistertedIntegration();
  /**
   * @param index Index of integration method in #integration_in_register.
   * @return Returns the name of the integration method in #integration_in_register at index.
   */
  std::string getIntegrationName(size_t index);

  /**
   * Evaluates integral in #integration_in_register at index at the point x with parameter p using function in FunctionManager::functions_in_register at function_index.
   * @see Integration::evaluate() For function details.
   */
  IntegrationLibrary::A evaluateIntegral(size_t index, double a, double b, unsigned int m, std::string p, size_t function_index);
  /**
   * Get access to the on the fly calculated bar heights.
   * @see Integration::getBarHieght() For function details.
   */
  std::vector<IntegrationLibrary::A> getBarHeight(size_t index);
};

#endif //INTEGRATION_LIBRARY_INTEGRATION_MANAGER_H
