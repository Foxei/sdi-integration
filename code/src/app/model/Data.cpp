#include "Data.h"

Data &Data::getInstance() {
  static Data instance;
  return instance;
}