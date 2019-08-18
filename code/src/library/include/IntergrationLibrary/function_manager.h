#ifndef INTEGRATION_LIBRARY_FUNCTION_MANAGER_H
#define INTEGRATION_LIBRARY_FUNCTION_MANAGER_H

class Function;

#include "numeric_definition.h"

#include <vector>
#include <cstdlib>
#include <string>
#include <cassert>

/**
 * This class handles all declared functions and allows easier invocation for the GUI implementation.\n
 * To fill #functions_in_register with an object of class Function,
 * init() allocates memory, which later has to be deallocated using function free().
 */
class FunctionManager {
 private:

  /**
   * Used to store all functions imported by FunctionManager::init().
   */
  std::vector<Function*> functions_in_register;

 public:

  /**
   * To include self-implemented Functions into the library:\n
   * Write the mandatory .h and .cpp and include the .h header.\n
   * Finally, in init(), invoke registerFunction() with said function.
   * @return Returns static instance of class FunctionManager.
   */
  static FunctionManager &getInstance() {
    static FunctionManager instance;
    return instance;
  }

  /**
   * Adds included Functions to the #functions_in_register.
   * @see FunctionManager::registerFunction()
   */
  void init();

  /**
   * Deletes all functions previously loaded by init().
   */
  void free();

  /**
   * Function used by init() to fill #functions_in_register with included functions.
   * The indices in the vector are:\n
   * Index for FirstFunction:	0.\n
   * Index for SecondFunction:	1.\n
   * Index for ThirdFunction:	2.\n
   * @see FunctionManager::init()
   */
  void registerFunction(Function *function);

  /**
   * @return Returns number of registered functions in #functions_in_register.
   */
  size_t getNumberRegisteredFunctions();

  /**
   * @param index Index of function in #functions_in_register.
   * @return Returns the name of the function in #functions_in_register at index.
   */
  std::string getFunctionName(size_t index);

  /**
   * Checks parameter p scope of function in #functions_in_register at index .
   * @see Function::checkParameter() For function details.
   */
  bool checkParameter(size_t index, std::string p);

  /**
   * Evaluates function in #functions_in_register at index at the point x with parameter p.
   * @see Function::evaluate() For function details.
   */
  IntegrationLibrary::A evaluateFunction(size_t index, std::string p, double x);
};

#endif //INTEGRATION_LIBRARY_FUNCTION_MANAGER_H