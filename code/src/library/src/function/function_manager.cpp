#include "IntergrationLibrary/function_manager.h"

#include "first_function.h"
#include "second_function.h"
#include "third_function.h"

void FunctionManager::init() {
  registerFunction(new FirstFunction);
  registerFunction(new SecondFunction);
  registerFunction(new ThirdFunction);
}

void FunctionManager::free() {
  for (Function *function:functions_in_register) {
    delete function;
  }
}

void FunctionManager::registerFunction(Function *function) {
  functions_in_register.push_back(function);
}

size_t FunctionManager::getNumberRegisteredFunctions() {
  return functions_in_register.size();
}

std::string FunctionManager::getFunctionName(size_t index) {
  assert(functions_in_register.size() > index);
  return functions_in_register.at(index)->getName();
}

bool FunctionManager::checkParameter(size_t index, std::string p) {
  assert(functions_in_register.size() > index);
  return functions_in_register.at(index)->checkParameter(std::move(p));
}

IntegrationLibrary::A FunctionManager::evaluateFunction(size_t index, std::string p, double x) {
  assert(functions_in_register.size() > index);
  return functions_in_register.at(index)->evaluate(std::move(p), x);
}