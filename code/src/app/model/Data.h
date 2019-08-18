#ifndef INTEGRATION_GUI_DATA_H
#define INTEGRATION_GUI_DATA_H

#include <cstdlib>
#include <string>

class Data {
 public:

  static constexpr int FUNCTION_FINENESS = 4000;

  static constexpr int DEFAULT_LOWER_BOUND = 0;

  static constexpr int DEFAULT_UPPER_BOUND = 1;

  static constexpr int DEFAULT_NUMBER_POINTS = 20;

  static constexpr int DEFAULT_PARAMETER = 1;

  static Data &getInstance();

  double LOWER_BOUND;

  double UPPER_BOUND;

  unsigned int NUMBER_POINTS;

  std::string PARAMETER;

  size_t FUNCTION_INDEX;

  size_t INTEGRATION_INDEX;

};

#endif //INTEGRATION_GUI_DATA_H
